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
*   This file contains the init functions.
*
*END************************************************************************/

#include "TSS_API.h"
#include "wdog.h"
#include "uart.h"
#include "module_id.h"
#include "freemaster.h"
#include "irq_common.h"
#include "main.h"
#include "events.h"

#include "app_init.h"

/***************************** Local Types *********************************/

uint16_t u16ProximityElectrodeTimeout = APP_TWRPI_PROXIMITY_TIMEOUT;
extern uint8_t PeriodicTimerFlag;

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
  /*  Leave I2C3 I2C2 I2C1 I2C0 SPI1 SPI0 UART1 clocks off UART0 Clocks on */
  SIM_SCGC1 |= SIM_SCGC1_UART0_MASK;
  /*  Leave I2S ANL TSI  ADC DAC12B clocks off VREF Clocks on*/
  SIM_SCGC2 |= SIM_SCGC2_VREF_MASK;
  /*  Leave CRC PDB CMT MTIM FTM1 FTM0 clocks off */
  SIM_SCGC3 |= 0x00;
  /*  Turn on IRQ clocks, leave FTFL DMA WDOG clocks off */
  SIM_SCGC4 |= SIM_SCGC4_IRQ_MASK;
  /*   RNGB MFBUS OSC2 OSC1 MCG clocks off */
  SIM_SCGC5 |= 0x00;
  /*  Leave PORTF PORTE PORTD PORTC PORTB PORTA on and USBOTG USBDCD  clocks off */
  SIM_SCGC6 |= SIM_SCGC6_PORTA_MASK |SIM_SCGC6_PORTB_MASK | SIM_SCGC6_PORTC_MASK| SIM_SCGC6_PORTD_MASK |SIM_SCGC6_PORTE_MASK | SIM_SCGC6_PORTF_MASK;
  /* LED's Init */
  MXC_PTAPF4 = ((MXC_PTAPF4 & 0xF0) | 0x01);//Select PTA0 GPIO
  MXC_PTCPF2 = ((MXC_PTCPF2 & 0x0F) | 0x10);//Select PTC5 GPIO
  PTA_DD = 0x01; //Configure PTA0 as output
  PTC_DD = 0x20; //configure PTC5 as output
  LED1_OFF;LED2_OFF;
  /* Set SW1 for button which enters to Low Power mode */
  configure_irq_for_rising_edge();
}

/**
 * \brief TWRPI board initialization function
 *
 * \param void
 *
 * \return void
 */
void TSS_Init_TWRPI_Board(TWRPI_DEVICES eTWRPI_module)
{
  uint8_t u8temp;

  TSS_MTIMStop();

  DelayMS(300, FALSE); /* For Signal Stabilization */
  (void) TSS_Init();

  /* Set Number of Samples */
  (void) TSS_SetSystemConfig(System_NSamples_Register, 0x08);
#if (TSS_USE_AUTO_SENS_CALIBRATION == 0)
  /* Sets the Sensitivity value for each electrode */
  for (u8temp =0; u8temp < TSS_N_ELECTRODES; u8temp++) {
    (void) TSS_SetSystemConfig(System_Sensitivity_Register + u8temp, 0x40);
  }
#endif

  /* Low Power Config */
  (void) TSS_SetSystemConfig(System_LowPowerElectrode_Register, 0u);
  (void) TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x40);
  /* Auto Trigger Config */
#if (APP_TSS_USE_TRIGGER_MODE == AUTO)
  (void) TSS_SetSystemConfig(System_SystemTrigger_Register, TSS_TRIGGER_MODE_AUTO);   /* TSS_TRIGGER_MODE_AUTO/TSS_TRIGGER_MODE_ALWAYS/TSS_TRIGGER_MODE_SW */
#elif (APP_TSS_USE_TRIGGER_MODE == ALWAYS)
  (void) TSS_SetSystemConfig(System_SystemTrigger_Register, TSS_TRIGGER_MODE_ALWAYS); /* TSS_TRIGGER_MODE_AUTO/TSS_TRIGGER_MODE_ALWAYS/TSS_TRIGGER_MODE_SW */
#elif (APP_TSS_USE_TRIGGER_MODE == SW)
  (void) TSS_SetSystemConfig(System_SystemTrigger_Register, TSS_TRIGGER_MODE_SW);   /* TSS_TRIGGER_MODE_AUTO/TSS_TRIGGER_MODE_ALWAYS/TSS_TRIGGER_MODE_SW */
