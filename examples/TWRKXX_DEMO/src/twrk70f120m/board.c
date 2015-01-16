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
* $FileName: board.c$
* $Version : 1.0.00.0$
* $Date    : Apr-15-2012$
*
* Comments:
*
*   This file contains the specific source code on the MCU.
*
*END************************************************************************/

#include "TSS_API.h"
#include "wdog.h"
#include "uart.h"
#include "freemaster.h"
#include "events.h"
#include "board.h"

/****************************** Types **************************************/

uint8_t LPSWflag = 0u;

/***************************************************************************
* InitPorts
* --------------------------------------------------------------------------
* Global init of the board ports
****************************************************************************/
void InitPorts(void)
{
  SIM_MemMapPtr sim = SIM_BASE_PTR;

  /* Disable WatchDog */
  wdog_disable();
  /* PORT clock enabling */
  sim->SCGC5 |= SIM_SCGC5_PORTA_MASK; /* Clock for PORTA */
  /* LED's MUX */
  PORTA_PCR11 = PORT_PCR_MUX(MUX_ALT1); /* LED1 */
  PORTA_PCR28 = PORT_PCR_MUX(MUX_ALT1); /* LED2 */
  PORTA_PCR29 = PORT_PCR_MUX(MUX_ALT1); /* LED3 */
  PORTA_PCR10 = PORT_PCR_MUX(MUX_ALT1); /* LED4 */
  /* LED's Init */
  GPIOA_PDDR |= (LED1 | LED2 | LED3 | LED4); /* Set pin to OUTPUT */
  /* Turn Off All LED's */
  LED1_OFF;   LED2_OFF;   LED3_OFF;   LED4_OFF;
  /* Set SW2 for button which enters to Low Power mode */
  sim->SCGC5 |= SIM_SCGC5_PORTE_MASK; /* PORTE clock enablement */
  PORTE_PCR26 = 0u;
  PORTE_PCR26 |= PORT_PCR_MUX(MUX_ALT1); /* Set SW2 for GPIO function */
  GPIOE_PDDR &= ~(1u << 26u); /* Set GPIO pin as Input */
  PORTE_PCR26 |= PORT_PCR_PS_MASK; /* Select pull-up */
  PORTE_PCR26 |= PORT_PCR_PE_MASK; /* Enable pull-up */
  /* Set Interrupt */
  PORTE_PCR26 &= ~PORT_PCR_IRQC_MASK; /* Disable Interrupt */
  PORTE_PCR26 |= PORT_PCR_IRQC(0x0A); /* Set Interrupt on Falling Edge */
  ISR_ENABLE_VECT(107); /* Enable LPSW intr */
}

/***************************************************************************
* InitDevices
* --------------------------------------------------------------------------
* Global init of the devices
****************************************************************************/
void InitDevices(void)
{
  SIM_MemMapPtr sim = SIM_BASE_PTR;

  /* Modules Clock enabling */
  sim->SCGC5 |= SIM_SCGC5_TSI_MASK; /* TSI clock enablement */
  sim->SCGC6 |= SIM_SCGC6_FTM1_MASK; /* TSS_HW_TIMER clock enablement */
  /* PORT clock enablement */
  sim->SCGC5 |= SIM_SCGC5_PORTA_MASK; /* PORTA clock enablement */
  sim->SCGC5 |= SIM_SCGC5_PORTB_MASK; /* PORTB clock enablement */
  sim->SCGC5 |= SIM_SCGC5_PORTC_MASK; /* PORTC clock enablement */
  /* Set Electrodes for TSI function */
  PORTB_PCR0 = PORT_PCR_MUX(MUX_ALT0);
  PORTB_PCR1 = PORT_PCR_MUX(MUX_ALT0);
  PORTB_PCR2 = PORT_PCR_MUX(MUX_ALT0);
  PORTB_PCR3 = PORT_PCR_MUX(MUX_ALT0);
  PORTC_PCR0 = PORT_PCR_MUX(MUX_ALT0);
  PORTC_PCR1 = PORT_PCR_MUX(MUX_ALT0);
  PORTC_PCR2 = PORT_PCR_MUX(MUX_ALT0);
  PORTA_PCR4 = PORT_PCR_MUX(MUX_ALT0);
  PORTB_PCR16 = PORT_PCR_MUX(MUX_ALT0);
  PORTB_PCR17 = PORT_PCR_MUX(MUX_ALT0);
  PORTB_PCR18 = PORT_PCR_MUX(MUX_ALT0);
  PORTB_PCR19 = PORT_PCR_MUX(MUX_ALT0);
}

