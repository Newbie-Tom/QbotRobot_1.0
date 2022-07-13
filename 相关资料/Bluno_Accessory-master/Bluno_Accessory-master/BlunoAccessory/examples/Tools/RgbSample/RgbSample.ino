/*
  RgbSample

This is a control sample of full color led
 Circuit:
 * DFRPBOT Accessory Shield V1.0
 * DFROBOT Bluno V2.0

 */
#include <rgb.h>
Rgb myRgb;
void setup() {
  Serial.begin(115200);             
}

void loop(){
   for (int i=0;i<255;i++)
   {
     myRgb.setRGBLed(i, 0, 255-i);
     delay (10);
   }
   for (int i=0;i<255;i++)
   {
     myRgb.setRGBLed(255-i, i, 0);
     delay (10);
   }
   for (int i=0;i<255;i++)
   {
     myRgb.setRGBLed(0, 255-i, i);
     delay (10);
   }
}