#endif

  /* Set DC-Tracker Rates */
  (void) TSS_SetSystemConfig(System_DCTrackerRate_Register, 0x64);
  (void) TSS_SetSystemConfig(System_SlowDCTrackerFactor_Register, 0x0F);  
  
  /* Disable all controls */
  (void) TSS_SetKeypadConfig(cKey0.ControlId, Keypad_ControlConfig_Register, 0x00);
  (void) TSS_SetKeypadConfig(cKey1.ControlId, Keypad_ControlConfig_Register, 0x00);
  (void) TSS_SetKeypadConfig(cKey2.ControlId, Keypad_ControlConfig_Register, 0x00);
  (void) TSS_SetRotaryConfig(cRotary3.ControlId, Rotary_ControlConfig_Register, 0x00);
  (void) TSS_SetASliderConfig(cASlider4.ControlId, ASlider_ControlConfig_Register, 0x00);
  (void) TSS_SetKeypadConfig(cKey5.ControlId, Keypad_ControlConfig_Register, 0x00);
  (void) TSS_SetARotaryConfig(cARotary6.ControlId, ARotary_ControlConfig_Register, 0x00);
  (void) TSS_SetKeypadConfig(cKey7.ControlId, Keypad_ControlConfig_Register, 0x00);
  (void) TSS_SetKeypadConfig(cKey8.ControlId, Keypad_ControlConfig_Register, 0x00);
  (void) TSS_SetKeypadConfig(cKey9.ControlId, Keypad_ControlConfig_Register, 0x00);
  (void) TSS_SetKeypadConfig(cKey10.ControlId, Keypad_ControlConfig_Register, 0x00);
  (void) TSS_SetKeypadConfig(cKey11.ControlId, Keypad_ControlConfig_Register, 0x00);

  switch (eTWRPI_module)
  {
    case TWRPI_UNKNOWN:
    case TWRPI_UNPLUGGED:
      /* Set Sensitivity */
#if (TSS_USE_AUTO_SENS_CALIBRATION == 0)
      /* Sets the Sensitivity value for each electrode */
      for (u8temp = 0; u8temp < TSS_N_ELECTRODES; u8temp++)
      {
        (void) TSS_SetSystemConfig(System_Sensitivity_Register + u8temp, 0x40);
      }
#endif

      /* Low Power Config */
      u8temp = TSS_GetControlElectrodeIndex(cKey0.ControlId.ControlNumber, 0u);
      (void) TSS_SetSystemConfig(System_LowPowerElectrode_Register, u8temp);
      (void) TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x40);
      /* Configure Control 1 */
      /* Configure the TSS Keypad Control to report the touch and release events */
      (void) TSS_SetKeypadConfig(cKey0.ControlId,Keypad_Events_Register,(TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK));
      /* Enables Callback function. Enables the control */
      (void) TSS_SetKeypadConfig(cKey0.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK | TSS_KEYPAD_CONTROL_EN_MASK));
      break;
    case TWRPI_KEYPAD:
      /* Low Power Config */
      u8temp = TSS_GetControlElectrodeIndex(cKey1.ControlId.ControlNumber, 1u);
      (void) TSS_SetSystemConfig(System_LowPowerElectrode_Register, u8temp);
      (void) TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x1A);
      /* Configure Control 2 */
      /* Configure the TSS Keyapd Control to report the touch and release events */
      (void) TSS_SetKeypadConfig(cKey1.ControlId,Keypad_Events_Register,(TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK));
      /* Enables Callback function. Enables the control */
      (void) TSS_SetKeypadConfig(cKey1.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK | TSS_KEYPAD_CONTROL_EN_MASK));
      break;
    case TWRPI_ROTARY:
      /* Low Power Config */
      u8temp = TSS_GetControlElectrodeIndex(cKey2.ControlId.ControlNumber, 0u);
      (void) TSS_SetSystemConfig(System_LowPowerElectrode_Register, u8temp);
      (void) TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x1A);

      /* Configure Control 3 */
      /* Configure the TSS Keypad Control to report the touch and release events */
      (void) TSS_SetKeypadConfig(cKey2.ControlId,Keypad_Events_Register,(TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK));
      /* Enables Callback function. Enables the control */
      (void) TSS_SetKeypadConfig(cKey2.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK | TSS_KEYPAD_CONTROL_EN_MASK));
      /* Configure Control 4 */
      /* Configure the TSS to report the touch, release and movement events */
      (void) TSS_SetRotaryConfig(cRotary3.ControlId,Rotary_Events_Register,TSS_ROTARY_MOVEMENT_EVENT_EN_MASK);
      /* Enables Callback function. Enables the control */
      (void) TSS_SetRotaryConfig(cRotary3.ControlId,Rotary_ControlConfig_Register,(TSS_ROTARY_CONTROL_EN_MASK | TSS_ROTARY_CALLBACK_EN_MASK));
      break;
    case TWRPI_SLIDER:
      /* Low Power Config */
      u8temp = TSS_GetControlElectrodeIndex(cKey5.ControlId.ControlNumber, 0u);
      (void) TSS_SetSystemConfig(System_LowPowerElectrode_Register, u8temp);
      (void) TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x1A);

      /* Configure Control 5 */
      /* Configure the range */
      (void) TSS_SetASliderConfig(cASlider4.ControlId,ASlider_Range_Register, 4);
      /* Configure the TSS to report the touch, release and movement events */
      (void) TSS_SetASliderConfig(cASlider4.ControlId,ASlider_Events_Register,(TSS_ASLIDER_MOVEMENT_EVENT_EN_MASK | TSS_ASLIDER_INITIAL_TOUCH_EVENT_EN_MASK));
      /* Enables Callback function. Enables the control */
      (void) TSS_SetASliderConfig(cASlider4.ControlId,ASlider_ControlConfig_Register,(TSS_ASLIDER_CONTROL_EN_MASK|TSS_ASLIDER_CALLBACK_EN_MASK));
      /* Configure Control 6 */
      /* Configure the TSS Keyapd Control to report the touch and release events */
      (void) TSS_SetKeypadConfig(cKey5.ControlId,Keypad_Events_Register,(TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK));
      /* Enables Callback function. Enables the control */
      (void) TSS_SetKeypadConfig(cKey5.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK | TSS_KEYPAD_CONTROL_EN_MASK));
      break;
    case TWRPI_ROTARY2:
      /* Low Power Config */
      u8temp = TSS_GetControlElectrodeIndex(cKey7.ControlId.ControlNumber, 0u);
      (void) TSS_SetSystemConfig(System_LowPowerElectrode_Register, u8temp);
      (void) TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x1A);
      /* Configure Control 7 */
      /* Configure the range */
      (void) TSS_SetASliderConfig(cARotary6.ControlId,ARotary_Range_Register, 32);
      /* Configure the TSS to report the touch, release and movement events */
      (void) TSS_SetASliderConfig(cARotary6.ControlId,ARotary_Events_Register,(TSS_AROTARY_MOVEMENT_EVENT_EN_MASK | TSS_AROTARY_INITIAL_TOUCH_EVENT_EN_MASK));
      /* Enables Callback function. Enables the control */
      (void) TSS_SetASliderConfig(cARotary6.ControlId,ARotary_ControlConfig_Register,(TSS_AROTARY_CONTROL_EN_MASK|TSS_AROTARY_CALLBACK_EN_MASK));
      /* Configure Control 8 */
      /* Configure the TSS Keyapd Control to report the touch and release events */
      (void) TSS_SetKeypadConfig(cKey7.ControlId,Keypad_Events_Register,(TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK));
      /* Enables Callback function. Enables the control */
      (void) TSS_SetKeypadConfig(cKey7.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK | TSS_KEYPAD_CONTROL_EN_MASK));
      break;
    case TWRPI_GKEYPAD:
      /* Low Power Config */
      u8temp = TSS_GetControlElectrodeIndex(cKey8.ControlId.ControlNumber, 3u);
      TSS_SetSystemConfig(System_LowPowerElectrode_Register, u8temp);
      (void) TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x1A);
      /* Configure Control 9 */
      /* Configure the TSS Keyapd Control to report the touch and release events */
      (void) TSS_SetKeypadConfig(cKey8.ControlId, Keypad_Events_Register, (TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK));
      /* Enables Callback function. Enables the control */
      (void) TSS_SetKeypadConfig(cKey8.ControlId, Keypad_ControlConfig_Register, (TSS_KEYPAD_CALLBACK_EN_MASK | TSS_KEYPAD_CONTROL_EN_MASK));
      break;
