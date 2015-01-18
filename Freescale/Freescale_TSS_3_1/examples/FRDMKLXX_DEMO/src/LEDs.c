/*********************************************************************
*
* Michael Meli
* ECE 561 - Spring 2015
*
* This file provides helper functions for controlling the LEDs.
*
*********************************************************************/

#include <MKL25Z4.h>
#include "board.h"

extern volatile unsigned int timer1ms;

// Set the colors of the LEDs to whatever
void setLEDColor(unsigned int red, unsigned int green, unsigned int blue){
	SET_LED_RED(red);
	SET_LED_GREEN(green);
	SET_LED_BLUE(blue);
}

// Over a period of 1 second, fade to the specified brightness
void fadeWhite(unsigned int brightness){
	unsigned int fadeFrequency;
	
	// determine fade frequency
	if (brightness < 1000){
		fadeFrequency = 1000 / brightness;
	}
}
