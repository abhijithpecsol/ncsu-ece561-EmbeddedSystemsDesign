/*----------------------------------------------------------------------------
 *
 * Approximations.h
 * Michael Meli
 *
 * Header file for approximations.c
 *
 *----------------------------------------------------------------------------*/
 
 #define ERROR_FIX (-0x4C000)
 #define PI_FLOAT     3.14159265f
 #define PIBY2_FLOAT  1.5707963f
 #define LINEAR_APPROX 0
 
 float sqrt_approx(float);
 float atan2_approx(float, float);

void populate_atan_lut(void);
float atan2_approx_lut(float x, float y);

float cos_approx(float x);
float sin_approx(float x);
