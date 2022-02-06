/**
 * Header
 */

#include <Arduino.h>

/**
 * Variables definition
 */
int i = 0;
unsigned long time_now = 0;
unsigned long time_signal = 0;

/**
 * @brief 
 * Serial communication with the 3 DoF manipulator
 */
void serialInput()
{
    if (Serial1.available() > 0)
  {
    serial_data = Serial1.read();
    switch (serial_data)
    {
      case 'o':
        // ON/OFF stepper motors
        flag_on = !flag_on;
        if(flag_on == true)
        {
          digitalWrite(pin_enable_x1,LOW);
          digitalWrite(pin_enable_x2,LOW);
          digitalWrite(pin_enable_y,LOW);
          Serial.println("Steppers ON");
        }
        else
        {
          digitalWrite(pin_enable_x1,HIGH);
          digitalWrite(pin_enable_x2,HIGH);
          digitalWrite(pin_enable_y,HIGH);
          Serial.println("Steppers OFF");
        }
        break;
      
      case 'r':
        // Print current position 
        Serial.print("Current X position: ");
        Serial.println(Xaxis.currentPosition());
        Serial.print("Current Y position: ");
        Serial.println(Yaxis.currentPosition());
        flag_print = !flag_print;
        break;

      case '1':
        // MOVE 1
        digitalWrite(pin_enable_y,LOW);
        digitalWrite(pin_enable_x1,LOW);
        digitalWrite(pin_enable_x2,LOW);

        Yaxis.moveTo(2925);
        Yaxis.setSpeed(400);
        while(Yaxis.distanceToGo() != 0)
        {
          Yaxis.runSpeedToPosition();
        }
        digitalWrite(control_pin_z1, LOW);
        digitalWrite(control_pin_z2, HIGH);
        analogWrite(PWM_pin_z, motor_speed_z);
        delay(1950); // 2950
        analogWrite(PWM_pin_z, 0);
        delay(10);
        Xaxis.moveTo(-7855);
        Xaxis.setSpeed(400);
        while(Xaxis.distanceToGo() != 0)
        {
          Xaxis.runSpeedToPosition();
        }
        break;

      case '2':   
        // MOVE 2 
        Xaxis.moveTo(-6450);
        Xaxis.setSpeed(200);
        while(Xaxis.distanceToGo() != 0)
        {
          Xaxis.runSpeedToPosition();
        }
        Yaxis.moveTo(4553);
        Yaxis.setSpeed(200);
        while(Yaxis.distanceToGo() != 0)
        {
          Yaxis.runSpeedToPosition();
        }
        break;
    }
  }
}

/**
 * @brief 
 * Keyboard inputs
 */
void keyboardInput()
{
  if (Serial.available() > 0)
  {
    serial_data = Serial.read();
    switch (serial_data)
    {
    case 'y':
      // Move along Y-Axis
      reading = Serial.parseInt();
      Yaxis.move(reading);
      while (Yaxis.distanceToGo() != 0)
      {
        Yaxis.runToPosition();
      }
      break;

    case 'x':
      // Move along X-Axis
      reading = Serial.parseInt();
      Xaxis.move(-8000);
      while (Xaxis.distanceToGo() != 0)
      {
        Xaxis.runToPosition();
      }
      break;

    case 'o':
      // ON/OFF Stepper motors
      flag_on = !flag_on;
      if (flag_on == true)
      {
        digitalWrite(pin_enable_x1, LOW);
        digitalWrite(pin_enable_x2, LOW);
        digitalWrite(pin_enable_y, LOW);
        Serial.println("Steppers ON");
      }
      else
      {
        digitalWrite(pin_enable_x1, HIGH);
        digitalWrite(pin_enable_x2, HIGH);
        digitalWrite(pin_enable_y, HIGH);
        Serial.println("Steppers OFF");
      }
      break;

    case 'r':
      // Print current position
      Serial.print("Current X position: ");
      Serial.println(Xaxis.currentPosition());
      Serial.print("Current Y position: ");
      Serial.println(Yaxis.currentPosition());
      flag_print = !flag_print;
      break;

    case 'h':
    // Move the end effector to home position
      digitalWrite(control_pin_z1, HIGH);
      digitalWrite(control_pin_z2, LOW);
      analogWrite(PWM_pin_z, motor_speed_z);
      delay(300); // 1840
      analogWrite(PWM_pin_z, 0);

      digitalWrite(pin_enable_y, LOW);
      digitalWrite(pin_enable_x1, LOW);
      digitalWrite(pin_enable_x2, LOW);
      // Homing
      Serial.println("Returning to Home position...");
      Yaxis.moveTo(0);
      while (Yaxis.distanceToGo() != 0)
      {
        Yaxis.runToPosition();
      }
      delay(100);
      Xaxis.moveTo(0);
      while (Xaxis.distanceToGo() != 0)
      {
        Xaxis.runToPosition();
      }
      digitalWrite(control_pin_z1, HIGH);
      digitalWrite(control_pin_z2, LOW);
      analogWrite(PWM_pin_z, motor_speed_z);
      delay(5000); // 1840
      analogWrite(PWM_pin_z, 0);
      delay(10);
      break;

    case 's':
      // Manual control of Z-Axis from keyboard - UP
      if (flag_automatic == false)
      {
        Serial.print("Down");
        Serial.print("\n");
        digitalWrite(control_pin_z1, LOW);
        digitalWrite(control_pin_z2, HIGH);
        analogWrite(PWM_pin_z, motor_speed_z);
        delay(40); // 1840
        analogWrite(PWM_pin_z, 0);
      }
      break;

    case 'w':
      // Manual control of Z-Axis from keyboard -DOWN
      if (flag_automatic == false)
      {
        Serial.print("Up");
        Serial.print("\n");
        digitalWrite(control_pin_z1, HIGH);
        digitalWrite(control_pin_z2, LOW);
        analogWrite(PWM_pin_z, motor_speed_z);
        delay(40); // 1840
        analogWrite(PWM_pin_z, 0);
      }
      break;
    }
  }
}