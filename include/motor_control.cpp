/**
 * Header
 */
#include "motor_control.h"

/**
 * Main Functions
 */
/**
 * @brief 
 * Endstroke initialization of the manipulator
 */
void endStrokeInit()
{
    // // move up tool at maximum
    digitalWrite(control_pin_z1, HIGH);
    digitalWrite(control_pin_z2, LOW);
    analogWrite(PWM_pin_z, motor_speed_z);
    delay(5000);
    analogWrite(PWM_pin_z, 0);
    delay(1000);
    digitalWrite(control_pin_z1, LOW);
    digitalWrite(control_pin_z2, HIGH);
    analogWrite(PWM_pin_z, motor_speed_z);
    delay(1700);
    analogWrite(PWM_pin_z, 0);
    delay(100);
    // Enable steppers on the X, Y directions
    digitalWrite(pin_enable_x1,LOW);
    digitalWrite(pin_enable_x2,LOW);
    digitalWrite(pin_enable_y,LOW);
    // Initialize X-Axis
    while (flag_initialization_X == true)
    {
        
        Xaxis.move(5000);
        if (digitalRead(pinStopX) == LOW)
        {
            Serial.println("Initialized X ...");
            Xaxis.setSpeed(0);
            Xaxis.runSpeed();
            flag_initialization_X = false;
        }
        Xaxis.run();
    }
    // Initialize Y-Axis
    while (flag_initialization_Y == true)
    {
        Yaxis.setSpeed(-5000);
        if (digitalRead(pinStopY) == LOW)
        {
            Serial.println("Initialized Y ...");
            Yaxis.setSpeed(0);
            Yaxis.runSpeed();
            flag_initialization_Y = false;
        }
        Yaxis.run();
    }
    // Send the manipulatore to Home position
    Xaxis.move(6000);
    Yaxis.move(1900);
    Xaxis.setSpeed(-1000);
    Yaxis.setSpeed(1000);
     while(Yaxis.distanceToGo() != 0 && Xaxis.distanceToGo() != 0)
    {
        Yaxis.runSpeed();
        Xaxis.runSpeed();
    }

    Xaxis.setCurrentPosition(0);
    Yaxis.setCurrentPosition(0);
    // Initialize Z-Axis
    digitalWrite(control_pin_z1, HIGH);
    digitalWrite(control_pin_z2, LOW);
    analogWrite(PWM_pin_z, motor_speed_z);
    delay(5000);
    analogWrite(PWM_pin_z, 0);
    //flag_protection = true;


    //Serial1.print('h');
}

/**
 * @brief 
 * Manual control function
 */
void manualControl()
{   
    flag_interrupt = false;
    flag_tool = true;
    /*if (Serial.available() > 0)
    {
        char serial_data_manual_control = Serial.read();
        switch (serial_data_manual_control)
        {
        case 's':
            Serial.print("Down");
            Serial.print("\n");
            digitalWrite(control_pin_z1, HIGH);
            digitalWrite(control_pin_z2, LOW);
            analogWrite(PWM_pin_z, motor_speed_z);
            delay(timer_actuate_z_axis); // 1840
            analogWrite(PWM_pin_z, 0);
            break;

        case 'w':
            Serial.print("UP");
            Serial.print("\n");
            digitalWrite(control_pin_z1, LOW);
            digitalWrite(control_pin_z2, HIGH);
            analogWrite(PWM_pin_z, motor_speed_z);
            delay(timer_actuate_z_axis); // 1840
            analogWrite(PWM_pin_z, 0);
            break;

        default:
            // if nothing else matches, do the default
            // default is optional
            break;
        }
    }*/

    flag_w = digitalRead(pin_joystick_X_up);
    flag_s = digitalRead(pin_joystick_X_down);
    flag_d = digitalRead(pin_joystick_Y_up);
    flag_a = digitalRead(pin_joystick_Y_down);

    if (flag_w == true)
    {
        // Serial.print("DEBUG W \n");
        Xaxis.setSpeed(X_axis_speed);
        Xaxis.runSpeed();
    }
    if (flag_s == true)
    {
        // Serial.print("DEBUG S \n");
        Xaxis.setSpeed(-X_axis_speed);
        Xaxis.runSpeed();
    }
    if (flag_d == true)
    {
        // Serial.print("DEBUG D \n");
        Yaxis.setSpeed(Y_axis_speed);
        Yaxis.runSpeed();
    }
    if (flag_a == true)
    {
        // Serial.print("DEBUG A \n");
        Yaxis.setSpeed(-Y_axis_speed);
        Yaxis.runSpeed();
    }
}