/**************************************************************************//*!
* FreeMASTER_Init
* --------------------------------------------------------------------------
* Initialize FreeMASTER resources
******************************************************************************/
void FreeMASTER_Init(void)
{
  /* Enable the UART2_TXD function on the pin */
  PORTE_PCR16 = PORT_PCR_MUX(0x3);
  /* Enable the UART2_RXD function on the pin */
  PORTE_PCR17 = PORT_PCR_MUX(0x3);
  /* UART0 and UART1 are clocked from the core clock, but all other UARTs are
  * clocked from the peripheral clock. So we have to determine which clock
  * to send to the uart_init function.
  */
  if ((FMSTR_UART_PORT == UART0_BASE_PTR) || (FMSTR_UART_PORT == UART1_BASE_PTR))
    uart_init (FMSTR_UART_PORT, CORE_CLK_HZ, FMSTR_UART_BAUD);
  else
    uart_init (FMSTR_UART_PORT, CORE_CLK_HZ / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1) , FMSTR_UART_BAUD);

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
void DelayMS(uint16_t u16delay, uint8_t u8CallbackEnabler)
{
  uint16_t i;
  uint16_t u16timeout;
  SIM_MemMapPtr sim = SIM_BASE_PTR;

  /* Timer Init */
  sim->SCGC3 |= SIM_SCGC3_FTM2_MASK;  /* FTM2 clock enable */
  FTM2_SC = 0u;
  FTM2_CNT = 0x0000U;
  FTM2_SC |= FTM_SC_PS(7);
  FTM2_MOD = 469u;                    /* 1ms@120MHz */
  FTM2_SC |= 0x08U;

  for (i=0; i < u16delay; i++)
  {
    u16timeout = 0u;
    do {
	  if (u8CallbackEnabler) DelayMS_Callback();
      u16timeout++;
    } while (!(FTM2_SC & FTM_SC_TOF_MASK) && (u16timeout < 0x7FFFu));
    FTM2_SC &= ~FTM_SC_TOF_MASK;      /* Clear Flag */
    FTM2_CNT = 0x0000U;
  }

  FTM2_SC = 0u;
}

/***************************************************************************//*!
*
* @brief  PIT Timer Init for Auto Trigger Mode function
*
* @param  void
*
* @return void
*
* @remarks
*
****************************************************************************/
void PITInit(void)
{
  SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;    /* PIT clock enable */
  ISR_ENABLE_VECT(84);                /* PIT vector enable */

  PIT_MCR = 0x00u;                    /* Enable PIT */
  PIT_LDVAL0 = 0x0128000;             /* setup timer 0 for 20ms */
  PIT_TCTRL0 = PIT_TCTRL_TIE_MASK;    /* enable Timer 0 interrupts */
  PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK;   /* start Timer 0 */
}

/***************************************************************************//*!
*
* @brief  PIT Timer Stop for Auto Trigger Mode function
*
* @param  void
*
* @return void
*
* @remarks
*
****************************************************************************/
void PITStop(void)
{
  SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;    /* PIT clock enable */
  PIT_MCR = PIT_MCR_MDIS_MASK;         /* Turn off PIT */
}

