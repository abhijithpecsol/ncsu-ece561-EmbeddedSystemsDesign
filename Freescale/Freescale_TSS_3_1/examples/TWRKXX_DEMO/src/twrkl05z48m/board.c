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
#include "uart_kl05.h"
#include "freemaster.h"
#include "events.h"
#include "board.h"

/******************************* Macros ************************************/

#define APP_LPTRM_USE_OSCERKSEL 1

/****************************** Types **************************************/

uint8_t LPSWflag = 0u;

/***************************************************************************
* NVIC_SetPriority
* --------------------------------------------------------------------------
* Set priority of the interrupt
****************************************************************************/
void NVIC_SetPriority(uint8_t IRQn, uint8_t priority)
{
  NVIC_MemMapPtr NVIC = NVIC_BASE_PTR;

  if(IRQn > 0)
  {
    NVIC->IP[(uint8_t) IRQn >> 2u]  &=  (uint32_t)(~(0xFF << (((uint32_t) IRQn & 0x03u) * 8u)));
    NVIC->IP[(uint8_t) IRQn >> 2u]  |=  (uint32_t)((uint32_t)priority << (((uint32_t) IRQn & 0x03u) * 8u));
  }
}

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
  sim->SCGC5 |= SIM_SCGC5_PORTB_MASK; /* PORTB clock enablement */
  /* LED's MUX */
  PORTB_PCR20 = PORT_PCR_MUX(MUX_ALT1); /* LED1 */
  PORTB_PCR19 = PORT_PCR_MUX(MUX_ALT1); /* LED2 */
  /* LED's Init */
  GPIOB_PDDR |= (LED1 | LED2); /* Set pin to OUTPUT */
  /* Turn Off All LED's */
  LED1_OFF; LED2_OFF;
  /* Set SW2 for button which enters to Low Power mode */
  PORTB_PCR14 = 0u;
  PORTB_PCR14 = PORT_PCR_MUX(MUX_ALT1); /* Set SW3 for GPIO function */
  /* Set Interrupt */
  PORTB_PCR14 &= ~PORT_PCR_IRQC_MASK; /* Disable Interrupt */
  PORTB_PCR14 |= PORT_PCR_IRQC(0x0A) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; /* Set Interrupt on Falling Edge */
  enable_irq(INT_PORTD - 16); /* Enable LPSW intr */
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
  sim->SCGC5 |= SIM_SCGC5_TSI_MASK;  /* TSI clock enablement */
  sim->SCGC6 |= SIM_SCGC6_TPM1_MASK; /* TSS_HW_TIMER clock enablement */
  /* Set Electrodes for TSI function */
  PORTA_PCR9 = PORT_PCR_MUX(MUX_ALT0);
  PORTA_PCR8 = PORT_PCR_MUX(MUX_ALT0);
#if (TSS_USE_FREEMASTER_GUI == 0)
  PORTB_PCR2 = PORT_PCR_MUX(MUX_ALT0); /* Conflict with UART0 */
  PORTB_PCR1 = PORT_PCR_MUX(MUX_ALT0); /* Conflict with UART0 */
#endif
  PORTB_PCR0 = PORT_PCR_MUX(MUX_ALT0);
  PORTA_PCR7 = PORT_PCR_MUX(MUX_ALT0);
  PORTB_PCR11 = PORT_PCR_MUX(MUX_ALT0);
  PORTB_PCR10 = PORT_PCR_MUX(MUX_ALT0);
  PORTB_PCR12 = PORT_PCR_MUX(MUX_ALT0);
  PORTB_PCR13 = PORT_PCR_MUX(MUX_ALT0);
  PORTA_PCR11 = PORT_PCR_MUX(MUX_ALT0);
  PORTA_PCR10 = PORT_PCR_MUX(MUX_ALT0);

}

