/*
  BuzzerSample

 This is an example of control buzzer

 Circuit:
 * DFRPBOT Accessory Shield V1.0
 * DFROBOT Bluno V2.0
 https://github.com/DFRobot/DFRobot_BlunoAccessory
 */
#include <Buzzer.h>
#include <Wire.h>
Buzzer myBuzzer;
void setup() {
  
}

void loop() {
  myBuzzer.setBuzzer(true);
  delay(1000);
  myBuzzer.setBuzzer(false);
  delay(1000);

}
