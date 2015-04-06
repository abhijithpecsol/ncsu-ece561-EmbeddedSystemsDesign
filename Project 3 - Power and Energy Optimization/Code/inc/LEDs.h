#ifndef LEDS_H
#define LEDS_H

// Freedom KL25Z LEDs
#define RED_LED_POS (18)		// on port B
#define GREEN_LED_POS (19)	// on port B
#define BLUE_LED_POS (1)		// on port D

// PWM
#define TPM_Cn_MODE        (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK)
#define TPM_MODULE         1000
#define TPM_INIT_VAL       0
#define SET_LED_GREEN(x)   TPM2->CONTROLS[1].CnV = (x)
#define SET_LED_RED(x)     TPM2->CONTROLS[0].CnV = (x)
#define SET_LED_BLUE(x)    TPM0->CONTROLS[1].CnV = (x)
#define PWM_OFF							(0)
#define PWM_ON							(10)
#define LEDS_ON_TIME			4

// function prototypes
void Init_RGB_LEDs(void);
void Control_RGB_LEDs(unsigned int red_on, unsigned int green_on, unsigned int blue_on);
void Toggle_RGB_LEDs(unsigned int red, unsigned int green, unsigned int blue);

#endif
