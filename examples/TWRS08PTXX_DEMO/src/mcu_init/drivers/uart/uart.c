/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2011-2012 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file uart.c
*
* @author a13984
*
* @version 0.0.3.0
*
* @date Dec-20-2011
*
* @brief providing common UART routines. 
*
*******************************************************************************
*
* provide APIs for access SCIs. more APIs to be added.
******************************************************************************/
#include "uart.h"


/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: TERMIO_PutChar1
*
* @brief print a char via SCI0
*        
* @param  send	a char to print 
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

  void TERMIO_PutChar1(char send)
  {
    char dummy;

    while((SCI0_S1 & SCI0_S1_TDRE_MASK) != SCI0_S1_TDRE_MASK);
    dummy = SCI0_S1;
    SCI0_D  = send;
  }//end SCI_PutChar

  /*****************************************************************************//*!
  +FUNCTION----------------------------------------------------------------
  * @function name: TERMIO_GetChar1
  *
  * @brief receives a char via SCI0
  *        
  * @param  none 
  *
  * @return a char received
  *
  * @ Pass/ Fail criteria: none
  *****************************************************************************/

  char TERMIO_GetChar1(void)
  {
    char dummy, dummy2;

    while(( SCI0_S1 & SCI0_S1_RDRF_MASK) != SCI0_S1_RDRF_MASK);
    dummy = SCI0_S1;
    dummy = SCI0_D;
#if	(MASKSET == PT60_2N40C)    
    // read twice to clear 2nd RDRF 
    dummy2 = SCI0_S1;
    dummy2 = SCI0_D;
#else
    (void) dummy2;
#endif     
    return dummy;
  } //end SCI_GetChar

  /*****************************************************************************//*!
  +FUNCTION----------------------------------------------------------------
  * @function name: Init_SCI0
  *
  * @brief initializes SCI0 baud rate, enable both tx and rx
  *        
  * @param  none 
  *
  * @return none
  *
  * @ Pass/ Fail criteria: none
  *****************************************************************************/
  void Init_SCI0(dword busCLKHz)
  {	  
	 byte dummy;
	 
    /* make sure clocks to peripheral modules are enabled */
    SCG_C3 |= SCG_C3_SCI0_MASK;

	 /* SCI0_BDH = 0;*/
#if (USE_SCI_BAUDRATE != 9600)
	  /* use 38400bps */
    /*
     * SCI0_BDL = 0x20;  // BDH_BDL = Bus_CLK/(16*BR), BUS_CLK = 20MHz,BR = 38400
     */
	 SCI0_BD = (SCI0_BD & 0xE000) | ((busCLKHz>>4)/38400L);
#else
    /* SCI0_BDL = 0x80;  // BDH_BDL = Bus_CLK/(16*BR), BUS_CLK = 20MHz,BR = 9600
     * 
     */	
	 SCI0_BD = (SCI0_BD & 0xE000) | ((busCLKHz>>4)/9600L);	 
#endif
	 
    SCI0_C1  = 0;     // 8bit mode, 1 stop bit, no parity
    SCI0_C2  = 0x0C;  // enable TX , enable RX
    dummy = SCI0_S1;
    dummy = SCI0_D;
  }//end Init_SCI0_

  /*****************************************************************************//*!
  +FUNCTION----------------------------------------------------------------
  * @function name: TERMIO_PutChar2
  *
  * @brief sends a char via SCI1
  *        
  * @param  send  a char to be sent
  *
  * @return none
  *
  * @ Pass/ Fail criteria: none
  *****************************************************************************/

  void TERMIO_PutChar2(char send)
  {
    char dummy;

    while(( SCI1_S1 & SCI1_S1_TDRE_MASK) != SCI1_S1_TDRE_MASK);
    dummy = SCI1_S1;
    SCI1_D  = send;
  }//end SCI_PutChar

  /*****************************************************************************//*!
  +FUNCTION----------------------------------------------------------------
  * @function name: TERMIO_GetChar2
  *
  * @brief receive a char from SCI1
  *        
  * @param  none 
  *
  * @return a char received
  *
  * @ Pass/ Fail criteria: none
  *****************************************************************************/

  char TERMIO_GetChar2(void)
  {
    char dummy,dummy2;

    while(( SCI1_S1 & SCI1_S1_RDRF_MASK) !=SCI1_S1_RDRF_MASK);
    dummy = SCI1_S1;
    dummy = SCI1_D;
#if	(MASKSET == PT60_2N40C)    
    // read twice to clear 2nd RDRF 
    dummy2 = SCI1_S1;
    dummy2 = SCI1_D;
#else
    (void) dummy2;
#endif     
    return dummy;
  } //end SCI_GetChar

  /*****************************************************************************//*!
  +FUNCTION----------------------------------------------------------------
  * @function name: Init_SCI1
  *
  * @brief initializes SCI1 baud rate, enable both tx and rx
  *        
  * @param  none 
  *
  * @return none
  *
  * @ Pass/ Fail criteria: none
  *****************************************************************************/
  void Init_SCI1(void)
  {
    byte dummy;
	  
    /* make sure clocks to peripheral modules are enabled */
    SCG_C3 |= SCG_C3_SCI1_MASK;

    /* Set Baud rate (BUS_CLK_HZ/16/USE_SCI_BAUDRATE) for 16Mhz BusClk */
    #if (CORE_CLK_KHZ == 16000)
      #if (USE_SCI_BAUDRATE == 115200)
        SCI1_BD = (SCI1_BD & 0xE000) | 9u; // 115200
      #elif (USE_SCI_BAUDRATE == 57600)
        SCI1_BD = (SCI1_BD & 0xE000) | 17u; // 57600
      #elif (USE_SCI_BAUDRATE == 38400) 
        SCI1_BD = (SCI1_BD & 0xE000) | 26u; // 38400 
      #elif (USE_SCI_BAUDRATE == 19200)
        SCI1_BD = (SCI1_BD & 0xE000) | 52u; // 19200
      #elif (USE_SCI_BAUDRATE == 9600)      
        SCI1_BD = (SCI1_BD & 0xE000) | 104u; // 9600
      #elif (USE_SCI_BAUDRATE == 4800)
        SCI1_BD = (SCI1_BD & 0xE000) | 208u; // 4800
      #else 
        #error " Undefined baudrate "
      #endif
    /* Set Baud rate (BUS_CLK_HZ/16/USE_SCI_BAUDRATE) for 8Mhz BusClk */        
    #elif (CORE_CLK_KHZ == 8000)
      #if (USE_SCI_BAUDRATE == 57600)
        SCI1_BD = (SCI1_BD & 0xE000) | 9u; // 57600
      #elif (USE_SCI_BAUDRATE == 38400) 
        SCI1_BD = (SCI1_BD & 0xE000) | 13u; // 38400 
      #elif (USE_SCI_BAUDRATE == 19200)
        SCI1_BD = (SCI1_BD & 0xE000) | 26u; // 19200
      #elif (USE_SCI_BAUDRATE == 9600)      
        SCI1_BD = (SCI1_BD & 0xE000) | 52u; // 9600
      #elif (USE_SCI_BAUDRATE == 4800)
        SCI1_BD = (SCI1_BD & 0xE000) | 104u; // 4800
      #elif (USE_SCI_BAUDRATE == 2400)
        SCI1_BD = (SCI1_BD & 0xE000) | 208u; // 2400
      #else 
        #error " Undefined baudrate "
      #endif
    #else
      #error " Unsupported MCU CLK "
    #endif        
    
    /* Set mode */
	SCI1_C1  = 0;     // 8bit mode, 1 stop bit, no parity
    SCI1_C2  = 0x0C;  // enable TX , enable RX
    SCI1_C3  = 0;
    
    dummy = SCI1_S1;
    dummy = SCI1_D;    
  }//end Init_SCI0_
  
  /*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: TERMIO_PutChar3
   *
   * @brief send a char thru SCI2
   *        
   * @param  send, a char to be sent
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/

  void TERMIO_PutChar3(char send)
  {
    char dummy;

    while(( SCI2_S1 & SCI2_S1_TDRE_MASK) != SCI2_S1_TDRE_MASK);
    dummy = SCI2_S1;
    SCI2_D  = send;
  }//end SCI_PutChar

  /*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: TERMIO_GetChar3
   *
   * @brief receives a char from SCI2
   *        
   * @param  none
   *
   * @return a char received
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/

  char TERMIO_GetChar3(void)
  {
    char dummy,dummy2;

    while(( SCI2_S1 & SCI2_S1_RDRF_MASK) !=SCI2_S1_RDRF_MASK);
    dummy = SCI2_S1;
    dummy = SCI2_D;
#if	(MASKSET == PT60_2N40C)    
    // read twice to clear 2nd RDRF 
    dummy2 = SCI2_S1;
    dummy2 = SCI2_D;
#else
    (void) dummy2;
#endif 
    return dummy;
  } //end SCI_GetChar

  /*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: Init_SCI2
   *
   * @brief initializes SCI2 baud rate, enable both tx and rx
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/

  void Init_SCI2(void)
  {
    byte dummy;
    
    /* make sure clocks to peripheral modules are enabled */
    SCG_C3 |= SCG_C3_SCI2_MASK;
    
    /* Set Baud rate */
    SCI2_BDH = 0u;
	SCI2_BDL = (BUS_CLK_HZ/16/USE_SCI_BAUDRATE);
	/* Set mode */
    SCI2_C1  = 0;     // 8bit mode, 1 stop bit, no parity
    SCI2_C2  = 0x0C;  // enable TX , enable RX
    dummy = SCI2_S1;
    dummy = SCI2_D;    
  }//end Init_SCI0_

  
  /*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: wait_sci_tx_empty
   *
   * @brief loops to wait for the SCI tx empty
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/

void wait_sci_tx_empty(void){
 #if (USE_SCI == 0)
    while(( SCI0_S1 & SCI0_S1_TDRE_MASK) !=SCI0_S1_TDRE_MASK);
 #elif (USE_SCI == 1)
    while(( SCI1_S1 & SCI1_S1_TDRE_MASK) !=SCI1_S1_TDRE_MASK);
 #elif (USE_SCI == 2)
    while(( SCI2_S1 & SCI2_S1_TDRE_MASK) !=SCI2_S1_TDRE_MASK);
 #else
	#error "undefined SCI port!\n";
 #endif
}
