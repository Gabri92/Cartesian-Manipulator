/**
 * Header
 */
#include <AccelStepper.h>

/**
 * Variables definition
 */
double positioning_error_threshold = 1.5;

int pin_automatic_switch = 53;

// Enable pin
#define pin_enable_x1 40
#define pin_enable_x2 44
#define pin_enable_y 42

/////////////////////////////////
// Initialization
bool flag_initialization_X = true;
bool flag_initialization_Y = true;
/////////////////////////////////

/////////////////////////////////
// Interrupt pin
#define pinStopX 50
#define pinStopY 51
/////////////////////////////////

// Timer variables
long timer = 0;
long time_X_catch = 0;
long time_Y_catch = 0;
long time_print = 0;
long timer_actuator = 150;
long timer_actuate_z_axis = 150;

/////////////////////////////////
// Pin variables z axis actuator
#define enable_pin_z 10
#define control_pin_z1 11
#define control_pin_z2 12
#define PWM_pin_z 13
int motor_speed_z = 120;
bool flag_tool = true;

/////////////////////////////////
// Motor control variables
#define stepPinX 2
#define dirPinX 3
#define stepPinY 4
#define dirPinY 5

int X_axis_speed = 2000;
int Y_axis_speed = 2000;

int reading = 0;

int pos_x1 = 1636;
int pos_y1 = 0;
int pos_x2 = 0;
int pos_y2 = 0;
int pos_x3 = 0;
int pos_y3 = 0;

// Pin Joystick
int pin_joystick_X_up = 22; // pin controllo joystick
int pin_joystick_X_down = 26;
int pin_joystick_Y_up = 24;
int pin_joystick_Y_down = 28;

// stepper variables
/////////////////////////////////

AccelStepper Xaxis(AccelStepper::DRIVER, stepPinX, dirPinX);
AccelStepper Yaxis(AccelStepper::DRIVER, stepPinY, dirPinY);

/////////////////////////////////
// FLAGS
bool flag_on = false;
bool flag_automatic = false;
bool flag_pos_Y = false;
bool flag_pos_X = false;
bool flag_catch_X = false;
bool flag_catch_Y = false;
bool flag_print = false;
bool flag_interrupt = false;
bool flag_protection = false;

bool flag_w = false;
bool flag_s = false;
bool flag_a = false;
bool flag_d = false;

/**
 * Functions declaration
 */
void automaticControl(float, float);
void catchSlice();
void moveDownTool();
void moveUpTool();
void homing();
