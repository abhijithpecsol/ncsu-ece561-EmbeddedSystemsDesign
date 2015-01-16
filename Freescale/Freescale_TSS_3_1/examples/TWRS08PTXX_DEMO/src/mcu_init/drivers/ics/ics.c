
/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2011-2012 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file ics.c
*
* @author a13984
*
* @version 0.0.3.0
*
* @date Feb-3-2012
*
* @brief providing APIs for configuring internal clock sources (ICS). 
*
*******************************************************************************
*
* provide APIs for configuring internal clock sources (ICS)
******************************************************************************/
#include "common.h"
#include "ics.h"

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
void FEI_to_FEE(void);
void FEI_to_XTAL(void);
void FEE_to_FEI(void);
void FBE_to_FBI(void);

/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: FEI_to_FEE
   *
   * @brief change clock from FEI mode to FEE mode and divide clock by 1
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void FEI_to_FEE(void)
{
	/* assume external crystal is 8Mhz or 4MHz
	 * 
	 */
	/* enable OSC with high gain, high range and select oscillator output as OSCOUT
	 * 
	 */
	ICS_OSCSC |= ICS_OSCSC_OSCEN_MASK
#if defined(CRYST_HIGH_GAIN)
	|  ICS_OSCSC_HGO_MASK 		/* Rs must be added and be large up to 200K */
#endif
	| ICS_OSCSC_RANGE_MASK 
	| ICS_OSCSC_OSCOS_MASK;
	asm{
		nop
		nop
	}
	/* wait for OSC to be initialized
	 * 
	 */
	while(!(ICS_OSCSC & ICS_OSCSC_OSCINIT_MASK));
	
	/* divide down external clock frequency to be within 31.25K to 39.0625K
	 * 
	 */
#if (EXT_CLK_CRYST == 8000)	
	/* 8MHz */
	ICS_C1_RDIV = 3;	/* now the divided frequency is 8000/256 = 31.25K */
#elif (EXT_CLK_CRYST == 4000)
	/* 4MHz */
	ICS_C1_RDIV = 2;	/* now the divided frequency is 4000/128 = 31.25K */	
#else
	#error "Error: crystal value not supported!\n";
#endif
	
	/* change FLL reference clock to external clock */
	ICS_C1_IREFS = 0;
	
	/* wait for the reference clock to be changed to external */
	asm{ 
		nop
		nop
	}
	while(ICS_S & ICS_S_IREFST_MASK);
	
	/* wait for FLL to lock */
	while(!(ICS_S & ICS_S_LOCK_MASK));
		
	/* now FLL output clock is 31.25K*512 = 16MHz
	 * 
	 */
	if(((ICS_C2 & ICS_C2_BDIV_MASK)>>5) != 0)
	{
		ICS_C2_BDIV = 0;
	}
	/* now system/bus clock is 16MHz
	 * 
	 */
	/* clear Loss of lock sticky bit */
	ICS_S |= ICS_S_LOLS_MASK;	
}

/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: FEI_to_XTAL
*
* @brief change clock from FEI mode to external crystal
*        
* @param  none 
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void FEI_to_XTAL(void)
{
    /* assume external crystal is 8Mhz or 4MHz
     * 
     */
    /* enable OSC with high gain, high range and select oscillator output as OSCOUT
     * 
     */

    ICS_OSCSC |= ICS_OSCSC_OSCEN_MASK
    #if defined(CRYST_HIGH_GAIN)
    |  ICS_OSCSC_HGO_MASK       /* Rs must be added and be large up to 200K */
    #endif
    | ICS_OSCSC_RANGE_MASK 
    | ICS_OSCSC_OSCOS_MASK;
    asm{
        nop
        nop
    }
    /* wait for OSC to be initialized
       * 
       */
    while(!(ICS_OSCSC & ICS_OSCSC_OSCINIT_MASK));
    
    
    ICS_C1_CLKS = 2;
    ICS_C2_BDIV = 0;
    
}

