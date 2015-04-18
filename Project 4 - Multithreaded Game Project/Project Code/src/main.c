/*----------------------------------------------------------------------------
 *----------------------------------------------------------------------------*/
#include <MKL25Z4.H>
#include <stdio.h>
#include "gpio_defs.h"

#include "tft_lcd.h"
#include "font.h"

#include "LEDs.h"
#include "timers.h"
#include "sound.h"

#include "i2c.h"
#include "mma8451.h"
#include "delay.h"
#include "profile.h"
#include "math.h"

#include <RTL.h>
#include "tasks.h"

#define USE_GFX_LCD

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {

	Init_Debug_Signals();
	Init_RGB_LEDs();
	Sound_Init();	

	TFT_Init();
	TFT_Text_Init(1);
	TFT_Erase();
	TFT_Text_PrintStr_RC(0,0, "Doodle Jump v0.1");
	
/*
	Graphics_Test();
	while (1) 
		;
*/
	
	
//	TFT_TS_Calibrate();
//	TFT_TS_Test();

	i2c_init();											// init I2C peripheral
	if (!init_mma()) {							// init accelerometer
		Control_RGB_LEDs(1,0,0);			// accel initialization failed, so turn on red error light
		while (1)
			;
	}

	Delay(70);

	os_sys_init(&Task_Init);

	while (1)
		;
}

// *******************************ARM University Program Copyright © ARM Ltd 2013*************************************   
