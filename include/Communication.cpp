/**
 * @file Communication.cpp
 * 
 * @brief 
 * 
 * Serial Communication with Raspberry Pi, which 
 * send coordinates for the cartesian movement
 * 
 */

#include <Arduino.h>
////////////////////////////////////
// Communication Variables
String readString;
char serial_data = 0;

const byte num_chars = 32;

char received_chars[num_chars]; // received message
char temp_chars[num_chars];     // temporary array for use when parsing

bool new_data = false; // variables to hold the parsed data
char message_from_PC[num_chars] = {0};

float pos_X = 10;
float pos_Y = 10;
////////////////////////////////////

void parseData()
{ // split the data into its parts

  char *strtok_indx; // this is used by strtok() as an index

  strtok_indx = strtok(temp_chars, ",");
  pos_X = atof(strtok_indx); // convert this part to a float

  strtok_indx = strtok(NULL, ",");
  pos_Y = atof(strtok_indx); // convert this part to a float
}

void sendData()
{
  Serial.print("Arduino received: \t");
  Serial.print(readString); //see what was received
  readString = ""; // reinitialize string variable
}

void recvWithStartEndMarkers()
{
  static boolean recv_in_progress = false;
  static byte ndx = 0;
  char start_marker = '<';
  char end_marker = '>';
  char rc;

  // serial read section
  while (Serial3.available())
  {
    rc = Serial3.read();

    if (recv_in_progress == true)
    {
      if (rc != end_marker)
      {
        received_chars[ndx] = rc;
        ndx++;
        if (ndx >= num_chars)
        {
          ndx = num_chars - 1;
        }
      }
      else
      {
        received_chars[ndx] = '\0'; // terminate the string
        readString = received_chars;
        recv_in_progress = false;
        ndx = 0;
        new_data = true;
      }
    }

    else if (rc == start_marker)
    {
      recv_in_progress = true;
    }
  }
}

void cameraServoing()
{
  recvWithStartEndMarkers();
  if (readString.length() > 0)
  {
    strcpy(temp_chars, received_chars);
    // this temporary copy is necessary to protect the original data
    //   because strtok() used in parseData() replaces the commas with \0
    parseData();
    new_data = false;
    sendData();
  }

  delay(1);
  Serial3.flush();
}