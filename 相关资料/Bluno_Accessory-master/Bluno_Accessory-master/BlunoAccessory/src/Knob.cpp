/*
The value of this file is read a knob

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
#include <Knob.h>
/*********************************************************************
Return the value of Knob
value :0-1023
*********************************************************************/
int Knob::readKnob(void){
	int value = analogRead(knobPin);
#if defined	ARDUINO_AVR_UNO || defined ARDUINO_AVR_LEONARDO
    value = map(value, 0, 670, 0, 1023);
#endif
    if(value > 1023)
		value = 1023;
	return (1023 - value);
}
