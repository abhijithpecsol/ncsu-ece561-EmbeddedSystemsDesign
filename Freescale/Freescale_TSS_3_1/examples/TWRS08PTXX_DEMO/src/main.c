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
*   This file contains the source for the PT60 TSS example program.
*
*END************************************************************************/

#include <hidef.h> /* for EnableInterrupts macro */

#include "TSS_API.h"
#include "module_id.h"
#include "freemaster.h"
#include "app_init.h"
#include "events.h"
#include "uart.h"
#include "main.h"

uint8_t u8Dummy = 0u;

/* GUI - FreeMASTER TSA table */
#if FMSTR_USE_TSA
  /* Example of FreeMASTER User Application Table */
  FMSTR_TSA_TABLE_BEGIN(UserAppTable)
    FMSTR_TSA_RW_VAR(u8Dummy, FMSTR_TSA_UINT8)
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
 * \brief Main function
 *
 * \param void
 *
 * \return int
 */
void main(void)
{
  Sys_Init();
  InitPorts();  /* Init application ports */
  EnableInterrupts; /* Enable Interrupts globally */

  MODULE_ID_Init(); /* Init TWRPI modules detection */
  /* Detection of TWRPI daughter board - Default TSS init */
  (void) MODULE_ID_Check_Modules();

#if TSS_USE_FREEMASTER_GUI
  FreeMASTER_Init();
#endif
  /* Set Dummy Example for FreeMASTER */
  u8Dummy = 0u;

  for(;;)
  {
#if TSS_USE_FREEMASTER_GUI
      FMSTR_Poll();
#endif

    if (TSS_Task() == TSS_STATUS_OK)
    {
      /* Touchpad Daughter Boards Management */
      (void) MODULE_ID_Check_Modules();
      /* Periodic Timer Control */
      PeriodicTimerControl();
    }
    /* Write your code here ... */
  }
}
