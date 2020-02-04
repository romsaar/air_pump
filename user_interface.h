#include <LiquidCrystal.h>
// Menu places
#define MENU_REQ_PRESSURE       0
#define MENU_OPEN_ANGLE          1
#define MENU_CLOSE_ANGLE         2
#define MENU_SERVO_DELAY         3
#define MENU_HVLP_EN                4
#define MENU_FEEDER_EN             5
#define MAX_MENU_PLACES           6

// Buttons
#define NO_BUTTON                     0
#define RIGHT_BUTTON                 1
#define LEFT_BUTTON                   2
#define UP_BUTTON                      3
#define DOWN_BUTTON                4
#define SELECT_BUTTON               5

// Pins Definitions
#define LCD_RS                       24 //(Uno 8) 
#define LCD_E                         25 //(Uno 9)
#define LCD_D4                      26 //(Uno 4) 
#define LCD_D5                      27 //(Uno 5)
#define LCD_D6                      28 //(Uno 6)
#define LCD_D7                      29 //(Uno 7)

//pressure
#define MAX_REQ_PRESSURE             100
#define REQ_PRESSURE_STEP            1

/*******************************************************************************
* Declaration for GUI & menu
*******************************************************************************/
//LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // initialize the library with the numbers of the interface pins
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7); // initialize the library with the numbers of the interface pins
int menu_place_id = MENU_REQ_PRESSURE;
int current_button = NO_BUTTON;

bool is_pressed = false;            // remove this parameter !!!!!!!!!!!!!!!!!!!!!!

/*******************************************************************************
* Declaration for jig parameters
*******************************************************************************/
//bool is_pump_active = false;      // Feeder starts inactive
int req_velocity = 15;              // required feeder velocity [cm/sec]
//int open_angle = HVLP_OPEN_POS;     // HVLP active position
//int close_angle = HVLP_CLOSE_POS;   // HVLP idle position
//int servo_delay = 1000;             // wait 1000ms brfore activating the HVLP
bool is_pump_enabled = true;      // Feeder is enabled 
//bool is_hvlp_enabled = true;        // HVLP is enabled

// Function
void menu_handler_on(int velocity);
void print_idle_screen(void); 
void print_on_screen(void); 
void get_button(void);
void menu_handler(void);
void update_screen(int);
