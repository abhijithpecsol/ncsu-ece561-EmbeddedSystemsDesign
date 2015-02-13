/*********************************************************************
*
* Michael Meli
* ECE 561 - Spring 2015
*
* Header file for ADC.c
*
*********************************************************************/

#define VBG_VALUE (1.00)			// reference voltage value
#define SIM		SIM_BASE_PTR
#define ADC0 	ADC0_BASE_PTR
#define PMC		PMC_BASE_PTR

void Init_ADC(void);
float Measure_VRail(void);
