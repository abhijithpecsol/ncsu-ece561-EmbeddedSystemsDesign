#include "MKL25Z4.h"
#include "mma8451.h"
#include "LEDs.h"
#include <math.h>

extern float roll, pitch;

// Read acceleration
void Read_Accel(){
	read_full_xyz();
	convert_xyz_to_roll_pitch();
}

// Update LEDs appropriately
void Update_LEDs(){
	// green = less than 15 degrees
	if (fabs(roll) < 15.0 && fabs(pitch) < 15.0){
		Control_RGB_LEDs(0, 1, 0);
	}
	// red = over 30 degrees
	else if (fabs(roll) > 30.0 || fabs(roll) > 30.0){
		Control_RGB_LEDs(1, 0, 0);
	}
	// yellow = between 15 and 30 degrees
	else {
		Control_RGB_LEDs(1, 1, 0);
	}
}
