
//#define TEST_PRESSURE_

// air pressure sensor part id MPX5050GP/1
//
#include <ArduinoJson.h>
/*
 * ROS include
 */
#include <ros.h>

#include <std_msgs/String.h>
#include <std_msgs/Float32.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/Float32MultiArray.h>
#include <geometry_msgs/Twist.h>
//#include <vector>

int sensorPin = A1;    // variable to store the value coming from the sensorint sensorPin = A0;  
double sensorValue = 0;  // 
int pump = 3; // Arduino PWM output pin 3; connect to IBT-2 pin 1 (RPWM)
int led_pin=13;
double required_pwm = 180; // duty-cycle to PWM pump
double required_pressure = 45; // set the staining tank pressure
double current_pressure = 1.1; //mBar
double sum = 0.0;
int ctr = 50;
bool is_pump_active = false;

/********************************
 * DB
 * ******************************/
//StaticJsonDocument<200> jsondb;
DynamicJsonDocument jsondb(1024);
/********************************
 * ROS DEFINITIONS
 * ******************************/

#define ISROS
#define ROSSTRING
//#define ROSFLOAT
//#define ROSARRAY
#ifdef ISROS
    #define TOPICNAMEPUB "rdconfigpub"
    #define TOPICNAMESUB "rdconfigsub"
    #define BAUDRATE 57600
    ros::NodeHandle  nh;
    #ifdef ROSSTRING
        std_msgs::String topic_msg;
    #endif
    #ifdef ROSFLOAT
        std_msgs::Float32 topic_msg;
    #endif
     #ifdef ROSARRAY // https://answers.ros.org/question/226726/push-vector-into-multiarray-message-and-publish-it/
        std_msgs::Float32MultiArray topic_msg;
    #endif
    
    void commandVelocityCallback(const geometry_msgs::Twist& cmd_vel_msg);
    void rdconfigsub_callback(const std_msgs::String& str);
    //ros::Subscriber<geometry_msgs::Twist> cmd_vel_sub("cmd_vel", commandVelocityCallback);
    ros::Publisher chatter(TOPICNAMEPUB, &topic_msg);
    ros::Subscriber<std_msgs::String> rdconfigsub("rdconfigsub", rdconfigsub_callback);
#else
    #define BAUDRATE 115200
#endif
/********************************************************
 * myprintf
 * ******************************************************/
void myprintf(const char * fmt, const char* fn, ...)
{
    char chbuf[1024];
    #if 1
    va_list args;
    strncpy(chbuf, fn, 1024);  
    strcat(chbuf, ":");  
    va_start(args, fmt);
    //TODO - use flash and not RAM for string
    vsnprintf(chbuf+strlen(chbuf), sizeof(chbuf)-strlen(chbuf), fmt, args);
    va_end(args); 
    #endif
    nh.loginfo(chbuf); 
    //Serial.printf(chbuf);
}

/******************************************************************************
 * Callback for rdconfigsub topic
 * ****************************************************************************/
void rdconfigsub_callback(const std_msgs::String& str)
{
    //publish_chatter(0);

    const char* fn="rdconfig_callback()";
#define DEBUGROS    
#ifdef DEBUGROS    
    myprintf("str:%s", fn, str.data);
    //char dstr[1024];
    //sprintf(dstr, "%s:str:%s", fn, str.data);
    //nh.loginfo(dstr); 
#endif
    StaticJsonDocument<200> doc;
    String data=String(str.data);
    myprintf("data:%s", fn, data.c_str());
    DeserializationError error=deserializeJson(doc, data.c_str(), 200);
// Test if parsing succeeds.
    if (error) {
        myprintf("deserializeJson() failed: error:%s", fn, error.c_str());
        return;
    }
   
    JsonObject root = doc.as<JsonObject>();
    if (root.containsKey("workmode")){
        jsondb["workmode"]=root["workmode"].as<String>();
    }
    else
    {
        myprintf("Does not contain workmode", fn);
    }

    if (root.containsKey("workstate")){
        jsondb["workstate"]=root["workstate"].as<String>();
    }
    else
    {
        myprintf("Does not contain workstate", fn);
    }
    char output[1024];
    serializeJson(jsondb, output, sizeof(output));

    myprintf("output:%s", fn, output);
    //DEBUG by ROS
    publish_config();

}
/*******************************************************************************
* Callback function for cmd_vel msg
*******************************************************************************/
void commandVelocityCallback(const geometry_msgs::Twist& cmd_vel_msg)
{
#ifdef ISROS
    if (cmd_vel_msg.linear.x<0.5)
    {
        digitalWrite(led_pin,LOW);
        publish_chatter(0);
    }
    else
    {
        digitalWrite(led_pin,HIGH);
        publish_chatter(5);
       
    }
#endif
#if 0
  //goal_velocity_from_cmd[LINEAR]  = cmd_vel_msg.linear.x;
  goal_velocity_from_cmd[LINEAR]  = -cmd_vel_msg.linear.x;    // RAN (11-05-19) - temporary fix to manual controller !!!!
  goal_velocity_from_cmd[ANGULAR] = cmd_vel_msg.angular.z;

  goal_velocity_from_cmd[LINEAR]  = constrain(goal_velocity_from_cmd[LINEAR],  MIN_LINEAR_VELOCITY, MAX_LINEAR_VELOCITY);
  goal_velocity_from_cmd[ANGULAR] = constrain(goal_velocity_from_cmd[ANGULAR], MIN_ANGULAR_VELOCITY, MAX_ANGULAR_VELOCITY);
#endif
}
/***********************
 * setup
 * *********************/
