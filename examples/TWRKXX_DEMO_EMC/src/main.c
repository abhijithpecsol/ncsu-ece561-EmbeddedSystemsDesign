/**HEADER********************************************************************
*
* Copyright (c) 2012 Freescale Semiconductor;
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
* $FileName: main.c$
* $Version : 3.6.49.0$
* $Date    : Nov-7-2012$
*
* Comments:
*
*   This file contains the source for the TSS example program.
*
*END************************************************************************/

#include "TSS_API.h"
#include "module_id.h"
#include "freemaster.h"
#include "app_init.h"
#include "events.h"
#include "board.h"

/********************************* Types ***********************************/

uint8_t u8Dummy;

/*********************** GUI - FreeMASTER TSA table ************************/

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

/***************************************************************************
* Main
* --------------------------------------------------------------------------
*
****************************************************************************/
int main (void)
{
  /* Init HW */
  InitPorts();
  /* Initialization of Touchpad Daughter Boards detection */
  MODULE_ID_Init();
  /* Detection of TWRPI daughter board - Default TSS init */
  (void) MODULE_ID_Check_Modules();
  /* Init FreeMASTER resources */
  #if TSS_USE_FREEMASTER_GUI
    FreeMASTER_Init();
  #endif
  /* Enable Interrupts globally */
  EnableInterrupts();

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
    }
    /* Write your code here ... */
  }

}

/***************************************************************************/