/**
 * @brief 
 * Automatic control function
 */
void automaticControl(float pos_X, float pos_Y)
{

    if (pos_X > positioning_error_threshold)
    {
        Xaxis.setSpeed(X_axis_speed);
        Xaxis.runSpeed(); /* code */
        flag_pos_X = false;
    }

    if (pos_X < -positioning_error_threshold)
    {
        Xaxis.setSpeed(-X_axis_speed);
        Xaxis.runSpeed(); /* code */
        flag_pos_X = false;
    }

    if (pos_X > -positioning_error_threshold && pos_X < positioning_error_threshold)
    {
        Xaxis.stop(); /* code */
        if (flag_pos_X == false)
        {
            time_X_catch = millis();
            flag_pos_X = true;
        }
        if (timer - time_X_catch > 2000)
        {
            flag_catch_X = true;
        }
    }

    if (pos_Y > positioning_error_threshold)
    {
        Yaxis.setSpeed(-Y_axis_speed);
        Yaxis.runSpeed(); // code 
        flag_pos_Y = false;
    }

    if (pos_Y < -positioning_error_threshold)
    {
        Yaxis.setSpeed(Y_axis_speed);
        Yaxis.runSpeed(); // code 
        flag_pos_Y = false;
    }

    if (pos_Y > -positioning_error_threshold && pos_Y < positioning_error_threshold)
    {
        Yaxis.stop(); // code 
        if (flag_pos_Y == false)
        {
            time_Y_catch = millis();
            flag_pos_Y = true;
        }
        if (timer - time_Y_catch > 2000)
        {
            flag_catch_Y = true;
        }
    }
    flag_catch_Y = true;
    if (flag_catch_Y == true && flag_catch_X == true)
    {
        catchSlice();
    }
}

void moveToRecollectingTool()
{
    //Xaxis.setAcceleration(3000);
    Xaxis.moveTo(-30000);
    while(Xaxis.distanceToGo() != 0)
    {
        Xaxis.runToPosition();
    }    
    delay(10);
}

///////////////////////////////////////////////////////////////
/**
 * @brief 
 * Set of movements performed to collect the slice
 */
void moveDownTool()
{
    // // move down
    digitalWrite(control_pin_z1, LOW);
    digitalWrite(control_pin_z2, HIGH);
    analogWrite(PWM_pin_z, motor_speed_z);
    delay(2150);
    analogWrite(PWM_pin_z, 0);
    delay(10);
}

void moveUpTool()
{
    // // move up
    digitalWrite(control_pin_z1, HIGH);
    digitalWrite(control_pin_z2, LOW);
    analogWrite(PWM_pin_z, motor_speed_z);
    delay(5000);
    analogWrite(PWM_pin_z, 0);
    delay(10);
}

void catchSlice()
{
    if (flag_tool == true)
    {

        moveDownTool();

        moveToRecollectingTool();
        

        if (Xaxis.distanceToGo() == 0)
        {
            moveUpTool();
            delay(100);
            homing();
        }

        // Reset all the flags!
        flag_tool = false;
        flag_catch_Y = false;
        flag_catch_X = false;
        flag_pos_Y = false;
        flag_pos_X = false;
    }
}
///////////////////////////////////////////////////////////////

/**
 * @brief 
 * Stop X-Axis motor
 */
void stopX()
{  
    Xaxis.stop();
    flag_print = true;
    flag_interrupt = true;
}

/**
 * @brief 
 * Stop Y-Axis motor
 */
void stopY()
{
  Yaxis.stop();
  flag_print = true;
  flag_interrupt = true;
}

/**
 * @brief 
 * Send manipulator in Home position
 */
void homing()
{
    Xaxis.moveTo(0);
    Yaxis.moveTo(0);
    while(Yaxis.distanceToGo() != 0)
    {
        Yaxis.runToPosition();
    }
    while(Xaxis.distanceToGo() != 0)
    {
        Xaxis.runToPosition();
    }
}