/**************************************************************************//*!
* FreeMASTER_Init
* --------------------------------------------------------------------------
* Initialize FreeMASTER resources
******************************************************************************/
void FreeMASTER_Init(void)
{
  /* Enable the SCI0_TXD function on the pin */
  PORTB_PCR1 = PORT_PCR_MUX(0x2);
  /* Enable the SCI0_RXD function on the pin */
  PORTB_PCR2 = PORT_PCR_MUX(0x2);
  /* Select the clock source for UART0 */
  SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);
  /* UART0 and UART1 are clocked from the core clock, but all other UARTs are
  * clocked from the peripheral clock. So we have to determine which clock
  * to send to the uart_init function.
  */
  uart0_init (FMSTR_UART_PORT, CORE_CLK_HZ, FMSTR_UART_BAUD);
  /* Enable UART Isr */
  enable_irq(FMSTR_UART_VECTOR - 16);
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
  sim->SCGC6 |= SIM_SCGC6_TPM0_MASK;  /* TPM0 clock enable */
  sim->SOPT2 |= SIM_SOPT2_TPMSRC(1);  /* 00=Clk.disabled,01=MCGFLLCLK/MCGPLLCLK/2,10 = OSCERCLK,11=MCGIRCLK */
  TPM0_SC = 0u;
  TPM0_CNT = 0x0000U;
  TPM0_SC |= TPM_SC_PS(7);
  TPM0_MOD = 188u;                    /* 1ms@48MHz */
  TPM0_SC |= 0x08U;

  for (i=0; i < u16delay; i++)
  {
    u16timeout = 0u;
    do {
	  if (u8CallbackEnabler) DelayMS_Callback();
      u16timeout++;
    } while (!(TPM0_SC & TPM_SC_TOF_MASK) && (u16timeout < 0x7FFFu));
    TPM0_SC |= TPM_SC_TOF_MASK;      /* Clear Flag */
    TPM0_CNT = 0x0000U;
  }

  TPM0_SC = 0u;
}