#if (TSS_USE_KEYDETECTOR_VERSION == 1)      
    case TWRPI_SHIELD1:
      /* Low Power Config */
      u8temp = TSS_GetControlElectrodeIndex(cKey9.ControlId.ControlNumber, 1u);
      (void) TSS_SetSystemConfig(System_LowPowerElectrode_Register, u8temp);
      (void) TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x1A);
      /* Configure Control 10 */
      /* Configure the TSS Keypad Control to report the touch and release events */
      (void) TSS_SetKeypadConfig(cKey9.ControlId, Keypad_Events_Register, (TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK));
      /* Enables Callback function. Enables the control */
      (void) TSS_SetKeypadConfig(cKey9.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK | TSS_KEYPAD_CONTROL_EN_MASK));
      break;
    case TWRPI_SHIELD2:
      /* Low Power Config */
      u8temp = TSS_GetControlElectrodeIndex(cKey10.ControlId.ControlNumber, 1u);
      (void) TSS_SetSystemConfig(System_LowPowerElectrode_Register, u8temp);
      (void) TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x1A);

      /* Configure Control 12 */
      /* Configure the TSS Keypad Control to report the touch and release events */
      (void) TSS_SetKeypadConfig(cKey10.ControlId, Keypad_Events_Register, (TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK));
      /* Enables Callback function. Enables the control */
      (void) TSS_SetKeypadConfig(cKey10.ControlId, Keypad_ControlConfig_Register, (TSS_KEYPAD_CALLBACK_EN_MASK | TSS_KEYPAD_CONTROL_EN_MASK));
      break;
