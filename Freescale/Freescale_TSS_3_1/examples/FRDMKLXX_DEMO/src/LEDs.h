/*********************************************************************
*
* Michael Meli
* ECE 561 - Spring 2015
*
* Header file for LEDs.c
*
*********************************************************************/

void setLEDColor(unsigned int red, unsigned int green, unsigned int blue);
void fadeWhite(unsigned int brightness);
void fadeOutWhite(void);
void flashRedLED(unsigned int periodMS);
