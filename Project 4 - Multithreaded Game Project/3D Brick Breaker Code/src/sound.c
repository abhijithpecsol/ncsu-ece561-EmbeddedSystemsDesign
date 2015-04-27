#include <stdint.h>
#include <MKL25Z4.h>
#include <math.h>
#include <RTL.h>

#include "sound.h"
#include "delay.h"
#include "gpio_defs.h"
#include "timers.h"
#include "DMA.h"
#include "tasks.h"

uint16_t SineTable[NUM_STEPS];
uint16_t Waveform[NUM_WAVEFORM_SAMPLES];

uint32_t WNG_Len=0;

void DAC_Init(void) {
  // Init DAC output
	
	SIM->SCGC6 |= MASK(SIM_SCGC6_DAC0_SHIFT); 
	SIM->SCGC5 |= MASK(SIM_SCGC5_PORTE_SHIFT); 
	
	PORTE->PCR[DAC_POS] &= ~PORT_PCR_MUX_MASK;	
	PORTE->PCR[DAC_POS] |= PORT_PCR_MUX(0);	// Select analog 
		
	// Disable buffer mode
	DAC0->C1 = 0;
	DAC0->C2 = 0;
	
	// Enable DAC, select VDDA as reference voltage
	DAC0->C0 = MASK(DAC_C0_DACEN_SHIFT) | MASK(DAC_C0_DACRFS_SHIFT);
}

/*
	Code for driving DAC
*/
void Play_Sound_Sample(uint16_t val) {
	DAC0->DAT[0].DATH = DAC_DATH_DATA1(val >> 8);
	DAC0->DAT[0].DATL = DAC_DATL_DATA0(val);
}

void SineTable_Init(void) {
	unsigned n;
	
	for (n=0; n<NUM_STEPS; n++) {
		SineTable[n] = (MAX_DAC_CODE/2)*(1+sinf(n*(2*3.1415927/NUM_STEPS)));
	}
}

void Init_Waveform(void) {
	uint32_t i;
	
	for (i=0; i<NUM_WAVEFORM_SAMPLES; i++) {
		Waveform[i] = (MAX_DAC_CODE/2)*
			(1.0+((1.0-((float)i)/NUM_WAVEFORM_SAMPLES))*sinf(i*(2*3.1415927/NUM_STEPS)));
	}
}

/* Initialize sound hardware and sine table*/
void Sound_Init(void) {
	SineTable_Init();	
	Init_Waveform();

	DAC_Init();
	DMA_Init();
	TPM0_Init();
	Configure_TPM0_for_DMA(45);

	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);


	SIM->SCGC5 |= (1UL << SIM_SCGC5_PORTE_SHIFT); 
	
	PORTE->PCR[AMP_ENABLE_POS] &= ~PORT_PCR_MUX_MASK;	
	PORTE->PCR[AMP_ENABLE_POS] |= PORT_PCR_MUX(1);	// Select GPIO
	PTE->PDDR |= MASK(AMP_ENABLE_POS); // set to output
	PTE->PSOR = MASK(AMP_ENABLE_POS);  // enable audio amp

}

void Sound_Enable_Amp(void) {
	PTE->PSOR = MASK(AMP_ENABLE_POS);  // enable audio amp
}

void Sound_Disable_Amp(void) {
	PTE->PCOR = MASK(AMP_ENABLE_POS);  // disable audio amp
}

void Play_Tone(void) {
	int n, d=MAX_DAC_CODE>>1, p;
	
	for (p=5; p>=1; p--) {
		for (n=0; n<20/p; n++) {
			Play_Sound_Sample((MAX_DAC_CODE>>1)+d);
			Delay(p);
			Play_Sound_Sample((MAX_DAC_CODE>>1)-d);
			Delay(p);
		}
	}
}

uint16_t Sound_Generate_Next_Sample (void) {
	static uint16_t lfsr = 0xACE1u;
	static uint16_t bit;
	
	// source code from http://en.wikipedia.org/wiki/Linear_feedback_shift_register
  /* taps: 16 14 13 11; characteristic polynomial: x^16 + x^14 + x^13 + x^11 + 1 */
  bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
  lfsr =  (lfsr >> 1) | (bit << 15);
	return lfsr & 0x03ff;
}

// void Play_Waveform_with_DMA(uint32_t sample_period_us, uint16_t * waveform, uint32_t num_samples) {
void Play_Waveform_with_DMA(void) {
	//	Init_Waveform();
	//	Configure_TPM0_for_DMA(45);
	Configure_DMA_For_Playback(Waveform, NUM_WAVEFORM_SAMPLES, 1);
	Start_DMA_Playback();
}


void Play_Tone_with_DMA(unsigned int period, unsigned int num_cycles) {
	Configure_TPM0_for_DMA(period/NUM_STEPS);
	Configure_DMA_For_Playback(SineTable, NUM_STEPS, num_cycles);
	Start_DMA_Playback();
}


__task void Task_Sound_Manager(void) {
	
	os_itv_set(1000);
	
	while (1) {
		os_itv_wait();
		//		os_evt_wait_and(EV_PLAYSOUND, WAIT_FOREVER); // wait for trigger
		// make a new sound every second
		
		// Hack - temporary code until voice code is added
		WNG_Len = 2500;
		Play_Waveform_with_DMA();
	}
}

__task void Task_Refill_Sound_Buffer(void) {
	uint32_t i;
	
	while (1) {
		os_evt_wait_and(EV_REFILL_SOUND, WAIT_FOREVER); // wait for trigger

		for (i=0; i<NUM_WAVEFORM_SAMPLES; i++) {
			if (WNG_Len > 0) {
				Waveform[i] = Sound_Generate_Next_Sample();
				WNG_Len--;
			} else {
				Waveform[i] = MAX_DAC_CODE/2;
			}
		}
	}
}