#endif      
    case TWRPI_PROXIMITY:
      /* Low Power Config */
      u8temp = TSS_GetControlElectrodeIndex(cKey11.ControlId.ControlNumber, 4u);
      TSS_SetSystemConfig(System_LowPowerElectrode_Register, u8temp);
      TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x30);
      /* Configure Control 12 */
      /* Configure the TSS Keypad Control to report the touch and release events */
      (void) TSS_SetKeypadConfig(cKey11.ControlId,Keypad_Events_Register,(TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK));
      /* Enables Callback function. Enables the control */
      (void) TSS_SetKeypadConfig(cKey11.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK | TSS_KEYPAD_CONTROL_EN_MASK));
      break;
  }
  
#if APP_TSS_USE_DCTRACKER
  if (eTWRPI_module == TWRPI_SHIELD1)
  {
    /* Enables the TSS. Enables the DC Tracking feature. Default DC Tracking value is 0x64. Enables Water Tolerance */
    (void) TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK | TSS_DC_TRACKER_EN_MASK | TSS_WATER_TOLERANCE_EN_MASK));
  }
  else if (eTWRPI_module == TWRPI_PROXIMITY)
  {
    /* Enables the TSS. Enables the DC Tracking feature. Default DC Tracking value is 0x64. Enables Proximity */
    (void) TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK | TSS_DC_TRACKER_EN_MASK | TSS_PROXIMITY_EN_MASK));
    /* Proximity Enter Indication */
    DoLedBlinking(50u, 5u, FALSE);
  }
  else
  {
    /* Enables the TSS. Enables the DC Tracking feature. Default DC Tracking value is 0x64 */
    (void)TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK | TSS_DC_TRACKER_EN_MASK));
  }
#else
  if (eTWRPI_module == TWRPI_SHIELD1)
  {
    /* Enables the TSS and Water Tolerance */
    (void) TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK | TSS_WATER_TOLERANCE_EN_MASK));
  }
  else if (eTWRPI_module == TWRPI_PROXIMITY)
  {
    /* Enables the TSS. Enables Proximity */
    (void) TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK | TSS_PROXIMITY_EN_MASK));
    /* Proximity Enter Indication */
    DoLedBlinking(50u, 5u, FALSE);
  }
  else
  {
    /* Enables the TSS */
    (void) TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK));
  }
#endif  
  
TSS_MTIMInit();
}

/**************************************************************************//*!
* FreeMASTER_Init
* --------------------------------------------------------------------------
* Initialize FreeMASTER resources
******************************************************************************/
void FreeMASTER_Init(void)
{
  /* UART0 and UART1 are clocked from the core clock, but all other UARTs are
   * clocked from the peripheral clock. So we have to determine which clock
   * to send to the uart_init function.
   */
  /* Fmstr UART Init */
  uart_init(FMSTR_UART_PORT, CORE_CLK_KHZ, FMSTR_UART_BAUD);

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
  sim->SCGC3 |= SIM_SCGC3_FTM0_MASK;  /* FTM0 clock enable */
  FTM0_SC = 0u;
  FTM0_CNT = 0x0000U;
  FTM0_SC |= FTM_SC_PS(7);
  FTM0_MOD = 190u;                    /* 1ms@50MHz */
  FTM0_SC |= 0x08U;

  for (i=0; i < u16delay; i++)
  {
    u16timeout = 0u;
    do {
      if (u8CallbackEnabler) DelayMS_Callback();
      u16timeout++;
    } while (!(FTM0_SC & FTM_SC_TOF_MASK) && (u16timeout < 0x7FFFu));
    FTM0_SC &= ~FTM_SC_TOF_MASK;      /* Clear Flag */
    FTM0_CNT = 0x0000U;
  }

  FTM0_SC = 0u;

}


