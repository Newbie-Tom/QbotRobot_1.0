/*
  JoySample

This is an example of a joystick

 Circuit:
 * DFRPBOT Accessory Shield V1.0
 * DFROBOT Bluno V2.0

 */
#include "BlunoAccessory.h"
#include <Wire.h>
#define RIGHT 1
#define UP 2
#define LEFT 3
#define DOWN 4
#define PUSH 5
#define MID 0

BlunoAccessory oled;
Joystick myJoy;
void setup() { 
  oled.begin();            
  Serial.begin(115200);   
  Serial.println("blunoAccessory Joystick test!"); 
  oled.Reset();
  oled.SSD1306();
  oled.fill(0x00);  
}

// the loop routine runs over and over again forever:
void loop() {

  //if the joystick is changed, joystickAvailable() will return true.
  oled.print(0, 6, "Joystick:");
  if (myJoy.joystickAvailable()){  
    switch (myJoy.readJoystick()){
      case RIGHT:
      oled.fill(9, 6, 0x00);
      oled.print(9, 6, "RIGHT");
      break;
      case LEFT:
      oled.fill(9, 6, 0x00);
      oled.print(9, 6, "LEFT");
      break;
      case UP:
      oled.fill(9, 6, 0x00);
      oled.print(9, 6, "UP");
      break;
      case DOWN:
      oled.fill(9, 6, 0x00);
      oled.print(9, 6, "DOWN");
      break;
      case PUSH:
      oled.fill(9, 6, 0x00);
      oled.print(9, 6, "PUSH");
      break;
      case MID:
      oled.fill(9, 6, 0x00);
      oled.print(9, 6, "MID");
      break;
      default:
      break;    
    }
  }
}