void setup()
{
    jsondb["workmode"]="verylongstring";
    jsondb["workstate"]="verylongstring";

    pinMode(sensorPin, INPUT); // set sensor pin at input
    pinMode(pump, OUTPUT); // set pump pin at output
    pinMode(led_pin,OUTPUT); 

    Serial.begin(BAUDRATE);
    TCCR2B=(TCCR2B&0xF8) | 1;
    /*
     * ROS initialization
     */
#ifdef ISROS
    nh.initNode();
    nh.advertise(chatter);
    //nh.subscribe(cmd_vel_sub);
    nh.subscribe(rdconfigsub);
#endif

}
/*********************
 * publish rdconfigpub
 * ******************/
void publish_config()
{
    const char* fn="publish_config()";
#ifdef ISROS
    #ifdef ROSSTRING
        //String value = String(current_pressure);
        String value; 
        serializeJson(jsondb, value);
        //value="{configuration:'CLEAN'}";
        topic_msg.data = value.c_str();
    #endif
    #ifdef ROSFLOAT
        topic_msg.data = current_pressure;
    #endif
    #ifdef ROSARRAY
        static float vec[3];
        vec[0]=pinlevel;//x
        vec[1]=1.1;//y 
        vec[2]=1.2;//alpha
        std_msgs::MultiArrayDimension dim;
        dim.label="xyalpha";
        dim.size=3;
        dim.stride=3;

        topic_msg.layout.dim=&dim;
        topic_msg.layout.dim_length=1;
        topic_msg.data_length=3;
        // copy in the data
        topic_msg.data=&vec[0];
    #endif
    myprintf("%s", fn, topic_msg.data);
 
    chatter.publish( &topic_msg );
#else
    Serial.println(value);
#endif
 }/*********************
 * publish chatter topic
 * ******************/
void publish_chatter(int pinlevel)
{
#ifdef ISROS
    #ifdef ROSSTRING
        //String value = String(current_pressure);
        String value; 
        value="{configuration:'CLEAN'}";
        topic_msg.data = value.c_str();
    #endif
    #ifdef ROSFLOAT
        topic_msg.data = current_pressure;
    #endif
    #ifdef ROSARRAY
        static float vec[3];
        vec[0]=pinlevel;//x
        vec[1]=1.1;//y 
        vec[2]=1.2;//alpha
        std_msgs::MultiArrayDimension dim;
        dim.label="xyalpha";
        dim.size=3;
        dim.stride=3;

        topic_msg.layout.dim=&dim;
        topic_msg.layout.dim_length=1;
        topic_msg.data_length=3;
        // copy in the data
        topic_msg.data=&vec[0];
    #endif

    chatter.publish( &topic_msg );
#else
    Serial.println(value);
#endif
 }
/*********************
 * loop
 * *******************/ 
void loop()
{
  
    //2V is the minimum to get pressure // 2V = 43 duty-cycle 

    // Update the LCD
    update_screen(current_pressure); // Handle the menu
    menu_handler();

    // Update the LCD
    update_screen(current_pressure);
     
    //analogWrite(3, 100);
    for (ctr=50;ctr>0;ctr--)
    {
        sensorValue = analogRead(sensorPin);
        sum+=sensorValue;
        delayMicroseconds(1000);
    }
    sum = sum/50;
    current_pressure = (sum -36.272)/1.8107; // from sensor valeu to mBar
    /*
     * ROS publish the result
     */
#ifdef ISROS
    //publish_chatter();
    nh.spinOnce(); 
#else
    Serial.println(value);
#endif
 
    //sensorValue = analogRead(sensorPin);
    //current_pressure = (sensorValue -36.272)/1.8107;
    /* if (current_pressure > required_pressure) 
      required_pwm = required_pwm - 1;
    else if (current_pressure < required_pressure)
       required_pwm = required_pwm +1;
    analogWrite(3, pumping);*/

    if (is_pump_active) 
    {
        PID_pressure_control( required_pressure, current_pressure, 3.0, 0.15,  0.0);
    }
    else
    {
        analogWrite(3, 0);
    }
    //Serial.print(required_pwm);
    //Serial.print( " ");
    //Serial.println(current_pressure);
#ifndef ISROS
    Serial.print("is_pump_active  ");
    Serial.println(is_pump_active);
#endif
    delay(1);
}
/***********************************
 * START command 
 * *********************************/
int start()
{
    int ret=0;
    return (ret);
}
/***********************************
 * STOP command 
 * *********************************/
int stop()
{
    int ret=0;
    return (ret);
}
