#include <Arduino.h>

/**
 * @brief 
 * Main configurations of the controller, 
 * inside the Setup() function
 */
// Declare pins as output:
void init_setup_manipulation()
{
    // joystick init
    pinMode(pin_joystick_X_up, INPUT); 
    pinMode(pin_joystick_X_down, INPUT);
    pinMode(pin_joystick_Y_up, INPUT);
    pinMode(pin_joystick_Y_down, INPUT);

    // joystick init
    pinMode(pin_automatic_switch, INPUT_PULLUP);

    // joystick init
    pinMode(enable_pin_z, OUTPUT);
    digitalWrite(enable_pin_z, HIGH);

    // Interrupt
    pinMode(pinStopX, INPUT_PULLUP); // pin di interrupt
    pinMode(pinStopY, INPUT_PULLUP); // pin di interrupt
    
    // Stepper init
    pinMode(pin_enable_x1,OUTPUT);
    pinMode(pin_enable_x2,OUTPUT);
    pinMode(pin_enable_y,OUTPUT); 
    //digitalWrite(pin_enable_x1,HIGH);
    //digitalWrite(pin_enable_x2,HIGH);
    //digitalWrite(pin_enable_y,HIGH); 
    
    // stepper limit init
    // Set the maximum steps per second:
    Xaxis.setMaxSpeed(1000);  //30000 //1300 200x step motor max speed 1200 24 V
    Yaxis.setMaxSpeed(1000); //400 step motor max speed 1600 24 V

    // Set the maximum acceleration in steps per second^2:
    Xaxis.setAcceleration(500); // 4000
    Yaxis.setAcceleration(500);

    Serial.begin(9600); // initialize serial communications at 9600 bps
    Serial1.begin(9600);
    Serial2.begin(9600);
}