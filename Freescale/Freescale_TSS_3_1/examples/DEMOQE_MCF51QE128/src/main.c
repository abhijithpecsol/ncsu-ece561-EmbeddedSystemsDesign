/**HEADER********************************************************************
* 
* Copyright (c) 2008-2011 Freescale Semiconductor;
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
* $FileName: main.c$
* $Version : 3.6.18.0$
* $Date    : Dec-10-2012$
*
* Comments:
*
*   This file contains the source for the TSS example program.
*
*END************************************************************************/

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#include "TSS_API.h"
#include "I2CDrv.h"     /* Include EGT communication software */
#include "freemaster.h"

/* LED Control Macros */
#define LED(x,y,z)  { PT##x##D_PT##x##D##y = z;PT##x##DD_PT##x##DD##y = 1; }

#define LED1_ON     LED(C,4,0)
#define LED1_OFF    LED(C,4,1)
#define LED2_ON     LED(C,5,0)
#define LED2_OFF    LED(C,5,1)
#define LED3_ON     LED(E,6,0)
#define LED3_OFF    LED(E,6,1)
#define LED4_ON     LED(E,7,0)
#define LED4_OFF    LED(E,7,1)

/********************************* Prototypes ********************************/

void TSS_fOnInit(void);
void MCU_Init(void);
void TSS_fOnFault(void);
void fCallBack0(TSS_CONTROL_ID u8ControlId);
void uart_init(int baud);

#if TSS_USE_FREEMASTER_GUI
void FreeMASTER_Init(void);
#endif

/*********************** GUI - FreeMASTER TSA table ************************/

#if FMSTR_USE_TSA
    /* Example of FreeMASTER User Application Table */
    uint8_t u8Dummy;
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

/***************************************************************************************************
* TSS_fOnInit
* --------------------------------------------------------------------------------------------------
* TSS OnInit Callback
****************************************************************************************************/

void TSS_fOnInit(void) 
{
    /* Include your TSS Init code here */  
}

/***************************************************************************************************
* MCU_Init
* --------------------------------------------------------------------------------------------------
* Initializes various registers and peripherals of MCU
****************************************************************************************************/

void MCU_Init(void)
{
  /* Common initialization of the write once registers */
  /* SOPT1: COPE=0,COPT=1,STOPE=0,WAITE=1,RSTOPE=0,BKGDPE=1,RSTPE=0 */
  SOPT1 = 0x52;                                      
  /* SOPT2: COPCLSK=0,SPI1PS=0,ACIC2=0,IICPS=1,ACIC1=0 */
  SOPT2 = 0x02;                                      
  /* SPMSC1: LVDF=0,LVDACK=0,LVDIE=0,LVDRE=1,LVDSE=1,LVDE=1,BGBE=0 */
  SPMSC1 = 0x1C;                                      
  /* SPMSC2: LPR=0,LPRS=0,LPWUI=0,PPDF=0,PPDACK=0,PPDE=1,PPDC=0 */
  SPMSC2 = 0x02;                                      
  /* SPMSC3: LVDV=0,LVWV=0,LVWIE=0 */
  SPMSC3 &= (unsigned char)~0x38;                     
  /* Initialization of CPU registers */
  asm {
    /* VBR: ADDRESS=0 */
    clr.l d0
    movec d0,VBR
    /* CPUCR: ARD=0,IRD=0,IAE=0,IME=0,BWD=0,FSD=0 */
    clr.l d0
    movec d0,CPUCR
  }
  /*  System clock initialization */
  if (*(unsigned char*far)0x03FF != 0xFF) { /* Test if the device trim value is stored on the specified address */
    ICSTRM = *(unsigned char*far)0x03FF; /* Initialize ICSTRM register from a non volatile memory */
    ICSSC = (unsigned char)((*(unsigned char*far)0x03FE) & (unsigned char)0x01); /* Initialize ICSSC register from a non volatile memory */
  }
  /* ICSC1: CLKS=0,RDIV=0,IREFS=1,IRCLKEN=0,IREFSTEN=0 */
  ICSC1 = 0x04;                        /* Initialization of the ICS control register 1 */
  #if TSS_USE_CTS_LOW_LEVEL /* 8 MHz */
    /* ICSC2: BDIV=0,RANGE=0,HGO=0,LP=0,EREFS=0,ERCLKEN=0,EREFSTEN=0 */
    ICSC2 = 0x00;                        /* Initialization of the ICS control register 2 */
    while(!ICSSC_IREFST) {               /* Wait until the source of reference clock is internal clock */
    }
    /* ICSSC: DRST_DRS=0,DMX32=0 */
    ICSSC &= (unsigned char)~0xE0;       /* Initialization of the ICS status and control */
    while((ICSSC & 0xC0) != 0x00) {      /* Wait until the FLL switches to Low range DCO mode */
    }
  #else /* 25 MHz */
    /* ICSC2: BDIV=0,RANGE=0,HGO=0,LP=0,EREFS=0,ERCLKEN=0,EREFSTEN=0 */
    ICSC2 = 0x00;                        /* Initialization of the ICS control register 2 */
    while(!ICSSC_IREFST) {               /* Wait until the source of reference clock is internal clock */
    }
    /* ICSSC: DRST_DRS=2,DMX32=0 */
    ICSSC = (unsigned char)((ICSSC & (unsigned char)~0x60) | (unsigned char)0x80); /* Initialization of the ICS status and control */
    while((ICSSC & 0xC0) != 0x80) {      /* Wait until the FLL switches to High range DCO mode */
    }
  #endif
  /* INTC_WCR: ENB=0,MASK=0 */
  INTC_WCR = 0x00;  
  /* SCGC1: TPM3=1,TPM2=1,TPM1=1,ADC=1,IIC2=1,IIC1=1,SCI2=1,SCI1=1 */
  SCGC1 = 0xFF;                                      
  /* SCGC2: FLS=1,IRQ=1,KBI=1,ACMP=1,RTC=1,SPI2=1,SPI1=1 */
  SCGC2 = 0xFF; 

  /* ### */
  asm {                                /* Set Interrupt level 0 */
    move.w SR,D0;
    andi.l #0xF8FF,D0;
    move.w D0,SR;
  }                                    
  
}