/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: FEI_to_FBI
   *
   * @brief change clock from FEI mode to FBI mode and divide clock by 1
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void FEI_to_FBI(void)
{
	/* assume external crystal is 8Mhz or 4MHz
	 * 
	 */
	/* change clock source to internal reference clock */
	ICS_C1_CLKS = 1;
	ICS_C2_LP = 0; 
	/* wait for the reference clock to be changed  */
	asm{ 
		nop
		nop
	}
	while(ICS_S_CLKST !=1);
			
	/* now internal reference clock is the system clock
	 * 
	 */
	if(((ICS_C2 & ICS_C2_BDIV_MASK)>>5) != 0)
	{
		ICS_C2_BDIV = 0;
	}
	/* now system/bus clock is around 32KHz
	 * 
	 */
	/* clear Loss of lock sticky bit */
	ICS_S |= ICS_S_LOLS_MASK;	
}

/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: FEI_to_FBE
   *
   * @brief change clock from FEI mode to FBE mode and divide clock by 1
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void FEI_to_FBE(void)
{
	/* assume external crystal is 8Mhz or 4MHz
	 * 
	 */
	/* change clock source to external reference clock */
	ICS_C1_IREFS = 0;
	ICS_C1_CLKS = 2;
	ICS_C2_LP = 0; 
	/* wait for the reference clock to be changed  */
	asm{ 
		nop
		nop
	}
	while(ICS_S_CLKST != 2);
	while(ICS_S_IREFST);
			
	/* now external  clock is the system clock
	 * 
	 */
	if(((ICS_C2 & ICS_C2_BDIV_MASK)>>5) != 0)
	{
		ICS_C2_BDIV = 0;
	}
	/* now system/bus clock is external clock
	 * 
	 */
	/* clear Loss of lock sticky bit */
	ICS_S |= ICS_S_LOLS_MASK;	
}

/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: FEI_to_FBE_OSC
   *
   * @brief change clock from FEI mode to FBE mode with external clock/oscillator 
   * and divide clock by 1
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void FEI_to_FBE_OSC(void)
{
//#define XOSC_ENABLE
#if	!defined(XOSC_ENABLE)	
	/* XOSC bypass mode, i.e. XOSC is disabled */
	/* PASS */
	ICS_OSCSC = 0x04;	/* high range */
#else
	/* XOSC is enabled */
	/* PASS */
	ICS_OSCSC = 0x04 | ICS_OSCSC_OSCEN_MASK | ICS_OSCSC_OSCSTEN_MASK;	/* high range */	
#endif
	/* change RDIV  reference divider to divide reference clock to be with FLL input spec
	 * 
	 */
#if (EXT_CLK_CRYST == 8000)	
	/* 8MHz */
	ICS_C1_RDIV = 3;	/* now the divided frequency is 8000/256 = 31.25K */
#elif (EXT_CLK_CRYST == 4000)
	/* 4MHz */
	ICS_C1_RDIV = 2;	/* now the divided frequency is 4000/128 = 31.25K */	
#else
	#error "Error: crystal value not supported!\n";
#endif	
	/* assume external oscillator is 8Mhz or 4MHz
	 * 
	 */
	/* change clock source to external reference clock */
	ICS_C1_IREFS = 0;
	ICS_C1_CLKS = 2;
	ICS_C2_LP = 0; 
	/* wait for the reference clock to be changed  */
	asm{ 
		nop
		nop
	}
	while(ICS_S_CLKST != 2);
	while(ICS_S_IREFST);
			
	/* now external  clock is the system clock
	 * 
	 */
	if(((ICS_C2 & ICS_C2_BDIV_MASK)>>5) != 0)
	{
		ICS_C2_BDIV = 0;
	}
	/* now system/bus clock is external clock
	 * 
	 */
	/* clear Loss of lock sticky bit */
	ICS_S |= ICS_S_LOLS_MASK;	
}

