#include "MKL25Z4.h"
#include "mma8451.h"
#include "LEDs.h"
#include <math.h>
#include "i2c.h"
#include "project3.h"

extern float roll, pitch;
extern uint8_t i2cLockedUp;

// Read acceleration
void Read_Accel(){
	read_full_xyz();
	if (i2cLockedUp == WORKING){
			convert_xyz_to_roll_pitch();
		}
		else {
			// reset i2c if locked up
			i2cLockedUp = WORKING;
			i2c_reset();
		}
}

// Update LEDs appropriately
void Update_LEDs(){
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
}