/****************************************************************************************************
* Fault interrupt (software interrupt)
* ---------------------------------------------------------------------------------------------------
* Re-enables the larger electrode in case is disabled (This electrode has a very large capacitance)
*****************************************************************************************************/

void TSS_fOnFault(void)
{
    if(tss_CSSys.Faults.ChargeTimeout || tss_CSSys.Faults.SmallCapacitor) /* If large or small capacitor fault  */
    {    
      asm (NOP);
      
      (void)TSS_SetSystemConfig(System_Faults_Register,0x00);         /* Clear the fault flag               */
      (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register,0xFF);  /* re-enable all electrodes  */
    }
}

/****************************************************************************************************
* fCallBack0
* ---------------------------------------------------------------------------------------------------
* Executes every time an event is reported by the TSS
*****************************************************************************************************/

void fCallBack0(TSS_CONTROL_ID u8ControlId)
{
  uint8_t u8ElecNum; /* 8 bits local variable used to store the event information */
  
  while (!TSS_KEYPAD_BUFFER_EMPTY(cKey0))   /* While unread events are in the buffer */
  {
    TSS_KEYPAD_BUFFER_READ(u8ElecNum,cKey0);/* Read the buffer and store the electrode number in u8ElecNum variable */
    
    if(u8ElecNum & 0x80)                    /* If the event was a release */
    {
      u8ElecNum = (uint8_t) (u8ElecNum & 0x0F);  /* Remove the release flag */
      
      if ((u8ElecNum == 0x00) || (u8ElecNum == 0x04)) LED1_OFF;
      if ((u8ElecNum == 0x01) || (u8ElecNum == 0x05)) LED2_OFF;
      if ((u8ElecNum == 0x02) || (u8ElecNum == 0x06)) LED3_OFF;
      if ((u8ElecNum == 0x03) || (u8ElecNum == 0x07)) LED4_OFF;
    }
    else                                    /* If the event was a touch */
    {
      if ((u8ElecNum == 0x00) || (u8ElecNum == 0x04)) LED1_ON;
      if ((u8ElecNum == 0x01) || (u8ElecNum == 0x05)) LED2_ON;
      if ((u8ElecNum == 0x02) || (u8ElecNum == 0x06)) LED3_ON;
      if ((u8ElecNum == 0x03) || (u8ElecNum == 0x07)) LED4_ON;
    }
  }
  
  (void) u8ControlId;
  return;
}

/**************************************************************************//*!
* Uart 1 initialization
******************************************************************************/
void uart_init(int baud)
{

#if TSS_USE_CTS_LOW_LEVEL
  SCI1BD = (uint16_t)(8000000/(baud*16));
#else
  SCI1BD = (uint16_t)(25000000/(baud*16));
#endif
}


/**************************************************************************//*!
* FreeMASTER_Init
* --------------------------------------------------------------------------
* Initialize FreeMASTER resources
******************************************************************************/

#if TSS_USE_FREEMASTER_GUI
void FreeMASTER_Init(void)
{
   /* Fmstr UART Init */
   uart_init (57600);
  
  /* Initialize FMSTR stack */
  FMSTR_Init();
}
#endif

/****************************************************************************************************
* Main
* ---------------------------------------------------------------------------------------------------
* Main loop
*****************************************************************************************************/

void main(void) 
{
 uint8_t i;
 
 MCU_Init();

 I2CInit();  /* Initializes the EGT module */
 
#if TSS_USE_FREEMASTER_GUI
 FreeMASTER_Init(); /* Init FreeMASTER resources */
#endif 
 EnableInterrupts; /* enable interrupts */

 (void)TSS_Init(); /* Initializes the TSS */
 
 (void)TSS_SetSystemConfig(System_NSamples_Register,0x0C);
 
 /* Configure Sensitivities */
 for(i = 0; i < TSS_N_ELECTRODES; i++)
 {
   (void)TSS_SetSystemConfig(System_Sensitivity_Register+i,0x3F);
 }
   
 /* Configure the TSS to report the touch and release events */
 (void)TSS_KeypadConfig(cKey0.ControlId,Keypad_Events_Register,TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK);

 /* Enables Callback function. Enables the control */
 (void)TSS_KeypadConfig(cKey0.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK | TSS_KEYPAD_CONTROL_EN_MASK));

  /* Enables the TSS. Enables DC Tracker. */
 (void)TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK | TSS_DC_TRACKER_EN_MASK));

  for(;;) {

    /* FreeMASTER */
#if TSS_USE_FREEMASTER_GUI
    FMSTR_Poll();
#endif
    if (TSS_Task() == TSS_STATUS_OK)     /* Executes the TSS main task */
    {
      EGTTask();          /* Executes the EGT main task */
    }
    
    __RESET_WATCHDOG(); /* by default, COP is disabled with device init. When enabling, also reset the watchdog. */
   
  } /* loop forever */
  /* please make sure that you never leave main */
}