/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: FEI_to_FEE_OSC
   *
   * @brief change clock from FEI mode to FEE mode with external clock/oscillator 
   * and divide clock by 1
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void FEI_to_FEE_OSC(void)
{
#define XOSC_ENABLE
#if	!defined(XOSC_ENABLE)	
	/* XOSC bypass mode, i.e. XOSC is disabled */
	/* PASS */
	ICS_OSCSC = 0x04;	/* high range */
#else
	/* XOSC is enabled */
	/* PASS */
	ICS_OSCSC = 0x04 | ICS_OSCSC_OSCEN_MASK | ICS_OSCSC_OSCSTEN_MASK;	/* high range */	
#endif
	/* change RDIV  reference divider to divide reference clock to be with FLL input spec
	 * 
	 */
#if (EXT_CLK_CRYST == 8000)	
	/* 8MHz */
	ICS_C1_RDIV = 3;	/* now the divided frequency is 8000/256 = 31.25K */
#elif (EXT_CLK_CRYST == 4000)
	/* 4MHz */
	ICS_C1_RDIV = 2;	/* now the divided frequency is 4000/128 = 31.25K */	
#else
	#error "Error: crystal value not supported!\n";
#endif	
	/* assume external oscillator is 8Mhz or 4MHz
	 * 
	 */
	/* change FLL reference clock to external clock */
	ICS_C1_IREFS = 0;
	
	/* wait for the reference clock to be changed to external */
	asm{ 
		nop
		nop
	}
	while(ICS_S & ICS_S_IREFST_MASK);
	
	/* wait for FLL to lock */
	while(!(ICS_S & ICS_S_LOCK_MASK));
		
	/* now FLL output clock is 31.25K*512 = 16MHz
	 * 
	 */
	if(((ICS_C2 & ICS_C2_BDIV_MASK)>>5) != 0)
	{
		ICS_C2_BDIV = 0;
	}
	/* now system/bus clock is 16MHz
	 * 
	 */
	/* clear Loss of lock sticky bit */
	ICS_S |= ICS_S_LOLS_MASK;	
}


/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: FEE_to_FEI
   *
   * @brief change clock from FEE mode to FEI mode
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void FEE_to_FEI(void)
{
	/* assume external crystal is 8Mhz or 4MHz
	 * 
	 */
		
	
	ICS_C1_IREFS = 1;	/* select internal reference for FLL */ 
	
	/* wait for the reference clock to be changed */
	asm{ 
		nop
		nop
	}
	while(!(ICS_S & ICS_S_IREFST_MASK));
	
	/* wait for FLL to lock */
	while(!(ICS_S & ICS_S_LOCK_MASK));

	/* clear Loss of lock sticky bit */
	ICS_S |= ICS_S_LOLS_MASK;
	
	/* now FLL output clock is 31.25K*512 = 16MHz
	 * 
	 */
	if(((ICS_C2 & ICS_C2_BDIV_MASK)>>5) != 0)
	{
		ICS_C2_BDIV = 0;
	}
	/* now system/bus clock is around 16MHz
	 * 
	 */
}

/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: FEE_to_FBI
   *
   * @brief change clock from FEE mode to FBI mode
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void FEE_to_FBI(void)
{
	/* assume external crystal is 8Mhz or 4MHz
	 * 
	 */
	/* clear Loss of lock sticky bit */
	ICS_S |= ICS_S_LOLS_MASK;
	
	ICS_C1_CLKS =  1;   /* select internal clock as clock source */
	ICS_C1_IREFS = 1;	/* select internal reference for FLL */ 
	ICS_C2_LP = 0;		/* LP = 0 */
	
	/* wait for the reference clock to be changed */
	asm{ 
		nop
		nop
	}
	while(!(ICS_S & ICS_S_IREFST_MASK));
	while(ICS_S_CLKST != 1);
	
	/* now system clock source is internal reference clock around 32K
	 * 
	 */
	if(((ICS_C2 & ICS_C2_BDIV_MASK)>>5) != 0)
	{
			ICS_C2_BDIV = 0;
	}	
}


/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: FEE_to_FBE
   *
   * @brief change clock from FEE mode to FBE mode
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void FEE_to_FBE(void)
{
	/* assume external crystal is 8Mhz or 4MHz
	 * 
	 */
	/* clear Loss of lock sticky bit */
	ICS_S |= ICS_S_LOLS_MASK;
	
	ICS_C1_CLKS =  2;   /* select the external clock as clock source */
	ICS_C2_LP = 0;		/* LP = 0 */
	
	/* wait for the clock source to be changed */
	asm{ 
		nop
		nop
	}
	while(ICS_S_CLKST != 2);
	
	/* now system clock source is external clock
	 * NOTE: make sure that the external clock is within 20MHz 
	 */
	if(((ICS_C2 & ICS_C2_BDIV_MASK)>>5) != 0)
	{
			ICS_C2_BDIV = 0;
	}	
}


