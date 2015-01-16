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

#include "common.h"
#include "TSS_API.h"
#include "module_id.h"
#include "freemaster.h"
#include "app_init.h"
#include "events.h"

#include "main.h"

/**************************** Prototypes ***********************************/

void GotoLowPower(uint8_t u8LowPowerType);
void LowPowerControl(void);

/****************************** Types **************************************/

uint8_t LPSWflag = 0u;
const uint32_t NVFOPT@0x0000040C = 0xFFFFFDFE;   //DISABLE EZPORT MODE IN FLASH

/*********************** GUI - FreeMASTER TSA table ************************/

#if FMSTR_USE_TSA
    /* Example of FreeMASTER User Application Table */
    FMSTR_TSA_TABLE_BEGIN(UserAppTable)
      FMSTR_TSA_RW_VAR(LPSWflag, FMSTR_TSA_UINT8)
    FMSTR_TSA_TABLE_END()

    /* FreeMASTER Global Table List definition */
    FMSTR_TSA_TABLE_LIST_BEGIN()
      #if TSS_USE_FREEMASTER_GUI
        FMSTR_TSA_TABLE(TSS_Table)
      #endif
      FMSTR_TSA_TABLE(UserAppTable)
    FMSTR_TSA_TABLE_LIST_END()
#endif

/**************************************************************************//*!
* GotoLowPower
* --------------------------------------------------------------------------
* Performs enter to MCU Low Power mode defined by parameter
******************************************************************************/

void GotoLowPower(uint8_t u8LowPowerType)
{
  LLWU_MemMapPtr psLLWUStruct = LLWU_BASE_PTR;

  /* Turn Off all LED's */
  LED1_OFF; LED2_OFF;
  /* Call application callback for Low Power Enter */
  LowPowerEnter_Callback();
  /* Setup LLWU (Low Leakage wake-up unit) for TSI */
  llwu_configure(0,0, LLWU_ME_WUME3_MASK); /* TSI LLWU Input Enable = LLWU_M3IF) */
  /* Enable possibility to wake up */
  enable_lpwui();
  /* Enter to xLLSn -> TSI Low Power */
  switch (u8LowPowerType) {
  case LP_TYPE_LLS: enter_lls(); break;
  case LP_TYPE_VLLS1: enter_vlls1(); break;
  case LP_TYPE_VLLS2: enter_vlls2(); break;
  case LP_TYPE_VLLS3: enter_vlls3(); break;
  }
  /* Disable possibility to wake up */
  disable_lpwui();
  /* Reinit PLL back to normal mode - Transition into PEE by setting CLKS to 0 */
  MCG_C1 &= ~MCG_C1_CLKS_MASK; /* CLKS=0, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0 */
}

/**************************************************************************//*!
* LowPowerControl
* --------------------------------------------------------------------------
* Manages enter to MCU Low Power mode initiated by combination of electrodes
******************************************************************************/
void LowPowerControl(void)
{
  if (LPSWflag)
  {
    /* Go to Low Power, then wake up by defined electrode */
    GotoLowPower(LP_TYPE_LLS);
    /* Re-enable LPSW */
    configure_irq_for_rising_edge();
    /* Reset LPSW flag */
    LPSWflag = 0u;
  }
}

/***************************************************************************
* Main
* --------------------------------------------------------------------------
*
****************************************************************************/
void main (void)
{
  EnableInterrupts;
  /* Init HW */
  InitPorts();  /* Init application ports */

  /* Initialization of Touchpad Daughter Boards detection */
  MODULE_ID_Init();
  /* Detection of TWRPI daughter board - Default TSS init */
  (void) MODULE_ID_Check_Modules();

  /* Init FreeMASTER resources */
#if TSS_USE_FREEMASTER_GUI
  FreeMASTER_Init();
#endif

  /* Enable Interrupts globally */
  EnableInterrupts;
  /* Reset SW flag */
  LPSWflag = 0u;

  /* Main Loop */
  for(;;)
  {

    /* FreeMASTER */
#if TSS_USE_FREEMASTER_GUI
    FMSTR_Poll();
#endif

    /* TSS Task */
    if (TSS_Task() == TSS_STATUS_OK)
    {

      /* Control of Touchpad Daughter Boards detection */
      (void) MODULE_ID_Check_Modules();
      /* Periodic Timer Control */
      PeriodicTimerControl();
      /* Low Power Control */
      LowPowerControl();
    }

    /* Write your code here ... */

  }
}

/***************************************************************************
* LPSWIsr
* --------------------------------------------------------------------------
* Interrupt Service routine for detection of LPSW push and enter to Low Power
****************************************************************************/
void LPSWIsr(void)
{
	/* Disable IRQ switch */
	disable_irq_for_rising_edge();
	/* Set LPSW flag */
	LPSWflag = 1u;
}

/***************************************************************************/
