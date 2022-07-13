/*
  KnobSample

This is a sample of knob
 Circuit:
 * DFRPBOT Accessory Shield V1.0
 * DFROBOT Bluno V2.0

 */
#include "BlunoAccessory.h"
#include <Wire.h>
BlunoAccessory oled;
Knob myKnob;
int knob=0;
void setup() {   
  oled.begin();
  oled.Reset();
  oled.SSD1306();
  oled.fill(0x00);          
  Serial.begin(115200);   
}

// the loop routine runs over and over again forever:
void loop() {
   oled.print(0, 4, "Knob:");
   
   if(knob!=myKnob.readKnob())
   {
    knob=myKnob.readKnob();
    oled.fill(5, 4, 0x00);
    oled.print(5, 4, knob); 
   }
}
