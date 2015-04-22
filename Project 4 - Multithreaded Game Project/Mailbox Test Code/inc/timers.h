#ifndef TIMERS_H
#define TIMERS_H
#include "MKL25Z4.h"

#define USE_PIT (0)

#define LCD_UPDATE_PERIOD 10

void Init_PIT(unsigned period);
void Start_PIT(void);
void Stop_PIT(void);

void Init_TPM(unsigned period_ms);
void Start_TPM(void);

void Init_PWM(TPM_Type * TPM, uint8_t channel_num, uint16_t period, uint16_t duty);
void Set_PWM_Value(TPM_Type * TPM, uint8_t channel_num, uint16_t value);


void Init_LPTMR(void);
void Start_LPTMR(void);
void Stop_LPTMR(void);

#endif
// *******************************ARM University Program Copyright © ARM Ltd 2013*************************************   
