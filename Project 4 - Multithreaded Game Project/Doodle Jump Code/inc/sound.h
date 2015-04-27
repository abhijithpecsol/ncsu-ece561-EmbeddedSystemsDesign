#ifndef SOUND_H
#define SOUND_H


// On Port E
#define AMP_ENABLE_POS (29)
#define DAC_POS (30)
#define MAX_DAC_CODE (4095)

extern void Init_DAC(void);
extern void Play_Tone(void);
extern void Sound_Init(void);

#endif // SOUND_H
