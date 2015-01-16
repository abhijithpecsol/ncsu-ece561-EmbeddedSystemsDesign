/*!
 * \file    uart.c
 * \brief   UART Driver
 *
 * This driver provides an initialization routine as well as the basic
 * UART I/O functions
 *
 * \version $Revision: 1.3 $
 * \author  Michael Norman
 */

#include "common.h"

/********************************************************************/
/*
 * Initialize the UART for 8N1 operation, interrupts disabled, and
 * no hardware flow-control
 *
 * NOTE: Since the UARTs are pinned out in multiple locations on most
 *       Kinetis devices, this driver does not enable UART pin functions.
 *       The desired pins should be enabled before calling this init function.
 *
 * Parameters:
 *  uartch      UART channel to initialize
 *  sysclk      UART module Clock in kHz(used to calculate baud)
 *  baud        UART baud rate
 */
/* Configure baud rate 
	 * Baud rate settings go as follows:
	 * required baud rate = 115200 bps
	 *       Module clock
	 * BR = --------------
	 *      (16 * (SBR+BRFD))
	 *      
	 *        50*10^6
	 *    = --------------
	 *      (16 * (27+ 4/32))
	 *      
	 *        50*10^6
	 *    = --------------
	 *      (16 * (27.1267))
	 *      
	 *    = 115207 bps
	 *    ~ 115200 bps     
	 */
void uart_init (UART_MemMapPtr uartch, int sysclk, int baud)
{
    register unsigned int sbr, brfa;
    unsigned char temp;
    
    
	/* Enable the pins for the selected UART */
	if (uartch == UART0_BASE_PTR)
	{
		SIM_SCGC1 |= SIM_SCGC1_UART0_MASK;
		#ifdef twrjf
        // Enable Port Muxing of Uart0, TXD on PTA7 and RXD on PTD6
        MXC_PTAPF1 = MXC_PTAPF1_A7(2); //UART0_TX
        MXC_PTDPF1 = MXC_PTDPF1_D6(2); //UART0_RX
        #else
		// Enable Port Muxing of Uart0, TXD on PTC6 and RXD on PTC7
		MXC_PTCPF1 = MXC_PTCPF1_C6(2)|MXC_PTCPF1_C7(2);
        #endif
	}
	if (uartch == UART1_BASE_PTR)
	{
		SIM_SCGC1 |= SIM_SCGC1_UART1_MASK;
		// Enable Port Muxing of Uart1, TXD on PTA2 and RXD on PTA3
		MXC_PTAPF3 = MXC_PTAPF3_A2(2)| MXC_PTAPF3_A3(2);
	}
    
	/* Enable the clock to the selected UART */    
    if(uartch == UART0_BASE_PTR)
		SIM_SCGC1 |= SIM_SCGC1_UART0_MASK;
    else
    	if (uartch == UART1_BASE_PTR)
			SIM_SCGC1 |= SIM_SCGC1_UART1_MASK;
                                
    /* Make sure that the transmitter and receiver are disabled while we 
     * change settings.
     */
    UART_C2_REG(uartch) &= ~(UART_C2_TE_MASK
				| UART_C2_RE_MASK );

    /* Configure the UART for 8-bit mode, no parity */
    UART_C1_REG(uartch) = 0;	/* We need all default settings, so entire register is cleared */
    
    /* Calculate baud settings */
    sbr = (unsigned int)((sysclk*1000)/(baud * 16));
        
    /* Save off the current value of the UARTx_BDH except for the SBR field */
    temp = UART_BDH_REG(uartch) & ~(UART_BDH_SBR(0x1F));
    
    UART_BDH_REG(uartch) = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
    UART_BDL_REG(uartch) = (unsigned char)(sbr & UART_BDL_SBR_MASK);
    
    /* Determine if a fractional divider is needed to get closer to the baud rate */
    brfa = (((sysclk*32000)/(baud * 16)) - (sbr * 32));
    
    /* Save off the current value of the UARTx_C4 register except for the BRFA field */
    temp = UART_C4_REG(uartch) & ~(UART_C4_BRFA(0x1F));
    
    UART_C4_REG(uartch) = temp |  UART_C4_BRFA(brfa);    

    /* Enable receiver and transmitter */
	UART_C2_REG(uartch) |= (UART_C2_TE_MASK | UART_C2_RE_MASK );
}	
	
	
	
/********************************************************************/
/*!
 * \brief   Get character from UART
 * \param   channel UART channel
 * \return  The received character
 *
 * Poll the UART receive FIFO until a character is present.  Return
 * a single character.
 */
char
uart_getchar (UART_MemMapPtr channel)
{
   /* Wait until character has been received */
    while (!(UART_S1_REG(channel) & UART_S1_RDRF_MASK)) {};
    return UART_D_REG(channel);
}
/********************************************************************/
/*!
 * \brief   Sent a character out the UART
 * \param   channel UART channel
 * \param   ch      Character to send
 * \return  None
 *
 * Poll the UART transmit FIFO until there is room for a character.  
 * Put a character into the Tx FIFO.
 */
void
uart_putchar (UART_MemMapPtr channel, char ch)
{
    /* Wait until space is available in the FIFO */
    while(!(UART_S1_REG(channel) & UART_S1_TDRE_MASK)) {};
    /* Send the character */
    UART_D_REG(channel) = (unsigned char)ch;
}
/********************************************************************/
/*!
 * \brief   Check for a received character 
 * \param   channel UART channel
 * \return  TRUE if a character is present, FALSE otherwise
 *
 * Poll the UART receive FIFO and determine if a character is present.
 */
char uart_getchar_present (UART_MemMapPtr channel)
{
    return (char) (UART_S1_REG(channel) & UART_S1_RDRF_MASK);
}
/********************************************************************/

