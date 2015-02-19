/*----------------------------------------------------------------------------
 *
 * Approximations.c
 * Michael Meli
 *
 * Perform approximations of certain math functions.
 *
 *----------------------------------------------------------------------------*/
 
 #include "approximations.h"
 #include <stdint.h>
 #include <math.h>
 
 float atanLUT[101];
 
 // Performs a square root approximation
 float sqrt_approx(float x){
	 int32_t val_int;
	 static int32_t error_fix = ERROR_FIX;
	 
	 if (x <= 0.0f){
		 return 0.0f;
	 }
	 
	 // perform approximation; see http://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Approximations_that_depend_on_the_floating_point_representation
	 val_int = *(int32_t*)&x;			// represent as 32 bit integer
	 val_int -= 1 << 23;						// subtract 2^m
	 val_int >>= 1;									// divide by 2
	 val_int += 1 << 29;						// add ((b+1) / 2)*2^m
	 val_int += error_fix;					// error compensation
	 return *(float*)&val_int;			// return interpreted int as float approximation
 }
  
// https://gist.github.com/volkansalma/2972237
float atan2_approx( float y, float x )
{
	float atan;
	float z = y/x;
	if ( x == 0.0f )
	{
		if ( y > 0.0f ) return PIBY2_FLOAT;
		if ( y == 0.0f ) return 0.0f;
		return -PIBY2_FLOAT;
	}
	if ( fabs(z) < 1.0f )
	{
		atan = z/(1.0f + 0.28f*z*z);
		if ( x < 0.0f )
		{
			if ( y < 0.0f ) return atan - PI_FLOAT;
			return atan + PI_FLOAT;
		}
	}
	else
	{
		atan = PIBY2_FLOAT - z/(z*z + 0.28f);
		if ( y < 0.0f ) return atan - PI_FLOAT;
	}
	return atan;
}

// populate arctan table
void populate_atan_lut(void){
	float i = 0.0;
	int index = 0;
	for (i = 0.0, index = 0; index < 100; index++, i += 0.01){
		atanLUT[index] = atanf(i);
	}
}

// perform atan approximation using lookup table
float atan2_approx_lut(float y, float x){
	float res;
	float yabs = fabs(y), xabs = fabs(x);
	
	// atan2
	// = arctan(y/x) 				if x > 0
	// = pi + arctan(y/x)		if y >= 0 and x < 0
	// = -pi + arctan(y/x)	if y < 0 and x < 0
	// = pi/2								if y > 0 and x = 0 /
	// = -pi/2							if y < 0 and x = 0 /
	// = undefined					if y = 0 and x = 0
	if (x == 0){
		if (y == 0){
			return 0;
		}
		else if (y > 0){
			return PIBY2_FLOAT;
		}
		else {
			return -PIBY2_FLOAT;
		}
	}
	else {
		if (res < 1.0){
	#if LINEAR_APPROX == 1
			// linear interprolation
			// ensures greater accuracy, but may be too slow
			float argLow, argHigh;
			int indexLow, indexHigh;
			res = yabs/xabs*100;
			argLow = floorf(res);
			indexLow = (int)argLow;
			argHigh = ceilf(res);
			indexHigh = (int)argHigh;
			res = atanLUT[indexLow] + (res - argLow)*(atanLUT[indexHigh] - atanLUT[indexLow])/(argHigh - argLow);
	#else
			// no linear interprolation
			// max error of 7 degrees
			float fArg;
			int lutIndex;
			fArg = roundf(yabs/xabs*100);
			lutIndex = (int)fArg;
			res = atanLUT[lutIndex];		// intermediate result, apply above rules
	#endif
			
			if (x > 0){			// x > 0
				return res;
			}
			else {				
				if (y >= 0){	// x < 0, y >= 0
					return PI_FLOAT + res;
				}
				else {				// x < 0, y < 0
					return -PI_FLOAT + res;
				}
			}
		}
		else {
			res = PIBY2_FLOAT - res/(res*res + 0.28f);
			if ( y < 0.0f ){
				return res - PI_FLOAT;
			}
			else {
				return res;
			}
		}
	}
}
