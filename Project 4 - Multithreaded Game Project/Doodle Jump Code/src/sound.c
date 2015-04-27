#include <stdint.h>
#include <MKL25Z4.h>
#include "sound.h"

void Init_DAC(void) {
  // Init DAC output
	
	SIM->SCGC6 |= (1UL << SIM_SCGC6_DAC0_SHIFT); 
	SIM->SCGC5 |= (1UL << SIM_SCGC5_PORTE_SHIFT); 
	
	PORTE->PCR[DAC_POS] &= ~PORT_PCR_MUX_MASK;	
	PORTE->PCR[DAC_POS] |= PORT_PCR_MUX(0);	// Select analog 
		
	// Disable buffer mode
	DAC0->C1 = 0;
	DAC0->C2 = 0;
	
	// Enable DAC, select VDDA as reference voltage
	DAC0->C0 = (1 << DAC_C0_DACEN_SHIFT) | 
							(1 << DAC_C0_DACRFS_SHIFT);

}

/*
	Code for driving DAC
*/
void Play_Sound_Sample(uint16_t val) {
	DAC0->DAT[0].DATH = DAC_DATH_DATA1(val >> 8);
	DAC0->DAT[0].DATL = DAC_DATL_DATA0(val);
}

/* Initialize sound hardware */
void Sound_Init(void) {
	Init_DAC();
	SIM->SCGC5 |= (1UL << SIM_SCGC5_PORTE_SHIFT); 
	PORTE->PCR[AMP_ENABLE_POS] &= ~PORT_PCR_MUX_MASK;	
	PORTE->PCR[AMP_ENABLE_POS] |= PORT_PCR_MUX(1);	// Select GPIO
	PTE->PDDR |= AMP_ENABLE_POS; // set to output
	PTE->PSOR = AMP_ENABLE_POS;  // enable audio amp
}
