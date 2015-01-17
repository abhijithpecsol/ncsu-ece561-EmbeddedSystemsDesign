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
#include <math.h>

#define OFF_STATE		(0x01)
#define ON_STATE 		(0x02)

uint16_t u16LPcounter = 0u;
unsigned int lastBrightness = 200;	// the last brightness value, used to turn on light by accelerometer, starts to a nice moderate value

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
	unsigned int state = OFF_STATE;			// variable to hold state information
	
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
	
	DelayMS(100);

  for(;;)
  {
    #if TSS_USE_FREEMASTER_GUI
      FMSTR_Poll();
    #endif

		// Check for touch and adjust the LED brightness
		if (TSS_Task() == TSS_STATUS_OK)
		{
			#if (LOW_POWER_TSI)
				LowPowerControl();
			#endif
		}
		
		// While in OFF state, check for accelerometer position > 33 degrees from horizontal
		if (state & OFF_STATE){
			// check accelerometer position
			read_full_xyz();								// take a reading
			convert_xyz_to_roll_pitch();		// determine roll and pitch from horizontal
			
			// perform action based on position more than 33 degrees from horizontal
			if (fabs(roll) > 33 || fabs(pitch) > 33){				
				// adjust state to on
				state &= ~OFF_STATE;
				state |= ON_STATE;
				
				// default LEDs to the last brightness value (defaulted above)
				setLEDColor(lastBrightness,lastBrightness,lastBrightness);	
			}
		}
  }
}
