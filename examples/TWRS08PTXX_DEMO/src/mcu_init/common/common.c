/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2011-2012 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file common.c
*
* @author a13984
*
* @version 0.0.4.0
*
* @date May-22-2012
*
* @brief provide common system initialization routine,terminal I/O initialization,
*        and some common utilities. 
*
*******************************************************************************
*
* provide common system initialization routine,terminal I/O initialization,
*        and some common utilities
*
******************************************************************************/

#include "common.h"
#include "ics.h"
#include "wdog.h"

/******************************************************************************
* External objects
******************************************************************************/

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/

/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/

/******************************************************************************
* Local functions
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/

/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: Sys_Init
*
* @brief initialize the MCU and the whole system.
*        
* @param  none 
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

void Sys_Init(void)
{
  /* Disable the watchdog timer */
  /* WARNINGIN: DO NOT single step this function call, otherwise
   * will cause watch dog reset
   */
  wdog_disable();
  /*
  /////////////////////////////////////////////////////////////////////////////////////////
  // Peripheral Init
  // --------------------------------------------------------------------------------------
  /////////////////////////////////////////////////////////////////////////////////////////
  */
  /* Enable BKGD pin, RESET pin, and enable STOP mode */
  /* NOTE: please make sure other register bits are also write-once and
   * need add other bit mask here if needed.
   */
   SYS_SOPT1 = SYS_SOPT1_BKGDPE_MASK | SYS_SOPT1_RSTPE_MASK | SYS_SOPT1_STOPE_MASK;
   
  /* output bus clock if enabled on PTH2 pin */
#if defined(OUTPUT_BUSCLK)
  SYS_SOPT3 |= SYS_SOPT3_CLKOE_MASK;
#endif
  
#if defined(USE_FEE)  
  FEI_to_FEE(); /* Initialize system clock to 16M with external 8M crystal*/
  //FEI_to_XTAL(); /* Initialize system clock to 8M with external 8M crystal*/
#elif	defined(USE_FBE_OSC)
  FEI_to_FBE_OSC();  
#elif	defined(USE_FEE_OSC)
  FEI_to_FEE_OSC();   
#endif  
  
  /* Disable RTC */ 
  RTC_SC2 = 0u;
}