/***************************************************************************//*!
*
* @brief  LPTMR Timer Init for Auto Trigger Mode function
*
* @param  void
*
* @return void
*
* @remarks
*
****************************************************************************/
void LPTMRInit(void)
{
  SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;

  #if defined(APP_LPTRM_USE_LPO) /* Does not run in VLLS0 */
    LPTMR0_PSR = ( LPTMR_PSR_PRESCALE(0) /* 0000 is div 2 */
                 | LPTMR_PSR_PBYP_MASK   /* Selected Clk directly to LPTMR */
                 | LPTMR_PSR_PCS(1)) ;   /* use the choice of clock 0=MCGIRCLK/1=LPOCLK/2=ERCLK32/3=OSCERCLK */
    LPTMR0_CMR = LPTMR_CMR_COMPARE(14); /* Set compare value for 20 ms */
  #elif defined(APP_LPTRM_USE_MCGIRCLK)
    LPTMR0_PSR = ( LPTMR_PSR_PRESCALE(0) /* 0000 is div 2 */
                 | LPTMR_PSR_PBYP_MASK   /* Selected Clk directly to LPTMR */
                 | LPTMR_PSR_PCS(0)) ;   /* use the choice of clock 0=MCGIRCLK/1=LPOCLK/2=ERCLK32/3=OSCERCLK */
    LPTMR0_CMR = LPTMR_CMR_COMPARE(480); /* Set compare value for 20 ms */
  #elif defined(APP_LPTRM_USE_OSCERKSEL)
    /* Select ERCLK32 clock for RTC/LPTMR (0 = OSC32KCLK, 2 = RTC_CLKIN extpin (32.768kHz osc), 3 = LPO (1kHz)) */
    SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL(0);
    OSC0_CR = OSC_CR_ERCLKEN_MASK | OSC_CR_EREFSTEN_MASK;
    LPTMR0_PSR = ( LPTMR_PSR_PRESCALE(3) /* 3 is div 16 */
                 | LPTMR_PSR_PCS(3)) ;   /* use the choice of clock 0=MCGIRCLK/1=LPOCLK/2=ERCLK32/3=OSCERCLK */
    LPTMR0_CMR = LPTMR_CMR_COMPARE(41); /* Set compare value for  20 ms */
  #elif defined(APP_LPTRM_USE_OSC32KSEL)
    /* Select ERCLK32 clock for RTC/LPTMR (0 = OSC32KCLK, 2 = RTC_CLKIN extpin (32.768kHz osc), 3 = LPO (1kHz)) */
    SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL(0);
    OSC0_CR = OSC_CR_ERCLKEN_MASK | OSC_CR_EREFSTEN_MASK;
    LPTMR0_PSR = ( LPTMR_PSR_PRESCALE(0) /* 0000 is div 2 */
                 | LPTMR_PSR_PBYP_MASK   /* Selected Clk directly to LPTMR */
                 | LPTMR_PSR_PCS(2)) ;   /* use the choice of clock 0=MCGIRCLK/1=LPOCLK/2=ERCLK32/3=OSCERCLK */
    LPTMR0_CMR = LPTMR_CMR_COMPARE(660); /* Set compare value for 20 ms */
  #elif defined(APP_LPTRM_USE_RTC_CLKIN) /* Run in all MCU modes, but RTC_CLKIN is shared with TSI_CH14 pin on KL25 */
    /* Set pin PTC1/RTC_CLKIN for MUX_ALT1 function where RTC_CLKIN is enabled */
    PORTA_PCR5 |= PORT_PCR_MUX(MUX_ALT1);
    /* Select ERCLK32 clock for RTC/LPTMR (0 = OSC32KCLK, 2 = RTC_CLKIN extpin (32.768kHz osc), 3 = LPO (1kHz)) */
    SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL(2);
    LPTMR0_PSR = ( LPTMR_PSR_PRESCALE(0) /* 0000 is div 2 */
                 | LPTMR_PSR_PBYP_MASK   /* Selected Clk directly to LPTMR */
                 | LPTMR_PSR_PCS(2)) ;   /* use the choice of clock 0=MCGIRCLK/1=LPOCLK/2=ERCLK32/3=OSCERCLK */
    LPTMR0_CMR = LPTMR_CMR_COMPARE(660); /* Set compare value for 20 ms */
  #endif

  LPTMR0_CSR =(  LPTMR_CSR_TCF_MASK    /* Clear any pending interrupt */
               | LPTMR_CSR_TIE_MASK    /* LPTMR interrupt enabled */
               | LPTMR_CSR_TPS(0)      /* TMR pin select */
              );
  /* Set to LPTMR lower interrupt priority, because it corrupt UART0 comm in TSS SW trigger mode*/
  NVIC_SetPriority(INT_LPTimer - 16, 0x7Fu);
  /* LPTMR vector enable */
  enable_irq(INT_LPTimer - 16);
  /* Turn on LPT and start counting */
  LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;
}

/***************************************************************************//*!
*
* @brief  LPTMR Timer Stop for Auto Trigger Mode function
*
* @param  void
*
* @return void
*
* @remarks
*
****************************************************************************/
void LPTMRStop(void)
{
  SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;
  disable_irq(INT_LPTimer - 16);    /* LPTMR vector disable */
  LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;  /* Turn off LPTMR and stoprt counting */
}

