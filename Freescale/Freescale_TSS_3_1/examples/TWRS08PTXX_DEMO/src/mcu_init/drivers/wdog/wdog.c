
/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2011-2012 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file wdog.c
*
* @author a13984
*
* @version 0.0.2.0
*
* @date Feb-3-2012
*
* @brief Provide common watchdog module routines. 
*
* @history:
* 	Jul.20, 2011	modified the watch dog unlock sequence and disable sequence
*******************************************************************************
* NOTE: Need to add more functionality. Right now it
* 				is just a disable routine since we know almost
* 				all projects will need that.    
******************************************************************************/
#include "common.h"
#include "wdog.h"
 
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
* @function name: wdog_disable
*
* @brief Watchdog timer disable routine
*        
* @param  send	a char to print 
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

void wdog_disable(void)
{
	/* First unlock the watchdog so that we can write to registers */
	 wdog_unlock();	
	/* Clear the WDOGE bit to disable the watchdog */
	/* WDOG_CS1 &= ~WDOG_CS1_WDOGE_MASK; */
	/*
	*((unsigned char*)0x3030) = 0x0;
		*((unsigned char*)0x3031) = 0x0;
		*((unsigned char*)0x3034) = 0xFF;
		*((unsigned char*)0x3035) = 0xFF;
		*((unsigned char*)0x3036) = 0x0;
		*((unsigned char*)0x3037) = 0x0;
	  asm{	// unlock watch dog
	  		LDA	#0xC5
			STA	0x3032
			LDA	#0x20
			STA	0x3033
			LDA	#0xD9
			STA	0x3032
			LDA	#0x28
			STA	0x3033	
		}
	*/

	/* Write all 6 registers once within 128 bus cycles after unlocking */
	WDOG_CS1 = 0x20;
	WDOG_CS2 = 0;
	WDOG_TOVALH = 0xFF;
	WDOG_TOVALL = 0xFF;
	WDOG_WINH = 0;
	WDOG_WINL = 0;
}

void wdog_enable(void )
{
	  wdog_unlock();	

		
		/* NOTE: the following write sequence must be completed within 128 buc clocks
		 * 
		 */
		 /* enable watchdog */

		WDOG_CS1 = 0x20 | WDOG_CS1_UPDATE_MASK; 
		         //  | WDOG_CS1_WDOGSE_MASK 
		         //  | WDOG_CS1_WDOGWE_MASK ;		// debug enable
		WDOG_CS2 = 1;	/* use internal 1K clock as clock source */	
		
		WDOG_TOVAL = 0x80FF;
}

/********************************************************************/
