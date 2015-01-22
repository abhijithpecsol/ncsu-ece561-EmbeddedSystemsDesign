/*********************************************************************
*
* Michael Meli
* ECE 561 - Spring 2015
*
* Functions for setting up the ADC and determining the reference
* voltage using the band-gap reference.
* Code from Dr. Dean
*
*********************************************************************/

#include "ADC.h"
#include "MKL25Z4.h"

// Configure ADC to read Vref
void Init_ADC(void) {
	SIM->SCGC6 |= (1UL << SIM_SCGC6_ADC0_SHIFT);
	ADC0->CFG1 = ADC_CFG1_ADLPC_MASK | ADC_CFG1_ADIV(0) | ADC_CFG1_ADICLK(0) |
	ADC_CFG1_ADLSMP_MASK | ADC_CFG1_MODE(3);
	ADC0->SC2 = ADC_SC2_REFSEL(0); 			// VREFHL selection, software trigger
	PMC->REGSC |= PMC_REGSC_BGBE_MASK;
}

// Measure the voltage of Vref (V3_3)
float Measure_VRail(void) {
	float vrail;
	unsigned res=0;
	ADC0->SC1[0] = ADC_SC1_ADCH(27); 		// start conversion on channel 27 (Bandgap reference)
	while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK));
	res = ADC0->R[0];
	vrail = (VBG_VALUE/res)*65536;			// calculate hte value of Vref
	return vrail;
}
