#include <MKL25Z4.H>
#include "LEDs.h"
#include "gpio_defs.h"
#include "project3.h"

#if USE_PWM == 1
	void Init_RGB_LEDs(void) {
		SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;
		SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM2_MASK;
		SIM->SOPT2 |= SIM_SOPT2_TPMSRC(2);

		PORTB->PCR[18] &= ~PORT_PCR_MUX_MASK;  		
		PORTB->PCR[18] |= PORT_PCR_MUX(3);  			// TPM2_CH0 enable on PTB18
		PORTB->PCR[19] &= ~PORT_PCR_MUX_MASK;  
		PORTB->PCR[19] |= PORT_PCR_MUX(3);  			// TPM2_CH1 enable on PTB19
		PORTD->PCR[1] &= ~PORT_PCR_MUX_MASK;  
		PORTD->PCR[1] |= PORT_PCR_MUX(4);  				// TPM0_CH1 enable on PTD1

		TPM0->MOD  = TPM_MODULE;   								// 0x0063 / 25MHz = 4uS PWM period
		TPM0->CONTROLS[1].CnSC = TPM_Cn_MODE;  		// No Interrupts; High True pulses on Edge Aligned PWM
		TPM0->CONTROLS[1].CnV  = TPM_INIT_VAL; 		// set to initial duty cycle

		TPM2->MOD  = TPM_MODULE;   								// 0x0063 / 25MHz = 4uS PWM period */
		TPM2->CONTROLS[0].CnSC = TPM_Cn_MODE;  		// No Interrupts; High True pulses on Edge Aligned PWM
		TPM2->CONTROLS[0].CnV  = TPM_INIT_VAL; 		// set to initial duty cycle
		TPM2->CONTROLS[1].CnSC = TPM_Cn_MODE;  		// No Interrupts; High True pulses on Edge Aligned PWM
		TPM2->CONTROLS[1].CnV  = TPM_INIT_VAL; 		// set to initial duty cycle

		TPM2->SC = TPM_SC_CMOD(1) | TPM_SC_PS(0);  // Edge Aligned PWM running from BUSCLK
		TPM0->SC = TPM_SC_CMOD(1) | TPM_SC_PS(0);  // Edge Aligned PWM running from BUSCLK
	}
		
	// rudimentary implementation
	void Control_RGB_LEDs(unsigned int red_on, unsigned int green_on, unsigned int blue_on){
		if (red_on) {
				SET_LED_RED(PWM_ON);
		} else {
				SET_LED_RED(PWM_OFF);
		}
		if (green_on) {
				SET_LED_GREEN(PWM_ON);
		}	else {
				SET_LED_GREEN(PWM_OFF);
		} 
		if (blue_on) {
				SET_LED_BLUE(PWM_ON);
		}	else {
				SET_LED_BLUE(PWM_OFF);
		}
	}
#else
	void Init_RGB_LEDs(void) {
		// Enable clock to ports B and D
		SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;;
		
		// Make 3 pins GPIO
		PORTB->PCR[RED_LED_POS] &= ~PORT_PCR_MUX_MASK;          
		PORTB->PCR[RED_LED_POS] |= PORT_PCR_MUX(1);          
		PORTB->PCR[GREEN_LED_POS] &= ~PORT_PCR_MUX_MASK;          
		PORTB->PCR[GREEN_LED_POS] |= PORT_PCR_MUX(1);          
		PORTD->PCR[BLUE_LED_POS] &= ~PORT_PCR_MUX_MASK;          
		PORTD->PCR[BLUE_LED_POS] |= PORT_PCR_MUX(1);       

		// Initially off
		PTB->PSOR = MASK(RED_LED_POS); 
		PTB->PSOR = MASK(GREEN_LED_POS); 
		PTD->PSOR = MASK(BLUE_LED_POS); 
		
		// Set ports to outputs
		PTB->PDDR |= MASK(RED_LED_POS) | MASK(GREEN_LED_POS);
		PTD->PDDR |= MASK(BLUE_LED_POS);
	}

	void Control_RGB_LEDs(unsigned int red_on, unsigned int green_on, unsigned int blue_on) {
		if (red_on) {
				PTB->PCOR = MASK(RED_LED_POS);
		} else {
				PTB->PSOR = MASK(RED_LED_POS); 
		}
		if (green_on) {
				PTB->PCOR = MASK(GREEN_LED_POS);
		}	else {
				PTB->PSOR = MASK(GREEN_LED_POS); 
		} 
		if (blue_on) {
				PTD->PCOR = MASK(BLUE_LED_POS);
		}	else {
				PTD->PSOR = MASK(BLUE_LED_POS); 
		}
	}	
#endif
