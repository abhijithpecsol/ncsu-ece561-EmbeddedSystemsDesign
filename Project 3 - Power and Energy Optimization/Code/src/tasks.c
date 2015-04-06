#include "MKL25Z4.h"
#include "mma8451.h"
#include "LEDs.h"
#include <math.h>
#include "i2c.h"
#include "project3.h"
#include "approx.h"

extern float roll, pitch;
extern int16_t acc_X, acc_Y, acc_Z;
extern uint8_t i2cLockedUp;

// Read acceleration
void Read_Accel(){
	read_full_xyz();
	#if USE_ANGLE_OPT == 1
		if (i2cLockedUp != WORKING){
			// reset i2c if locked up
			i2cLockedUp = WORKING;
			i2c_reset();
		}
	#else
		if (i2cLockedUp == WORKING){
			convert_xyz_to_roll_pitch();
		}
		else {
			// reset i2c if locked up
			i2cLockedUp = WORKING;
			i2c_reset();
		}
	#endif
}

// Update LEDs appropriately
void Update_LEDs(){
	#if USE_ANGLE_OPT == 1
		// determine pitch and roll atan2 arguments
		float pitch_ratio = fabs((float)acc_X/approx_sqrtf(acc_Y*acc_Y + acc_Z*acc_Z));
		float roll_ratio = fabs((float)acc_Y/acc_Z);
		float test_ratio;
	
		// pick the larger of the two arguments
		if (roll_ratio > pitch_ratio){
			test_ratio = roll_ratio;
		}
		else {
			test_ratio = pitch_ratio;
		}
		
		// check what angle range we are in
		if (test_ratio > .57735f){
			Control_RGB_LEDs(1, 0, 0);
		}
		else if (test_ratio < .26795f){
			Control_RGB_LEDs(0, 1, 0);
		}
		else {
			Control_RGB_LEDs(1, 1, 0);
		}
	#else
		#if USE_RADIANS == 1
			// red = over 30 degrees
			if (fabs(roll) > DEG_TO_RAD(30.0) || fabs(pitch) > DEG_TO_RAD(30.0)){
				Control_RGB_LEDs(1, 0, 0);
			}
			// green = less than 15 degrees
			else if (fabs(roll) < DEG_TO_RAD(15.0) && fabs(pitch) < DEG_TO_RAD(15.0)){
				Control_RGB_LEDs(0, 1, 0);
			}
			// yellow = between 15 and 30 degrees
			else {
				Control_RGB_LEDs(1, 1, 0);
			}
		#else
			// red = over 30 degrees
			if (fabs(roll) > 30.0 || fabs(pitch) > 30.0){
				Control_RGB_LEDs(1, 0, 0);
			}
			// green = less than 15 degrees
			else if (fabs(roll) < 15.0 && fabs(pitch) < 15.0){
				Control_RGB_LEDs(0, 1, 0);
			}
			// yellow = between 15 and 30 degrees
			else {
				Control_RGB_LEDs(1, 1, 0);
			}
		#endif
	#endif
}
