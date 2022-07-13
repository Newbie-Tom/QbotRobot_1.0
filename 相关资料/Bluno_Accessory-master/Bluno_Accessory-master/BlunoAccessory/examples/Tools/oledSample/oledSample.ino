/*
  OledSample

This is an example of OLED display

 Circuit:
 * DFRPBOT Accessory Shield V1.0
 * DFROBOT Bluno V2.0

 */
#include <BlunoAccessory.h>
#include <Wire.h>
BlunoAccessory oled;
void setup() {
   oled.begin();
   oled.Reset();
   oled.SSD1306();
   oled.fill(0x00);
   Serial.begin(115200);
}

void loop() {
    oled.print(3, 3, "Hello World!");
}
