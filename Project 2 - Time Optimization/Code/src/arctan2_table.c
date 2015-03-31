#include <MKL25Z4.H>
#include <math.h>

#define NUM_STEPS (256)

float ArcTan2Table[NUM_STEPS];

void Init_SineTable(void) {
	unsigned n;
	
	for (n=0; n<NUM_STEPS; n++) {
		ArcTan2Table[n] = atanf2(n);
	}
}
