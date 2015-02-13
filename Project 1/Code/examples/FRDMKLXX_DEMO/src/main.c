/*HEADER********************************************************************
*
* Copyright (c) 2013 Freescale Semiconductor;
* All Rights Reserved
*
***************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* Comments:
*
*   This file contains the source for the TSS example program.
*
*END************************************************************************/
#include "TSS_API.h"
#include "freemaster.h"
#include "app_init.h"
#include "events.h"
#include "main.h"
#include "board.h"
#include "i2c.h"
#include "mma8451.h"
#include "LEDs.h"
#include "timers.h"
#include "project1.h"
#include "ADC.h"

uint16_t u16LPcounter = 0u;
unsigned int lastBrightness = 200;	// the last brightness value, used to turn on light by accelerometer, starts to a nice moderate value
unsigned int state;									// variable to hold state information
extern volatile unsigned int timer250ms;
extern volatile unsigned int timer1ms;
unsigned int timeoutPeriodStart;
unsigned int offRedFlashPeriodStart;
unsigned int onRedFlashPeriodStart = 0;
/*********************** GUI - FreeMASTER TSA table ************************/

#if FMSTR_USE_TSA
  /* Example of FreeMASTER User Application Table */
  FMSTR_TSA_TABLE_BEGIN(UserAppTable)
    FMSTR_TSA_RW_VAR(u16LPcounter, FMSTR_TSA_UINT8)
  FMSTR_TSA_TABLE_END()

  /* FreeMASTER Global Table List definition */
  FMSTR_TSA_TABLE_LIST_BEGIN()
    #if TSS_USE_FREEMASTER_GUI
      FMSTR_TSA_TABLE(TSS_Table)
    #endif
    FMSTR_TSA_TABLE(UserAppTable)
  FMSTR_TSA_TABLE_LIST_END()
#endif

/**
 * \brief  Main function
 *
 * \param  void
 *
 * \return int
 */
int main (void)
{	
	unsigned int flags = 0;
	
  InitPorts();
	
  /* Default TSS init */
  TSS_Init_ASlider();
	
	/* init PWM for LED control */
	PWM_init();
		
  /* Enable Interrupts globally */
  EnableInterrupts();
	
  /* Reset Low Power Counter flag */
  u16LPcounter = 0u;
	
	/* Init i2c	*/
	i2c_init();

	/* init mma peripheral */
	if (!init_mma()) {									// enter this statement if error
		setLEDColor(500,0,500);						// magenta indicates error
		while (1);												// hold execution
	}
	
	// Init ADC
	Init_ADC();
	
	DelayMS(100);
	
	// init PIT
	PIT_init(23999);										// interrupt every 1 ms (24 MHz)
	
	Start_PIT();
	
	state = LED_OFF_FLAT;								// by default, in LED OFF state
	flags = NO_FLAGS;										// should be no flags set to begin
	
  for(;;)
  {
		float batteryVoltage = 0;
		
    #if TSS_USE_FREEMASTER_GUI
      FMSTR_Poll();
    #endif
		
		
		// ***************************************************************
		// ********************       ANY STATE       ********************
		// ***************************************************************
		// Check for touch and adjust the LED brightness
		if (TSS_Task() == TSS_STATUS_OK)
		{
			#if (LOW_POWER_TSI)
				LowPowerControl();
			#endif
		}
		
		// Check for low voltage
		batteryVoltage = Measure_VRail();
		if (batteryVoltage < 3.2){
			if (!(flags & LOW_VOLTAGE)){
				flags |= LOW_VOLTAGE;
				offRedFlashPeriodStart = timer250ms - 20;
			}
		}
		else {
			flags &= ~LOW_VOLTAGE;
		}
		
		// ***************************************************************
		// ************       LED OFF, BOARD FLAT STATE       ************
		// ***************************************************************
		if (state == LED_OFF_FLAT){
			// If tilt > 33 degrees, brighten LED
			if (compareAccelAngle(33)){
				state = FADING_IN;			// move into fading in state
				flags |= TILTED_ON;			// signify that the board was tilted on
			}
			
			// If voltage is low, flash the red LED for 100ms every 5 seconds
			if ((flags & LOW_VOLTAGE) && (timer250ms > offRedFlashPeriodStart + 20)){
				flashRedLED(100);
				offRedFlashPeriodStart = timer250ms;
			}
		}
		
		
		// ***************************************************************
		// ***************       LED FADING IN STATE       ***************
		// ***************************************************************
		else if (state == FADING_IN){
			// Fade into the last selected brightness
			// If the last brightness was 0, fade into a default value
			if (lastBrightness == 0){
				lastBrightness = 100;
			}
			fadeWhite(lastBrightness);
			
			onRedFlashPeriodStart = timer250ms;					// used to count when to flash red LED in on state with low voltage, must be reset when we enter
			timeoutPeriodStart = timer250ms;		// timeout counter must be reset when we enter on state
			DelayMS(100);												// quick delay to avoid undesired behavior
			
			state = LED_ON;											// move into the LED on state
		}
		
		
		// ***************************************************************
		// *******************       LED ON STATE       ******************
		// ***************************************************************
		else if (state == LED_ON){
			// Check for 10 second time out
			if (timer250ms >= timeoutPeriodStart + 40){
				state = DIMING_OUT;
				flags |= TIMING_OUT;
			}
			
			// If low voltage, turn LED red for 250 ms after 2 seconds of white
			if ((flags & LOW_VOLTAGE) && ((timer250ms - onRedFlashPeriodStart >= 8) || (onRedFlashPeriodStart == 0))){
				flashRedLED(250);
				onRedFlashPeriodStart = timer250ms;
			}
			
			// If we tilted the board on, dim the LED if we unrotate the board
			if ((flags & TILTED_ON) && !(compareAccelAngle(33))){
				state = DIMING_OUT;
			}
		}
		
		
		// ***************************************************************
		// ****************       LED DIMMING STATE       ****************
		// ***************************************************************
		else if (state == DIMING_OUT){
			fadeOutWhite();													// fade out to white
			offRedFlashPeriodStart = timer250ms;				// reset red flash period start, applicable for low voltage in off state
			DelayMS(100);
			
			// If we are dimming due to a tilt < 33 degrees, go straight to LED off and board flat state
			if (flags & TILTED_ON && !(flags & TIMING_OUT)){
				state = LED_OFF_FLAT;
				flags &= ~TILTED_ON;				// clear tilted on flag
			}
			// Otherwise, we clearly timed out, so go to timeout state
			else {
				flags &= ~TIMING_OUT;
				state = LED_OFF_TIMEOUT;
			}
			
			offRedFlashPeriodStart = timer250ms;
		}
		
	
		// ***************************************************************
		// *************       LED OFF, TIMEOUT STATE       **************
		// ***************************************************************
		else if (state == LED_OFF_TIMEOUT){
			// If tilt less than 33 degrees, go to board flat off state
			if (!(compareAccelAngle(33))){
				state = LED_OFF_FLAT;
				DelayMS(500);				// short delay to prevent a > 33 degree reading immediately, which would throw us into the on state
			}
			
			// If voltage is low, flash the red LED for 100ms every 5 seconds
			if ((flags & LOW_VOLTAGE) && (timer250ms > offRedFlashPeriodStart + 20)){
				flashRedLED(100);
				offRedFlashPeriodStart = timer250ms;
			}
		}
  }
}
