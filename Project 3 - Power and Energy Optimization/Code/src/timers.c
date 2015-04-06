#include "timers.h"
#include "MKL25Z4.h"
#include "LEDs.h"
#include "project3.h"
#include "gpio_defs.h"

volatile uint8_t run_Read_Accel = 0;
volatile uint8_t run_Update_LEDs = 0;
volatile uint8_t delay_Read_Accel = PERIOD_READ_ACCEL;
volatile uint8_t delay_Update_LEDs = PERIOD_UPDATE_LEDS;
volatile uint8_t led_on_period = 0;

void Init_LPTMR(void) {
	SIM->SCGC5 |=  SIM_SCGC5_LPTMR_MASK;

	// Configure LPTMR
	// select 1 kHz LPO clock with prescale factor 0, dividing clock by 2
	// resulting in 500 Hz clock
	LPTMR0->PSR = LPTMR_PSR_PCS(1) | LPTMR_PSR_PRESCALE(0); 
	LPTMR0->CSR = LPTMR_CSR_TIE_MASK;
	LPTMR0->CMR = 50; // Generate interrupt every 50 clock ticks or 100 ms

	// Configure NVIC 
	NVIC_SetPriority(LPTimer_IRQn, 128); // 0, 64, 128 or 192
	NVIC_ClearPendingIRQ(LPTimer_IRQn); 
	NVIC_EnableIRQ(LPTimer_IRQn);	
}

void Start_LPTMR(void) {
	LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;
}

void Stop_LPTMR(void) {
	LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK;
}

void LPTimer_IRQHandler(void) {
	#if DEBUG_SIGNALS
		PTE->PCOR |= MASK(30);
	#endif
	
	NVIC_ClearPendingIRQ(LPTimer_IRQn);
	LPTMR0->CSR |= LPTMR_CSR_TCF_MASK;
	
	// check what time period interrupted us
	// 10 ms LED on time, simply turn off LEDs
	if (LPTMR0->CMR == LED_ON_TIME){
		Control_RGB_LEDs(0,0,0);
		LPTMR0->CMR = (ACCEL_CHECK_FREQ - LED_ON_TIME);		// 90 ms to next 100 ms
		#if USE_PWM == 1
			led_on_period = 0;
		#endif
		return;
	}
	if (LPTMR0->CMR == (ACCEL_CHECK_FREQ - LED_ON_TIME)){
		LPTMR0->CMR = ACCEL_CHECK_FREQ;
	}
	
	// decrement delay variables
	delay_Read_Accel--;
	delay_Update_LEDs--;
	
	// check if delay period has ended; indicate and reset if so
	if (delay_Read_Accel == 0){
		run_Read_Accel = 1;
		delay_Read_Accel = PERIOD_READ_ACCEL;
	}
	if (delay_Update_LEDs == 0){
		run_Update_LEDs = 1;
		delay_Update_LEDs = PERIOD_UPDATE_LEDS;
		LPTMR0->CMR = LED_ON_TIME;
		#if USE_PWM == 1
			led_on_period = 1;
		#endif
	}
}

// Sets up configuration for the PIT. Period is how long for an interrupt.
void Init_PIT(unsigned period) {
	// Enable clock to PIT module
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	
	// Enable module, freeze timers in debug mode
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;			// enable module = 0
	PIT->MCR |= PIT_MCR_FRZ_MASK;				// freeze timers in debug = 1
	
	// Initialize PIT0 to count down from argument 
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(period);

	// No chaining of timers
	PIT->CHANNEL[0].TCTRL &= PIT_TCTRL_CHN_MASK;
	
	// Generate interrupts
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;

	/* Enable Interrupts */
	NVIC_SetPriority(PIT_IRQn, 128); // 0, 64, 128 or 192
	NVIC_ClearPendingIRQ(PIT_IRQn); 
	NVIC_EnableIRQ(PIT_IRQn);
}

void Start_PIT(void) {
// Enable counter
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
}

void Stop_PIT(void) {
// Enable counter
	PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TEN_MASK;
}

void PIT_IRQHandler() {
	//clear pending IRQ
	//NVIC_ClearPendingIRQ(PIT_IRQn);
	
	// channel 1
	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		// clear status flag for timer channel 0
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
		
		Stop_PIT();
	} 
	// channel 2
	else if (PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK) {
		// clear status flag for timer channel 1
		PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK;
	} 
}


// *******************************ARM University Program Copyright © ARM Ltd 2013*************************************   
