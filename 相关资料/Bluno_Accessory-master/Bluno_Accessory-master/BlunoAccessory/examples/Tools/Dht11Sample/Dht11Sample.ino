/*
  Dht11Sample

This is an example of temperature and humidity sensor

 Circuit:
 * DFRPBOT Accessory Shield V1.0
 * DFROBOT Bluno V2.0

 */
// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "BlunoAccessory.h"
#include <Wire.h>
#define DHTPIN 2     // what digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
BlunoAccessory oled;
DHT dht(DHTPIN, DHTTYPE);
int h = dht.readHumidity();
// Read temperature as Celsius (the default)
int t = dht.readTemperature();
// Read temperature as Fahrenheit (isFahrenheit = true)
void setup() {
  oled.begin();
  Serial.begin(115200);
  dht.begin();
  oled.Reset();
  oled.SSD1306();
  oled.fill(0x00);
}

void loop() {
  oled.print(0, 0, "H:");
  oled.print(2, 0, (int)dht.readHumidity());
  oled.print(4, 0, "%");
  oled.print(0, 2, "T:");
  oled.print(2, 2, (int)dht.readTemperature());
  oled.print(4, 2, "C");
}
