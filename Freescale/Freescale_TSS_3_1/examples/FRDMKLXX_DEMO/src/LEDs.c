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

void setLEDColor(unsigned int red, unsigned int green, unsigned int blue){
	SET_LED_RED(red);
	SET_LED_GREEN(green);
	SET_LED_BLUE(blue);
}
