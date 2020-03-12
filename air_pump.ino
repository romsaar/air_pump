
//#define TEST_PRESSURE_

// air pressure sensor part id MPX5050GP/1
//
/*
 * ROS include
 */
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float32.h>

int sensorPin = A1;    // variable to store the value coming from the sensorint sensorPin = A0;  
double sensorValue = 0;  // 
int pump = 3; // Arduino PWM output pin 3; connect to IBT-2 pin 1 (RPWM)
double required_pwm = 180; // duty-cycle to PWM pump
double required_pressure = 45; // set the staining tank pressure
double current_pressure = 0; //mBar
double sum = 0.0;
int ctr = 50;
bool is_pump_active = false;

/********************************
 * ROS DEFINITIONS
 * ******************************/
#define ISROS
//#define ROSSTRING
#define ROSFLOAT
#ifdef ISROS
    #define TOPICNAME "chatter"
    #define BAUDRATE 57600
    ros::NodeHandle  nh;
    #ifdef ROSSTRING
        std_msgs::String topic_msg;
    #endif
    #ifdef ROSFLOAT
        std_msgs::Float32 topic_msg;
    #endif
    
    ros::Publisher chatter(TOPICNAME, &topic_msg);
#else
    #define BAUDRATE 115200
#endif
/***********************
 * setup
 * *********************/
void setup()
{
    pinMode(sensorPin, INPUT); // set sensor pin at input
    pinMode(pump, OUTPUT); // set pump pin at output
    Serial.begin(BAUDRATE);
    TCCR2B=(TCCR2B&0xF8) | 1;
    /*
     * ROS initialization
     */
#ifdef ISROS
    nh.initNode();
    nh.advertise(chatter);
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
    #ifdef ROSSTRING
        String value = String(current_pressure);
        topic_msg.data = value.c_str();
    #endif
    #ifdef ROSFLOAT
        topic_msg.data = current_pressure;
    #endif
    chatter.publish( &topic_msg );
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