/***************************************************************************//*!
*
* @brief  MTIM Timer Init for Auto Trigger Mode function
*
* @param  void
*
* @return void
*
* @remarks
*
****************************************************************************/
void TSS_MTIMInit(void)
{
  SIM_SCGC3 |= SIM_SCGC3_MTIM_MASK;    	/* MTIM clock enable */

  MTIM0_SC |= MTIM_SC_TSTP_MASK;		  	/* Counter stopped */
  MTIM0_SC |= MTIM_SC_TRST_MASK;		  	/* Counter reset 0x0000 */
  MTIM0_CLK |= MTIM_CLK_CLKS(0) | MTIM_CLK_PS(5);	/* Clock source = BUSclock , Prescaler = 16 */
  MTIM0_MOD = MTIM_MOD_MOD(15625);       /* Modulo register = 15625 ~ 20ms */
  MTIM0_SC  |= MTIM_SC_TOIE_MASK;		/* Overflow interrupt enable */
  MTIM0_SC &= ~MTIM_SC_TSTP_MASK;		/* Counter is active */
}

/***************************************************************************//*!
*
* @brief  MTIM Timer Stop for Auto Trigger Mode function
*
* @param  void
*
* @return void
*
* @remarks
*
****************************************************************************/
void TSS_MTIMStop(void)
{
  MTIM0_SC  |= MTIM_SC_TSTP_MASK;
}

/***************************************************************************//*!
*
* @brief  MTIM Timer Isr function
*
* @param  void
*
* @return void
*
* @remarks
*
****************************************************************************/
interrupt void TSS_MTIMIsr(void)
{
  /* Clears the MTIM interrupt flag */
  MTIM0_SC;							/* Read SC register while TOF = 1 */
  MTIM0_SC &= ~MTIM_SC_TOF_MASK; 	/* Clear TOF */
  /* Call application callback */
  PeriodicTimer_Callback();
}
  
/**************************************************************************//*!
* PeriodicTimerControl
* --------------------------------------------------------------------------
* Control function periodic timer
******************************************************************************/
void PeriodicTimerControl(void)
{
  if (PeriodicTimerFlag)
  {
  #if APP_TSS_USE_DCTRACKER
    if (MODULE_ID_Get_Device(MODULE_A) == TWRPI_PROXIMITY)
    {
      if (u16ProximityElectrodeTimeout == 0u)
      {
        if ((TSS_GetSystemConfig(System_SystemConfig_Register) & TSS_PROXIMITY_EN_MASK) != TSS_PROXIMITY_EN_MASK)
        {
          /* Enables the TSS. Enables the DC Tracking feature. Default DC Tracking value is 0x64. Enables Proximity */
          (void) TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK | TSS_DC_TRACKER_EN_MASK | TSS_PROXIMITY_EN_MASK));
          /* Proximity Enter Indication */
          DoLedBlinking(50u, 5u, TRUE);
        }
      }
      else
      {
        u16ProximityElectrodeTimeout--;
      }
    }
  #else
    if (MODULE_ID_Get_Device(MODULE_A) == TWRPI_PROXIMITY)
    {
      if (u16ProximityElectrodeTimeout == 0u)
      {
        if ((TSS_GetSystemConfig(System_SystemConfig_Register) & TSS_PROXIMITY_EN_MASK) != TSS_PROXIMITY_EN_MASK)
        {
          /* Enables the TSS. Enables Proximity */
          (void) TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK | TSS_PROXIMITY_EN_MASK));
          /* Proximity Enter Indication */
          DoLedBlinking(50u, 5u, TRUE);
        }
      }
      else
      {
        u16ProximityElectrodeTimeout--;
      }
    }
  #endif
    PeriodicTimerFlag = 0u;
  }
}


/**************************************************************************//*!
* DoLedBlinking
* --------------------------------------------------------------------------
* Perform blinking of all LEDs in defined period
******************************************************************************/
void DoLedBlinking(uint16_t u16Period, uint8_t u8Loops, uint8_t u8TSSEnabler)
{
  uint8_t u8temp;
  /* Indication */
  for (u8temp = 0u; u8temp < u8Loops; u8temp++)
  {
    LED1_ON;LED2_ON;
    DelayMS(u16Period, u8TSSEnabler);
    LED1_OFF; LED2_OFF;
    DelayMS(u16Period, u8TSSEnabler);
  }
}


