/*
  RelaySample

This is a sample control relay

 Circuit:
 * DFRPBOT Accessory Shield V1.0
 * DFROBOT Bluno V2.0

 */
#include <Relay.h>
Relay myRelay;
void setup() {

}

void loop() {
  myRelay.setRelay(true);
  delay(2000);
  myRelay.setRelay(false);
  delay(2000);

}
