/*
  BlunoSample

 This is a comprehensive example for bluetooth control sensor

 Circuit:
 * DFRPBOT Accessory Shield V1.0
 * DFROBOT Bluno V2.0
https://github.com/DFRobot/DFRobot_BlunoAccessory
 */
#ifdef ARDUINO_ARCH_ARC32
#include <BLEAttribute.h>
#include <BLECentral.h>
#include <BLECharacteristic.h>
#include <BLECommon.h>
#include <BLEDescriptor.h>
#include <BLEPeripheral.h>
#include <BLEService.h>
#include <BLETypedCharacteristic.h>
#include <BLETypedCharacteristics.h>
#include <BLEUuid.h>
#include "BLESerial.h"   
#endif
#include <Wire.h>
#include <PlainProtocol.h>
#include <string.h>
#include <BlunoAccessory.h>
#include <Arduino.h>
#define RIGHT 1
#define UP 2
#define LEFT 3
#define DOWN 4
#define PUSH 5
#define MID 0
#define DHTPIN 4
#define DHTTYPE DHT11

#ifdef ARDUINO_ARCH_ARC32
BLESerial mySerial;
PlainProtocol myBLUNO(mySerial,115200);
#elif ARDUINO_AVR_LEONARDO
PlainProtocol myBLUNO(Serial1,115200);
#else
PlainProtocol myBLUNO(Serial,115200);
#endif
//BlunoAccessory constructor,for setting the OLED
BlunoAccessory oled;

//Buzzer constructor,for setting the buzzer
Buzzer myBuzzer;

//Relay constructor,for setting the Relay
Relay myRelay;

//Joystick constructor,for setting the joyStick
Joystick myJoy;

//Knob constructor,for setting the knob
Knob myKnob;

//Rgb constructor,for setting the RGB
Rgb myRgb;

//DHT constructor,for setting the temperature, humidity
DHT dht(DHTPIN, DHTTYPE);

String oledDisplay = "";
int16_t Display = 0;
int16_t humidity = 0;
int16_t temperature = 0;

int16_t ledRed = 0;
int16_t ledBlue = 0;
int16_t ledGreen = 0;
int16_t knob = 0,prevknob = -3;
int16_t joyStick = 0, prevJoyStick = 4;

void draw1(void){          //Displays the character OLED is fixed
    oled.print(0, 0, "H:");
    oled.print(4, 0, "%");
    oled.print(0, 2, "T:");
    oled.print(4, 2, "C");
    oled.print(0, 4, "Knob:");
    oled.print(0, 6, "Joystick:");
    oled.print(9, 6, "Normal");
    oled.print(10, 0, "R:");
    oled.print(10, 2, "G:");
    oled.print(10, 4, "B:");
}
void setup() {
    oled.begin();
    dht.begin();
    oled.Reset();
    oled.SSD1306();
    oled.fill(0x00);
    draw1();
    Serial.begin(115200);    // initialize serial communication
    #ifdef ARDUINO_ARCH_ARC32
    mySerial.setName("Bluno101");
    mySerial.begin();
    #endif
}


void draw(void)           //Display OLED variable characters
{
    oled.print(2, 0, humidity);
    oled.print(2, 2, temperature); 
    oled.print(12, 0, ledRed);
    oled.print(12, 2, ledGreen);
    oled.print(12, 4, ledBlue);
}
bool check()
{
    bool ret = false;
    if(humidity != (int16_t)dht.readHumidity()){
        humidity = (int16_t)dht.readHumidity();
        ret = true;
    }
    if(temperature != (int16_t)dht.readTemperature()){
        temperature = (int16_t)dht.readTemperature();
        ret = true;
    }
    if(Display != (int16_t)oledDisplay.length()){
        Display = (int16_t)oledDisplay.length();
        ret = true;
    }
    return ret;
}
void loop() {
    if(check()){
        draw();
     }
     knob=myKnob.readKnob();
     delay(10);
    if(abs(knob - prevknob) > 2){
        oled.fill(5,4,0x00);
        oled.print(5, 4, knob); 
        prevknob = knob;
    }
    if(oledDisplay.length()){
      oled.fill(0,6,0x00);
      oled.print(0,6,oledDisplay);
    }else{
    joyStick = myJoy.readJoystick();
    if(joyStick != prevJoyStick){
      switch (joyStick){
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
        oled.print(9, 6, "Normal");
        break;
        default:
        break;   
      }
      prevJoyStick = joyStick;  
   }
 }
    if (myBLUNO.available()) {    //receive valid command    
        if (myBLUNO.equals("BUZZER")) {     //if the command name is "BUZZER"        
            myBuzzer.setBuzzer(myBLUNO.read());  //read the content and set the buzzer
        }else if (myBLUNO.equals("RELAY")){      //if the command name is "RELAY"
            myRelay.setRelay(myBLUNO.read());   //read the content and set the relay
        }else if (myBLUNO.equals("DISP")){       //if the command name is "DISP"
            oledDisplay = myBLUNO.readString();   //read the string to local value
        }else if(myBLUNO.equals("RGBLED")){      //if the command name is "RGBLED"
            ledRed = myBLUNO.read();              //read the red value first
            oled.fill(12, 0, 0x00);
            oled.print(12, 0, ledRed);
            ledGreen = myBLUNO.read();            //read the green value then
            oled.fill(12, 2, 0x00);
            oled.print(12, 2, ledGreen);
            ledBlue = myBLUNO.read();             //read the blue value at last
            oled.fill(12, 4, 0x00);
            oled.print(12, 4, ledBlue);   
            myRgb.setRGBLed(ledRed, ledGreen, ledBlue);   //set the color to the RGBLED
        }else if(myBLUNO.equals("TEMP")){    //if the command name is "TEMP"
            myBLUNO.write("TEMP", temperature);     //return the value of temperature to mobile device
        }else if(myBLUNO.equals("HUMID")){   //if the command name is "HUMID"
            myBLUNO.write("HUMID", humidity);   //return the value of humidity to mobile device
        }else if(myBLUNO.equals("KNOB")){    //if the command name is "KNOB"
            myBLUNO.write("KNOB", myKnob.readKnob());  //return the value of the knob(potentiometer) to mobile device
        }
    }

    if (myJoy.joystickAvailable()) {          //if the state of joystick is changed
        myBLUNO.write("ROCKER", joyStick);    //send the command and value to mobile device
        oledDisplay = "";                     //clear the oled display string to show the new state of joystick on oled
    }
}