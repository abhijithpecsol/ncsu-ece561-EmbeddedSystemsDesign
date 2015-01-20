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
extern volatile unsigned int lastRedFlash;

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
	
	unsigned int lastTime = timer1ms;
	unsigned int subTimer = 0;
	unsigned int fade = 0;
	unsigned int fadeFrequency = 0;
	
	// only handle touch if we are not fading out after time out
	if (!(state & FADING_OUT)){
		// if the light was previously off fade into brightness
		if ((adjPosition != 0) && (state & OFF_STATE) && !(state & FADING_IN)){
			state |= FADING_IN;							// separate fading flag
			
			SET_LED_RED(0);
			SET_LED_GREEN(0);
			SET_LED_BLUE(0);
			
			if (adjPosition < 1000){
				fadeFrequency = 1000 / adjPosition;
				if (1000 % adjPosition != 0){
					fadeFrequency++;
				}
			}
			else {
				fadeFrequency = 1;
			}
			
			lastTime = timer1ms;
			while (subTimer < 1000){
				if (lastTime != timer1ms){
					subTimer++;
					lastTime = timer1ms;
					
					if (subTimer % fadeFrequency == 0){
						fade++;
						SET_LED_RED(fade);
						SET_LED_GREEN(fade);
						SET_LED_BLUE(fade);
					}
				}
			}
			
			state &= ~OFF_STATE;				// switch state into off state
			state |= ON_STATE;					// still "fading" for another 250ms to prevent undesired behavior
			
			lastRedFlash = timer250ms;	// reset last red flash
		}
		// if in the on state, simply change brightness as appropriate, unless position is 0
		else if (state & ON_STATE && !(state & FADING_IN)){
			// if position 0, we go into the off state
//			if (adjPosition == 0){
//				state &= ~ON_STATE;
//				state |= OFF_STATE;
//				state |= ACCEL_RESET;
//			}
			
			state |= TIMEOUT_RESET;		// reset timeout flag
			SET_LED_RED(adjPosition);
			SET_LED_GREEN(adjPosition);
			SET_LED_BLUE(adjPosition);
		}
		
		lastBrightness = adjPosition;
	}
	
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
