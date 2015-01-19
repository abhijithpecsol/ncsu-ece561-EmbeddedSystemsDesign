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
	unsigned int fadeFrequency = 0;
	unsigned int subTimer = 0;
	unsigned int lastTime = 0;
	unsigned int fadeBrightness = 0;
	
	// determine fade frequency
	if (brightness < 1000){
		fadeFrequency = 1000 / brightness;		
		if (1000 % brightness != 0){
			fadeFrequency++;										// round up to nearest whole int
		}
	}
	else {
		fadeFrequency = 1;
	}
	
	// over a period of 1 second, fade to brightness every fadeFrequency ms
	subTimer = 0;
	lastTime = timer1ms;
	while (subTimer <= 1000){
		if ((timer1ms != lastTime)) {
			subTimer++;
			lastTime = timer1ms;
			if (subTimer % fadeFrequency == 0){
				fadeBrightness++;
				setLEDColor(fadeBrightness, fadeBrightness, fadeBrightness);
			}
		}
	}
	
	// make sure we are at the right brightness at 1 sec (we will be very close, if not exact)
	if (fadeBrightness < brightness) {
		setLEDColor(brightness, brightness, brightness);
	}
}

// Over a period of 1 second, fade out from the specified brightness
void fadeOutWhite(){
	unsigned int fadeFrequency = 0;
	unsigned int subTimer = 0;
	unsigned int lastTime = 0;
	unsigned int fadeBrightness = TPM2_C0V;
	
	// determine fade frequency
	if (fadeBrightness < 1000){
		fadeFrequency = 1000 / fadeBrightness;		
		if (1000 % fadeBrightness != 0){
			fadeFrequency++;										// round up to nearest whole int
		}
	}
	else {
		fadeFrequency = 1;
	}
	
	// over a period of 1 second, fade to brightness every fadeFrequency ms
	subTimer = 0;
	lastTime = timer1ms;
	while (subTimer <= 1000){
		if ((timer1ms != lastTime)) {
			subTimer++;
			lastTime = timer1ms;
			if (subTimer % fadeFrequency == 0){
				fadeBrightness--;
				SET_LED_RED(fadeBrightness);
				SET_LED_GREEN(fadeBrightness);
				SET_LED_BLUE(fadeBrightness);
			}
		}
	}
	
	// make sure we are at the right brightness at 1 sec (we will be very close, if not exact)
	if (fadeBrightness > 0) {
		SET_LED_RED(0);
		SET_LED_GREEN(0);
		SET_LED_BLUE(0);
	}
}
