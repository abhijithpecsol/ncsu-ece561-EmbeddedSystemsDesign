/*----------------------------------------------------------------------------
 *----------------------------------------------------------------------------*/
#include <MKL25Z4.H>
#include <stdio.h>
#include "math.h"

#include "config.h"
#include "gpio_defs.h"
#include "LCD_4bit.h"
#include "LEDs.h"
#include "timers.h"
#include "i2c.h"
#include "mma8451.h"
#include "spi.h"
#include "v2xe.h"
#include "triggers.h"
#include "delay.h"
#include "profile.h"

#include "approximations.h"

extern volatile uint8_t i2cState;
extern int32_t mag_X, mag_Y, mag_Z;
extern uint8_t i2cLockedUp;
int i2cLockedUpCount;

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
	uint16_t res=0;
#ifdef USE_LCD
	char buffer[9];
#endif
	
	Init_RGB_LEDs();
	// Initializing, so turn on yellow
	Control_RGB_LEDs(1, 1, 0);
	init_triggers();

#ifdef USE_LCD
	Init_LCD();
	Clear_LCD();
	Set_Cursor(0,0);
	Print_LCD(" Hello  ");
	Set_Cursor(0,1);
	Print_LCD(" World! ");
#endif
	
	i2c_init();																/* init i2c	*/
	i2c_reset();															// reset, useful if entering debug mode from running condition
	spi_init();																/* init spi */
	res = init_mma();													/* init mma peripheral */
	if (res == 0) {
		// Signal error condition
		Control_RGB_LEDs(1, 1, 1);			// white
		while(1);
	}
	// increse i2c baud rate
	I2C_DISABLE;
	I2C0->F = (I2C_F_ICR(0x00) | I2C_F_MULT(0));
	I2C_ENABLE;
	
//	Control_RGB_LEDs(0, 0, 0);
//	while (1) {
//		V2XEData data = {0,0,0,0,0};
//		get_v2xe_data(&data);
//		if (data.x != 0){
//			Control_RGB_LEDs(1, 1, 1);
//		}
//		Delay(1);
//	}
	
	Delay(1000);

	Init_Profiling();
	__enable_irq();
	//populate_atan_lut();
	
#if PROFILING == 1
	// Starting, so turn on magenta
	uint32_t i;
	Control_RGB_LEDs(1,0,1);
	Enable_Profiling();
	for (i=0; i<NUM_TESTS; i++) {
	#if POLLING == 1
		read_full_xyz();
		if (i2cLockedUp == WORKING){
			convert_xyz_to_roll_pitch();
		}
		else {
			// reset i2c if locked up
			i2cLockedUp = WORKING;
			i2cLockedUpCount++;
			i2c_reset();
		}
	
	#else
		i2c_int_start(MMA_ADDR, REG_XHI);
		if (i != 0){
			convert_xyz_to_roll_pitch();
		}
		while (!(i2cState == DATA_RECEIVED)){}
		i2cState = 0;
		get_accel_from_data();
	#endif
	}
	Disable_Profiling();
	// Done, turn on blue LED
	Control_RGB_LEDs(0, 0, 1);
	while(1);
	
#else
	while (1){
		if (TRIGGER_1_ASSERTED){				// test case with fixed mag values
			Control_RGB_LEDs(1, 0, 0);		// red LED
			read_full_xyz();							// read accelerometer
			if (!i2cLockedUp){
				convert_xyz_to_roll_pitch();
			}
			else {
				// reset i2c if locked up
				i2cLockedUp = WORKING;
				i2cLockedUpCount++;
				i2c_reset();
			}
			
			// TODO
			// 1 - set magnetometer readings to X_M_1, Y_M_1 and Z_M_1
			// 2 - calculate tilt-compensated direction
			
			Control_RGB_LEDs(0, 0, 0);		// turn off LEDs
		}
		else if (TRIGGER_2_ASSERTED){		// test case with fixed accel and mag
			Control_RGB_LEDs(0, 0, 1);		// blue LED
			
			// TODO
			// 1 - set accelerometer readings to X_ACC_2, Y_ACC_2 and Z_ACC_2
			// 2 - calculate roll and pitch
			// 3 - set magnetometer readings to X_M_2, Y_M_2 and Z_M_2
			// 4 - calculate tilt-compensated direction
			
			Control_RGB_LEDs(0, 0, 0);		// turn off LEDs
		}
		else {			// regular operation
			Control_RGB_LEDs(0, 1, 0);		// green LED
			read_full_xyz();							// read accelerometer
			if (!i2cLockedUp){
				convert_xyz_to_roll_pitch();	// calculate roll and pitch
			}
			else {
				// reset i2c if locked up
				i2cLockedUp = WORKING;
				i2cLockedUpCount++;
				i2c_reset();
			}
			
			// TODO
			// 1 - read magnetometer
			// 2 - calculate tilt-compensated direction
			
			Control_RGB_LEDs(0, 0, 0);		// turn off LEDs
		}
	}
#endif
}

// *******************************ARM University Program Copyright © ARM Ltd 2013*************************************   
