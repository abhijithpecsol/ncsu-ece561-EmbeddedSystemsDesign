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
 
 // Performs a square root approximation
 float sqrt_approx(float x){
	 int32_t val_int;
	 static int32_t error_fix = ERROR_FIX;
	 
	 if (x <= 0){
		 return 0;
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
