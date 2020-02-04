#include "PID.h"

void PID_pressure_control(float required_pressure, float current_pressure, float P, float D, float I)
{

  unsigned long t=millis();
  static int prev_pwm = 0;
  static float prev_pressure = 0;
  static float prev_diff = 0;  
  static unsigned long prev_time=0;
  static int required_pwm=0;
  int delta_pwm = 0;
  float vel_diff;  
  float differntial = 0;
  float delta_P_pwm, delta_D_pwm, delta_I_pwm;  
   
  // P part
  delta_P_pwm = float(required_pressure - current_pressure) * P;

  // D part
  delta_D_pwm = -float(current_pressure - prev_pressure)*1000*D/float(t-prev_time);

  // I part
  delta_I_pwm = 0.0;

  // Calculate new PWM
  delta_pwm = int(delta_P_pwm + delta_D_pwm);

  // Hard limit the max pwm change
  if (delta_pwm > MAX_PWM_CHANGE)
    delta_pwm = MAX_PWM_CHANGE;
  else if (delta_pwm < -MAX_PWM_CHANGE)
    delta_pwm = -MAX_PWM_CHANGE;

  // update the pwm
  required_pwm += delta_pwm;
         
  // Limit the actual pwm to be applied
  if (required_pwm>MAX_PWM)
    required_pwm = MAX_PWM;
  else if (required_pwm<MIN_PWM)
    required_pwm=MIN_PWM;

     
  // Update the PWM
  #ifdef TEST_PRESSURE_
    analogWrite(3,TEST_PWM);
  #else    
    analogWrite(3,required_pwm); // try to use HPWM!!!!!!!!!
  #endif


    // Update parameters
  prev_time = t;
  prev_pressure = current_pressure; 

  // Debug strings
  #ifdef MOTION_CONTROL_DEBUG__
   if (debug_enable)
  {
    Serial.print("Right Vel: ");
    Serial.print(current_pressure);
    Serial.print(" PWM: ");
    Serial.println(required_pwm);
  }
  #endif

}
