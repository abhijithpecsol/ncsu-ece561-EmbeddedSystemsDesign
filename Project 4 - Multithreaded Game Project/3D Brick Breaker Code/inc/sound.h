#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>
#include <RTL.h>

// On Port E
#define AMP_ENABLE_POS (29)
#define DAC_POS (30)
#define MAX_DAC_CODE (4095)
#define NUM_STEPS (64)
#define NUM_WAVEFORM_SAMPLES (512)

#define BLIP_SOUND_LEN (3507)


typedef struct {
	uint16_t Volume;
	uint16_t Duration;
	uint16_t Frequency; 
	uint16_t Type; // Sine, square, triangle, white noise
} VOICE_T;

void DAC_Init(void);
void Play_Tone(void);
void Sound_Init(void);

void Sound_Enable_Amp(void);
void Sound_Disable_Amp(void);

void Play_Tone_with_DMA(unsigned int period, unsigned int num_cycles);
//  void Play_Waveform_with_DMA(uint32_t sample_period_us, uint16_t * waveform, 
//	uint32_t num_samples);

void Sound_Refill_Buffer(uint32_t samples);

void Play_Waveform_with_DMA(void);
uint16_t Get_Next_Blip_Sample(void);

 __task void Task_Sound_Manager(void);
 __task void Task_Refill_Sound_Buffer(void);

#endif // SOUND_H