/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: FBI_to_FBE
   *
   * @brief change clock from FBI mode to FBE mode
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void FBI_to_FBE(void)
{
	/* assume external crystal is 8Mhz or 4MHz
	 * 
	 */
	ICS_C1_IREFS = 0;	/* select external reference clock */
	ICS_C1_CLKS =  2;   /* select the external clock as clock source */
	
	/* wait for the clock source to be changed */
	asm{ 
		nop
		nop
	}
	while(ICS_S_CLKST != 2);
	while(!ICS_S_IREFST);
	
	
	/* now system clock source is external clock
	 * NOTE: make sure that the external clock is within 20MHz 
	 */
	if(((ICS_C2 & ICS_C2_BDIV_MASK)>>5) != 0)
	{
			ICS_C2_BDIV = 0;
	}	
}


/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: FBI_to_FEE
   *
   * @brief change clock from FBI mode to FEE mode
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void FBI_to_FEE(void)
{
	/* assume external crystal is 8Mhz or 4MHz
	 * 
	 */
	ICS_C1_IREFS = 0;	/* select external reference clock */
	ICS_C1_CLKS =  0;   /* select the FLL output as clock source */
	
	/* wait for the clock source to be changed */
	asm{ 
		nop
		nop
	}
	while(ICS_S_CLKST != 0);
	while(!ICS_S_IREFST);
	
	
	/* now system clock source is external clock
	 * NOTE: make sure that the external clock is within 20MHz 
	 */
	if(((ICS_C2 & ICS_C2_BDIV_MASK)>>5) != 0)
	{
			ICS_C2_BDIV = 0;
	}
	/* clear Loss of lock sticky bit */
	ICS_S |= ICS_S_LOLS_MASK;	
}

/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: FBI_to_FBILP
   *
   * @brief change clock from FBI mode to FBILP mode. NOTE: make sure that
   *        BDM is disabled.
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void FBI_to_FBILP(void)
{
	/* assume external crystal is 8Mhz or 4MHz
	 * 
	 */
	ICS_C2_LP = 1;	/* enter low power mode */
}


/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: FBI_to_FEI
   *
   * @brief change clock from FBI mode to FEIP mode. 
   * 
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void FBI_to_FEI(void)
{
	/* assume external crystal is 8Mhz or 4MHz
	 * 
	 */
	ICS_C1_IREFS = 1;	/* select external reference clock */
	ICS_C1_CLKS =  0;   /* select the FLL output as clock source */
	
	/* wait for the clock source to be changed */
	asm{ 
		nop
		nop
	}
	while(ICS_S_CLKST != 0);
	while(!ICS_S_IREFST);

	/* now system clock source is FLL output
	 * 
	 */
	if(((ICS_C2 & ICS_C2_BDIV_MASK)>>5) != 0)
	{
			ICS_C2_BDIV = 0;
	}	
	/* clear Loss of lock sticky bit */
	ICS_S |= ICS_S_LOLS_MASK;	
}



/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: FBE_to_FBI
   *
   * @brief change clock from FBE mode to FBI mode
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void FBE_to_FBI(void)
{
	/* assume external crystal is 8Mhz or 4MHz
	 * 
	 */
	ICS_C1_IREFS = 1;	/* select internal reference clock */
	ICS_C1_CLKS =  1;   /* select the internal clock as clock source */
	
	/* wait for the clock source to be changed */
	asm{ 
		nop
		nop
	}
	while(ICS_S_CLKST != 1);
	while(ICS_S_IREFST != 1);	
	
	/* now system clock source is internal clock
	 * 
	 */
	if(((ICS_C2 & ICS_C2_BDIV_MASK)>>5) != 0)
	{
			ICS_C2_BDIV = 0;
	}	
}