/***************************************************************************//*!
*
* @brief  PIT Timer Isr function
*
* @param  void
*
* @return void
*
* @remarks
*
****************************************************************************/
void PITIsr(void)
{
  /* Clears the PIT interrupt flag */
  PIT_TFLG0 = PIT_TFLG_TIF_MASK;
  /* Read any PIT register as workaround for 0M33Z maskset errata e2682 */
  (void)PIT_MCR;
  /* Call application callback */
  PeriodicTimer_Callback();
}

/**************************************************************************//*!
* GotoLowPower
* --------------------------------------------------------------------------
* Performs enter to MCU Low Power mode defined by parameter
******************************************************************************/
void GotoLowPower(uint8_t u8LowPowerType)
{
  LLWU_MemMapPtr psLLWUStruct = LLWU_BASE_PTR;

  /* Setup LLWU (Low Leakage wake-up unit) for TSI */
  psLLWUStruct->ME |= LLWU_ME_WUME4_MASK;         /* TSI LLWU Input Enable = LLWU_M4IF) */
  /* Turn Off all LED's */
  LED1_OFF; LED2_OFF; LED3_OFF; LED4_OFF;
  /* Enable LLWU interrupt */
  ISR_ENABLE_VECT(37);
  /* Call application callback for Low Power Enter */
  LowPowerEnter_Callback(TRUE);
  /* Enter to xLLSn -> TSI Low Power */
  SMC_PMCTRL |= SMC_PMCTRL_LPWUI_MASK | SMC_PMCTRL_STOPM(u8LowPowerType);
  if (u8LowPowerType != LP_TYPE_LLS)
  {
    SMC_VLLSCTRL = SMC_VLLSCTRL_VLLSM(1u); /* VLLS1 */
  }
  /* Goto STOP with SLEEPDEEP enabled */
  stop();
  /* TSI LLWU Input Disable = LLWU_M4IF) */
  psLLWUStruct->ME &= ~LLWU_ME_WUME4_MASK;
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
    /* Go to Low Power, then wake up by wake up electrode */
    GotoLowPower(LOW_POWER_MODE);
    /* Re-enable LPSW */
    PORTE_PCR26 |= PORT_PCR_IRQC(0x0A); /* Set Interrupt on Falling Edge */
    ISR_ENABLE_VECT(107); /* Enable LPSW intr */
    /* Clear LPSWflag flag */
    LPSWflag = 0u;
  }
}

/***************************************************************************
* LPSWIsr
* --------------------------------------------------------------------------
* Interrupt Service routine for detection of LPSW push and enter to Low Power
****************************************************************************/
void LPSWIsr(void)
{
  PORTE_PCR26 |= PORT_PCR_ISF_MASK; /* Reset Intr Flag */
  PORTE_PCR26 &= ~PORT_PCR_IRQC_MASK; /* Disable Interrupt on Falling Edge */
  ISR_DISABLE_VECT(107); /* Disable SW intr */
  LPSWflag = 1u; /* Set LPSW flag */
}

/***************************************************************************
* LLWUIsr
* --------------------------------------------------------------------------
* LLWU isr routine
****************************************************************************/
void LLWUIsr(void)
{
  uint8_t u8temp;

  /* Clear flags */
  if (PMC_REGSC &  PMC_REGSC_ACKISO_MASK)
  {
    PMC_REGSC |= PMC_REGSC_ACKISO_MASK;
  }
  if (LLWU_F1)
  {
    u8temp = LLWU_F1;
    LLWU_F1 |= u8temp;
  }
  if (LLWU_F2)
  {
    u8temp = LLWU_F2;
    LLWU_F2 |= u8temp;
  }
  if (LLWU_F3)
  {
    u8temp = LLWU_F3;
    LLWU_F3 |= u8temp;
  }
  if(LLWU_FILT1 & LLWU_FILT1_FILTF_MASK)
  {
    LLWU_FILT1 |= LLWU_FILT1_FILTF_MASK;
  }
  if(LLWU_FILT2 & LLWU_FILT2_FILTF_MASK)
  {
    LLWU_FILT2 |= LLWU_FILT2_FILTF_MASK;
  }
}


/***************************************************************************/
