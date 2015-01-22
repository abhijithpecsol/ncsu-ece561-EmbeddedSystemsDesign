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
*   This file contains callback functions.
*
*END************************************************************************/
#include "TSS_API.h"
#include "board.h"
#include "app_init.h"
#include "events.h"
#include "project1.h"
#include "LEDs.h"

extern uint16_t u16LPcounter;
extern unsigned int lastBrightness;
extern unsigned int state;			
extern volatile unsigned int timer1ms;
extern volatile unsigned int timer250ms;
extern volatile unsigned int onRedFlashPeriodStart;
extern volatile unsigned int offRedFlashPeriodStart;
extern volatile unsigned int timeoutPeriodStart;

/**
 * \brief TSS callback for control 0
 *
 * This callback function is called by TSS after Fault
 * occurence. This event is enabled always and depends on
 * selection 'generate code' if the callback is used.
 * The default CallBack Name is automatically generated with
 * automatic prefix update by current Component Name. User can
 * define own name, but then the automatic name update is not
 * functional.
 *
 * \param u8ControlId Valid unique Identifier of the Control which generated
 *                     the CallBack the Control which generated the CallBack
 *                     of Callback's source Control.
 *
 * \return void
 */
void TSS1_fCallBack1(TSS_CONTROL_ID u8ControlId)
{
  /* Set LED brightness */
	// achieving full brightness requires mapping 8 bit value to 16 bit. need some mapping because larger difference in brightness at low levels than high levels
	unsigned int adjPosition = cASlider1.Position*4;
//	if (adjPosition > 170) {
//		if (adjPosition > 250){
//			adjPosition = 0xFFFF;			// full brightness (0xFFFF) is indistinguishable from ~1000
//		}
//		else {
//			adjPosition = adjPosition + (adjPosition - 170)*9;
//		}
//	}
	
	// Fade in to the touch position if we are in the LED off and board flat state
	if (state == LED_OFF_FLAT && adjPosition != 0){
		state = FADING_IN;					// we will handle the fade in here
		
		SET_LED_RED(0);
		SET_LED_GREEN(0);						// start with off LED
		SET_LED_BLUE(0);
		
		fadeWhite(adjPosition);			// fade
		
		onRedFlashPeriodStart = timer250ms;	// reset last red flash (low voltage applicable)
		timeoutPeriodStart = timer250ms;
		state = LED_ON;							// moved into the ON state
	}
	// If the LED is on or in timeout off, then just change the LED to the touched position
	else if ((state == LED_ON || state == LED_OFF_TIMEOUT) && adjPosition != 0){
		SET_LED_RED(adjPosition);
		SET_LED_GREEN(adjPosition);
		SET_LED_BLUE(adjPosition);
	
		if (state == LED_OFF_TIMEOUT){
			onRedFlashPeriodStart = timer250ms;	// reset last red flash (low voltage applicable)
			state = LED_ON;							// moved into the ON state
		}
		timeoutPeriodStart = timer250ms;
	}
	// If the LED is on and we touch at zero, then turn off the LED
	else if (state == LED_ON && adjPosition == 0){
		SET_LED_RED(0);
		SET_LED_GREEN(0);
		SET_LED_BLUE(0);
		
		state = LED_OFF_FLAT;
		offRedFlashPeriodStart = timer250ms;
	}
	// If the LED is in timeout and we touch at zero, move into the flat off state
	else if (state == LED_OFF_TIMEOUT && adjPosition == 0){
		state = LED_OFF_FLAT;
		DelayMS(100);
	}
		
	lastBrightness = adjPosition;				// save the touched position
	
  (void)u8ControlId;
}

/**
 * \brief TSS on fault callback
 *
 * This callback function is called by TSS after Fault
 * occurence. This event is enabled always and depends on
 * selection 'generate code' if the callback is used.
 * The default CallBack Name is automatically generated with
 * automatic prefix update by current Component Name. User can
 * define own name, but then the automatic name update is not
 * functional.
 *
 * \param electrode_number Electrode number if defined otherwise default value
 *
 * \return void
 */
void TSS_fOnFault(uint8_t electrode_number)
{
  if(tss_CSSys.Faults.ChargeTimeout || tss_CSSys.Faults.SmallCapacitor) /* If large or small capacitor fault  */
  {
    (void) TSS_SetSystemConfig(System_Faults_Register, 0x00);           /* Clear the fault flag */
    (void) TSS_SetSystemConfig(System_ElectrodeEnablers_Register + 0u, 0x03);   /* Reenable electrodes */
  }

  if(tss_CSSys.Faults.SmallTriggerPeriod)
  {
    (void) TSS_SetSystemConfig(System_Faults_Register, 0x00);           /* Clear the fault flag */

    #if APP_TSS_USE_DCTRACKER
      /* Enables the TSS. Enables the DC Tracking feature. Default DC Tracking value is 0x64 */
      (void)TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK | TSS_DC_TRACKER_EN_MASK));
    #else
      /* Enables the TSS */
      (void)TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK));
    #endif
  }
}

/**
 * \brief  TSS on init callback
 *
 * This callback function is called after whole TSS Init. This
 * event is enabled always and depends on selection 'generate
 * code' if the callback is used.
 * The default CallBack Name is automatically generated with
 * automatic prefix update by current Component Name. User can
 * define own name, but then the automatic name update is not
 * functional.
 *
 * \param  void
 *
 * \return void
 */
void TSS_fOnInit(void)
{
  /* Call platform specific initialization */
  InitDevices();
}

/**
 * \brief  Custom FreeMASTER callback for suppresion of entering into a low power
 *
 * \param  void
 *
 * \return void
 */
void TSS_FmstrIsrCallbackCustom(void)
{
  /* Set Sample Interrupted flag, because SCI measures at background and it
     can interrupt sampling of GPIO based methods */
  TSS_SET_SAMPLE_INTERRUPTED();
  /* Suppress enter to low power if FreeMASTER communication running */
  u16LPcounter = 0u;
}
