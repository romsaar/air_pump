#include "user_interface.h"
/*******************************************************************************
* Menu handler
* Buttons:
*           Right - scrolls right the menu
*           Left - scrolls left the menu
*           Up - increment current menu place
*           Down - decrement current menu place
*           Select - transiotion to On mode           
* Menu places: 
*           0=required velocity
*           1=Open angle
*           2=Close angle
*           3=servo_delay
*           4=HVLP enable
*           5=Feeder enable
*******************************************************************************/

void menu_handler()
{
  // poll the buttons
  get_button();

  if (current_button != NO_BUTTON)
  {
    // handle pressed button
    lcd.setCursor(0,1);
    lcd.print("               ");

    switch (current_button) {
      case RIGHT_BUTTON:
        // handle right button - update the menu place       
        menu_place_id = (menu_place_id+1) % MAX_MENU_PLACES;
        //lcd.print ("Right ");
        break;
      case LEFT_BUTTON:
        // Left button - update the menu place
        menu_place_id = (menu_place_id-1) % MAX_MENU_PLACES;
        if (menu_place_id<0)
          menu_place_id += MAX_MENU_PLACES;
        //lcd.print (menu_place_id);
        break;
      case UP_BUTTON:
        // Up button - increase/toggle the relevant menu place value
        switch (menu_place_id) {
          case MENU_REQ_PRESSURE:
            required_pressure = min(required_pressure+REQ_PRESSURE_STEP, MAX_REQ_PRESSURE);
            break;
         /* case MENU_OPEN_ANGLE:
            open_angle = min(open_angle+HVLP_ANGLE_STEP, HVLP_MAX);
            break;
          case MENU_CLOSE_ANGLE:
            close_angle = min(close_angle+HVLP_ANGLE_STEP, HVLP_MAX);
            break;
          case MENU_SERVO_DELAY:
            servo_delay = min(servo_delay+SERVO_DELAY_STEP,MAX_SERVO_DELAY);
            break;
          case MENU_HVLP_EN:
           is_hvlp_enabled = not(is_hvlp_enabled);
            break;
          case MENU_FEEDER_EN:
            is_feeder_enabled = not(is_feeder_enabled);
            break; */        
          default:
            // statements
            break;
        }        
        break;
      case DOWN_BUTTON:
        // Down button - decrease/toggle the relevant menu place value  
        switch (menu_place_id) {
          case MENU_REQ_PRESSURE:
            required_pressure = max(required_pressure-REQ_PRESSURE_STEP, 0);
            break;
       /*  case MENU_OPEN_ANGLE:
            open_angle = max(open_angle-HVLP_ANGLE_STEP, HVLP_MIN);
            break;
          case MENU_CLOSE_ANGLE:
            close_angle = max(close_angle-HVLP_ANGLE_STEP, HVLP_MIN);
            break;
          case MENU_SERVO_DELAY:
            servo_delay = max(servo_delay-SERVO_DELAY_STEP,0);
            break;
          case MENU_HVLP_EN:
           is_hvlp_enabled = not(is_hvlp_enabled);
            break;
          case MENU_FEEDER_EN:
            is_feeder_enabled = not(is_feeder_enabled);
            break;*/       
          default:
            // statements
            break;
        }   
        break;
      case SELECT_BUTTON:
        // Select button - toggle feeder mode        
        is_pump_active = not(is_pump_active);             
        break;          
      default:
        // statements
        break;
    }

    // reset current button
    current_button = NO_BUTTON;    
         
  }
}

/*******************************************************************************
* Loop function
*******************************************************************************/

void update_screen(float pressure)
{/*
    // upper row
    lcd.setCursor(0,0);
    if (is_pump_active)
    {
      // Active pump display
      lcd.print ("Active ");      
      lcd.print (current_pressure);
      lcd.print ("mBar");
    }
    else
    {
      // Inactive pump display
      lcd.print ("Inactive        ");
    }
    */
    // lower row
    lcd.setCursor(0,1);
    switch (menu_place_id) {
        case MENU_REQ_PRESSURE:
          lcd.print ("Prs=");
          lcd.print (required_pressure);
          lcd.print ("mBar");
          break;
       /* case MENU_OPEN_ANGLE:
          lcd.print ("Open = ");
          lcd.print (open_angle);
          lcd.print ("deg");
          break;
        case MENU_CLOSE_ANGLE:
          lcd.print ("Close = ");
          lcd.print (close_angle);
          lcd.print ("deg");
          break;
        case MENU_SERVO_DELAY:
          lcd.print ("Delay = ");
          lcd.print (servo_delay);
          lcd.print ("ms");
          break;
        case MENU_HVLP_EN:
          if (is_hvlp_enabled)
            lcd.print("HVLP enabled");
           else
            lcd.print("HVLP disabled");
          break;
        case MENU_FEEDER_EN:
          if (is_feeder_enabled)
            lcd.print("Feeder enabled");
           else
            lcd.print("Feeder disabled");
          break;*/       
        default:
          // statements
          break;
      }  
}


/*******************************************************************************
* Get button
*   Indicates if a new button was pressed (right after the button was released)
*   The using function is expected to set current_button to NO_BUTTON
*******************************************************************************/
void get_button() 
{
  
  static bool is_pressed_internal = false;
  static int last_button = NO_BUTTON;
  int x;

  // sample button analog pin
  x = analogRead (0); 

  if (is_pressed_internal)
  {
    // Wait for button release
    if (x>=800)
    {
      // button release - update current button
      current_button = last_button;
      last_button = NO_BUTTON;
      is_pressed_internal = false;     
    }
  }
  else // check if new button is pressed
  {     
    if (x<100)
      // Right button
      last_button = RIGHT_BUTTON;
    else if (x < 200)
      // Up button 
      last_button = UP_BUTTON;
    else if (x<400)
      // Down button
      last_button = DOWN_BUTTON;
    else if (x<600)
      // Left button
      last_button = LEFT_BUTTON;
    else if (x<800)
      // Select button
      last_button = SELECT_BUTTON;

     // indicate a button is pressed
     if (last_button > 0)
      is_pressed_internal = true;
  }
}