/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: FBE_to_FEE
   *
   * @brief change clock from FBE mode to FEE mode
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void FBE_to_FEE(void)
{
	/* assume external crystal is 8Mhz or 4MHz
	 * 
	 */

	ICS_C1_CLKS =  0;   /* select the FLL output as clock source */
	
	/* wait for the clock source to be changed */
	asm{ 
		nop
		nop
	}
	while(!ICS_S_CLKST);

	
	/* now system clock source is FLL output
	 * NOTE: external clock <= 20MHz
	 */
	if(((ICS_C2 & ICS_C2_BDIV_MASK)>>5) != 0)
	{
			ICS_C2_BDIV = 0;
	}	
	/* clear Loss of lock sticky bit */
	ICS_S |= ICS_S_LOLS_MASK;	
}


/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: FBE_to_FEI
   *
   * @brief change clock from FBE mode to FEI mode
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void FBE_to_FEI(void)
{
	/* assume external crystal is 8Mhz or 4MHz
	 * 
	 */
	ICS_C1_IREFS = 1;	/* select internal reference clock */
	ICS_C1_CLKS =  0;   /* select the internal clock as clock source */
	
	/* wait for the clock source to be changed */
	asm{ 
		nop
		nop
	}
	while(ICS_S_CLKST);
	while(ICS_S_IREFST != 1);	
	
	/* now system clock source is internal clock
	 * 
	 */
	if(((ICS_C2 & ICS_C2_BDIV_MASK)>>5) != 0)
	{
			ICS_C2_BDIV = 0;
	}	
	/* clear Loss of lock sticky bit */
	ICS_S |= ICS_S_LOLS_MASK;	
}



/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: FBE_to_FBELP
   *
   * @brief change clock from FBE mode to FBELP mode. NOTE: make sure that
   *        BDM is disabled.
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void FBE_to_FBELP(void)
{
	/* assume external crystal is 8Mhz or 4MHz
	 * 
	 */
	ICS_C2_LP = 1;	/* enter low power mode */
}


/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: FBELP_to_FBE
   *
   * @brief change clock from FBELP mode to FBE mode. 
   * 
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void FBELP_to_FBE(void)
{
	/* assume external crystal is 8Mhz or 4MHz
	 * 
	 */
	ICS_C2_LP = 0;	/* enter low power mode */
}

/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: FBILP_to_FBI
   *
   * @brief change clock from FBILP mode to FBI mode. 
   * 
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void FBILP_to_FBI(void)
{
	/* assume external crystal is 8Mhz or 4MHz
	 * 
	 */
	ICS_C2_LP = 0;	/* enter low power mode */
}


/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: XOSC_Init
   *
   * @brief initialize XOSC with given parameters: GAIN, RANGE in control structure
   * 
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void XOSC_Init(TXOSC_Control *pOSCCtrl)
{
	
	/* 
	 * 
	 */
	if(pOSCCtrl->gain)
	{
		/* high gain */
		ICS_OSCSC |= ICS_OSCSC_HGO_MASK ;		/* Rs must be added and be large up to 200K */
	}
	else
	{
		/* low gain */
		ICS_OSCSC &= ~ ((unsigned char) ICS_OSCSC_HGO_MASK);			
	}
	
	if(pOSCCtrl->range)
	{
		ICS_OSCSC |= ICS_OSCSC_RANGE_MASK; /* high range */	
	}
	else
	{
		ICS_OSCSC &= ~((unsigned char) ICS_OSCSC_RANGE_MASK); /* low range */			
	}
	if(pOSCCtrl->stopEnable)
	{
		ICS_OSCSC |= ICS_OSCSC_OSCSTEN_MASK; 
	}
	else
	{
		ICS_OSCSC &= ~((unsigned char) ICS_OSCSC_OSCSTEN_MASK); 		
	}
	if(pOSCCtrl->oscOut)
	{
		ICS_OSCSC |= ICS_OSCSC_OSCOS_MASK;
	}
	else
	{
		ICS_OSCSC &= ~((unsigned char) ICS_OSCSC_OSCOS_MASK);		
	}
	if(pOSCCtrl->enable)
	{
		ICS_OSCSC |= ICS_OSCSC_OSCEN_MASK;
	}
	else
	{
		ICS_OSCSC &= ~((unsigned char) ICS_OSCSC_OSCEN_MASK);		
	}	
	if(pOSCCtrl->waitInit)
	{

		/* wait for OSC to be initialized
		 * 
		 */
		while(!(ICS_OSCSC & ICS_OSCSC_OSCINIT_MASK));
		
	}
}
