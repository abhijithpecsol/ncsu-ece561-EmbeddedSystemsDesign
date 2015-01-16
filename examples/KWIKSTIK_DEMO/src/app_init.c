/**HEADER********************************************************************
*
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 1989-2008 ARC International;
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
* $FileName: app_init.c$
* $Version : 3.6.1.0$
* $Date    : Dec-15-2011$
*
* Comments:
*
*   This file contains the init functions.
*
*END************************************************************************/

#include "TSS_API.h"
#include "wdog.h"
#include "uart.h"
#include "freemaster.h"

#include "app_init.h"

/***************************** Local Types *********************************/

/****************************************************************************************************
* TSS_Init_Keypad0
* ---------------------------------------------------------------------------------------------------
* Inits TSS for Keypad0 module
*****************************************************************************************************/

void TSS_Init_Keypad0(void)
{
    uint8_t lcv;

    DelayMS(300); /* For Signal Stabilization */

    /* Initializes the TSS */
    (void)TSS_Init();

    /* Set Number of Samples */
    (void)TSS_SetSystemConfig(System_NSamples_Register, 0x08);

    /* Sets the Sensitivity value for each electrode */
    for (lcv =0; lcv < TSS_N_ELECTRODES; lcv++)
    {
        (void)TSS_SetSystemConfig(System_Sensitivity_Register + lcv, 0x40);
    }

    /* Enablers Settings */
    (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register + 0u, 0x3F);
    (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register + 1u, 0x00);
    (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register + 2u, 0x00);
    (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register + 3u, 0x00);

    /* Low Power Config */
    TSS_SetSystemConfig(System_LowPowerElectrode_Register, 0u);
    TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x40);
    /* Auto Trigger Config */
    TSS_SetSystemConfig(System_SystemTrigger_Register, TSS_TRIGGER_MODE_ALWAYS);   /* TSS_TRIGGER_MODE_AUTO/TSS_TRIGGER_MODE_ALWAYS/TSS_TRIGGER_MODE_SW */

    /* Configure Control 1 */
    /* Configure the TSS Keyapd Control to report the touch and release events */
    (void)TSS_SetKeypadConfig(cKey0.ControlId,Keypad_Events_Register,(TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK));
    /* Enables Callback function. Enables the control */
    (void)TSS_SetKeypadConfig(cKey0.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK | TSS_KEYPAD_CONTROL_EN_MASK));

    #if TSS_USE_DCTRACKER
        /* Enables the TSS. Enables the DC Tracking feature. Default DC Tracking value is 0x64 */
        (void)TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK | TSS_DC_TRACKER_EN_MASK));
    #else
        /* Enables the TSS */
        (void)TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK));
    #endif
}


/**************************************************************************//*!
* FreeMASTER_Init
* --------------------------------------------------------------------------
* Initialize FreeMASTER resources
******************************************************************************/

void FreeMASTER_Init(void)
{
      /* Enable the UART5_TXD function on PTC17 */
    PORTE_PCR8 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin

    /* Enable the UART5_RXD function on PTC16 */
    PORTE_PCR9 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin

    /* UART0 and UART1 are clocked from the core clock, but all other UARTs are
     * clocked from the peripheral clock. So we have to determine which clock
     * to send to the uart_init function.
     */
    if ((FMSTR_UART_PORT == UART0_BASE_PTR) | (FMSTR_UART_PORT == UART1_BASE_PTR))
        uart_init (FMSTR_UART_PORT, CORE_CLK_KHZ, FMSTR_UART_BAUD);
    else
        uart_init (FMSTR_UART_PORT,CORE_CLK_KHZ / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1) , FMSTR_UART_BAUD);

    /* Enable UART Isr */
    ISR_ENABLE_VECT(FMSTR_UART_VECTOR);

    /* Initialize FMSTR stack */
    FMSTR_Init();
}

/***************************************************************************
* DelayMS
* --------------------------------------------------------------------------
* Common Delay function in ms units
****************************************************************************/

void DelayMS(uint16_t u16delay)
{
    uint16_t i;
    uint16_t u16timeout;

    SIM_MemMapPtr sim = SIM_BASE_PTR;

    /* Timer Init */
    sim->SCGC3 |= SIM_SCGC3_FTM2_MASK;  /* FTM2 clock enable */
    FTM2_SC = 0u;
    FTM2_CNT = 0x0000U;
    FTM2_SC |= FTM_SC_PS(7);
    FTM2_MOD = 375u;                    /* 1ms@96MHz */
    FTM2_SC |= 0x08U;

    for (i=0; i < u16delay; i++)
    {
        u16timeout = 0u;
        do
        {
            u16timeout++;
        } while (!(FTM2_SC & FTM_SC_TOF_MASK) && (u16timeout < 0x7FFFu));
        FTM2_SC &= ~FTM_SC_TOF_MASK;      /* Clear Flag */
        FTM2_CNT = 0x0000U;
    }

    FTM2_SC = 0u;
}
