/*
This file is to realize the normal of OLED display and import the library file of each sensor

September-December 2016

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

The latest version of this library can always be found at
https://github.com/DFRobot/DFRobot_BlunoAccess
*/
#ifndef __BLUNOACCESSORY_H
#define __BLUNOACCESSORY_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h> 
#include <Dht11.h>
#include <Joystick.h>
#include <Knob.h>
#include <Buzzer.h>
#include <Relay.h>
#include <rgb.h>
#include <Wire.h>
#include <WordStock.h>

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif



//class HardwareSerial;
class BlunoAccessory
{
public:
	BlunoAccessory():
	x(0),y(0)
	{	}
	
	~BlunoAccessory(){}
	void begin();
	void show(uint8_t x, uint8_t y, uint8_t DATA);
	void fill(unsigned char dat);
	void fill(uint8_t x,uint8_t y,unsigned char dat);
	void setCursor(uint8_t x, uint8_t y);
	void SSD1306();
	void Reset();
	void print(uint8_t x, uint8_t y,float f);
	void print(uint8_t x, uint8_t y,int n);
	void print(uint8_t x, uint8_t y,const char *str);
	void print(uint8_t x, uint8_t y,String str);
public:
	uint8_t x,y;
};

extern class BlunoAccessory blunoAccessory;

 
#endif