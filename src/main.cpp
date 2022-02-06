/**
 * @file main.cpp
 * @author Dario Lunni (dario.lunni@gmail.com), Gabriele Baldi (gabriele.baldi.92@gmail.com)
 * @brief 
 * 
 * Control of a cartesian manipulator which is able to communicate 
 * with a raspberry Pi and a camera, providing a vision system, and with 
 * a 3 DoF manipulator for synchronous operations
 * 
 */

/**
 * Header
 */
#include <Arduino.h>
#include <Communication.cpp>
#include <motor_control.cpp>
#include <init_setup_manipulation.cpp>
#include <input.cpp>

/**
 * Main Functions
 */
/**
 * @brief 
 * Initialize the manipulator and enable interrupts attached
 * to endstrokes' safety switches
 */
void setup()
{
  init_setup_manipulation();
  endStrokeInit();
  attachInterrupt(digitalPinToInterrupt(pinStopX), stopX, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinStopY), stopY, FALLING);
}

/**
 * @brief 
 * Controller loop
 */
void loop()
{
  if(flag_protection == true)
  {
    attachInterrupt(digitalPinToInterrupt(pinStopX), stopX, FALLING);
    attachInterrupt(digitalPinToInterrupt(pinStopY), stopY, FALLING);
    flag_protection = false;
  }

  ////////////////////////////////////////////////////////////////////
  // This part of code activate the communication with the manipulator
  //while(Serial1.available() == 0){}
  //serialInput();
  ////////////////////////////////////////////////////////////////////

  // Input Keyboard
  keyboardInput();

  timer = millis();

  // Activate or deactivate the communication with Raspberry Pi
  // and automatic movement assisted by the vision system
  flag_automatic = digitalRead(pin_automatic_switch);
  cameraServoing();

  if (flag_automatic == true && flag_interrupt == false)
  {
    automaticControl(pos_X, pos_Y);
  }
  if (flag_automatic == false)
  {
    if(flag_interrupt == true)
    {
      flag_interrupt = false;
    }
    manualControl();
  }
}