/***************************************************************************//*!
*
* @brief  LPTMR Timer Isr function
*
* @param  void
*
* @return void
*
* @remarks
*
****************************************************************************/
void LPTMRIsr(void)
{
  LPTMR0_CSR |=  LPTMR_CSR_TCF_MASK;   /* Clear the LPT timer compare flag */
  LPTMR0_CSR = ( LPTMR_CSR_TEN_MASK    /* Enable timer */
                 | LPTMR_CSR_TIE_MASK  /* Enable interrupts */
                 | LPTMR_CSR_TCF_MASK  /* Clear the flag */
               );
  /* Workaround which solving issue with TSI errata on L family with clearing of EOSF */
  if((TSS_GetSystemConfig(System_SystemConfig_Register) & TSS_LOWPOWER_EN_MASK) == TSS_LOWPOWER_EN_MASK)
  {
     if (TSI0_GENCS & TSI_GENCS_EOSF_MASK)
     {
       /* Clera EOSF flag */
       TSI0_GENCS |= TSI_GENCS_EOSF_MASK;
     }
  }
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
  /* Call application callback for Low Power Enter */
  LowPowerEnter_Callback(TRUE);
  /* Enter to xLLSn -> TSI Low Power */
  SMC_PMCTRL = SMC_PMCTRL_STOPM(u8LowPowerType);
  /* Do periodic wake up by LPTRM as Workaround which
     solving issue with TSI errata on L family with clearing of TSI EOSF */
  psLLWUStruct->ME |= LLWU_ME_WUME0_MASK;        /* LPTMR LLWU Input Enable = LLWU_M0IF) */
  do {
    MCG_C6 &= ~MCG_C6_CME0_MASK; /* Disable MCG clock monitor */
    stop();  /* Goto STOP with SLEEPDEEP enabled */
    MCG_C1 &= ~MCG_C1_CLKS_MASK; /* CLKS=0, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0 */
    MCG_C6 |= MCG_C6_CME0_MASK; /* Enable MCG clock monitor */
  } while ((TSS_GetSystemConfig(System_SystemConfig_Register) & TSS_LOWPOWER_EN_MASK) == TSS_LOWPOWER_EN_MASK);
  psLLWUStruct->ME &= ~LLWU_ME_WUME0_MASK;        /* LPTMR LLWU Input Disable = LLWU_M0IF) */
  psLLWUStruct->ME &= ~LLWU_ME_WUME4_MASK;        /* TSI LLWU Input Disable = LLWU_M4IF) */
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
    PORTB_PCR14 |= PORT_PCR_IRQC(0x0A) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; /* Set Interrupt on Falling Edge */
    enable_irq(INT_PORTD - 16); /* Enable LPSW intr */
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
  PORTB_PCR14 |= PORT_PCR_ISF_MASK; /* Reset Intr Flag */
  PORTB_PCR14 &= ~PORT_PCR_IRQC_MASK; /* Disable Interrupt on Falling Edge */
  disable_irq(INT_PORTD - 16); /* Disable LPSW intr */
  LPSWflag = 1u; /* Set LPSW flag */
}

/***************************************************************************
* BoardSpecificSettings
* --------------------------------------------------------------------------
* Board-specific initialization after app general settings
****************************************************************************/
void BoardSpecificInit(void)
{
#if (TSS_USE_FREEMASTER_GUI == 1)
  uint8_t u8Temp;
  /* Disable electrodes which have confict with UART0 (PTB2/TSI_CH2, PTB1/TSI_CH3) */
  u8Temp = TSS_GetSystemConfig(System_ElectrodeEnablers_Register+0);
  TSS_SetSystemConfig(System_ElectrodeEnablers_Register+0, u8Temp & ~0x0Cu );
  u8Temp = TSS_GetSystemConfig(System_ElectrodeEnablers_Register+1);
  TSS_SetSystemConfig(System_ElectrodeEnablers_Register+1, u8Temp & ~0xC0u );
  u8Temp = TSS_GetSystemConfig(System_ElectrodeEnablers_Register+3);
  TSS_SetSystemConfig(System_ElectrodeEnablers_Register+3, u8Temp & ~0x0Cu );
  u8Temp = TSS_GetSystemConfig(System_ElectrodeEnablers_Register+4);
  TSS_SetSystemConfig(System_ElectrodeEnablers_Register+4, u8Temp & ~0x01u );
#endif
}

/***************************************************************************/
