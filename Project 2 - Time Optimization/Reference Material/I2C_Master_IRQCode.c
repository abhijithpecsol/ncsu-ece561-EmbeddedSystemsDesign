/*
 * I2C_Master.c
 *
 *  Created on: Jan 29, 2014
 *      Author: B47334
 */

#include "I2C_Master.h"



void Enable_irq (int irq)
{   
    /* Make sure that the IRQ is an allowable number. Up to 32 is 
     * used.
     *
     * NOTE: If you are using the interrupt definitions from the header
     * file, you MUST SUBTRACT 16!!!
     */

      /* Set the ICPR and ISER registers accordingly */
      NVIC_ICPR |= 1 << (irq%32);
      NVIC_ISER |= 1 << (irq%32);
}



void I2C_Master_Init(void)
{
	/* Enable clock for I2C0 module */
	SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;
	
	/* Enable clock for Port E */
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	
	/* Port E MUX configuration */
	PORTE_PCR24 |=  PORT_PCR_MUX(5);
	PORTE_PCR25 |=  PORT_PCR_MUX(5);
	
	
	/* Configure Divider Register */
	I2C0_F |= (I2C_F_MULT(2) | I2C_F_ICR(22));
	
	/* Enable I2C module and interrupt */
	I2C0_C1 |= I2C_C1_IICEN_MASK | I2C_C1_IICIE_MASK;
	
	
	/* Enable TX mode */
	I2C0_C1 |= I2C_C1_TX_MASK;
	
	/* Enable I2C0 NVIC interrupt */
	Enable_irq(INT_I2C0 - 16);
}




void I2C_Master_Transmit(void)
{
	I2C0_C1 |= I2C_C1_MST_MASK;		//  Generate START SIGNAL
	I2C0_D = ((SLAVE_ADDRESS << 1) | READ);		// Write 7-bit Slave Address + READ bit
}




void I2C0_IRQHandler(void)
{
	u8 status = 0x00;
	u8 dummy_var;
	
	status = I2C0_S;	// Read status
	
	I2C0_S |= I2C_S_IICIF_MASK;	// Clear interrupt flag
	
	if(I2C0_C1 & I2C_C1_TX_MASK) // Transmitter mode?
	{
		if((status & I2C_S_RXAK_MASK) == 0) // ACK Received?
		{
			I2C0_C1 &= ~I2C_C1_TX_MASK;	// Change to receiver mode
			dummy_var = I2C0_D;	// Dummy read to start reception
		}
		else
		{
			I2C0_C1 &= ~I2C_C1_MST_MASK;	// Generate STOP signal
		}
	}
	else
	{	
		byte_counter++;		// Increment the count of bytes received
		
		if(byte_counter == BUFFER_SIZE)	// Last byte to be received?
		{
			I2C0_C1 &= ~I2C_C1_MST_MASK;	// Generate STOP signal
		}
		else
		{
			if(byte_counter == (BUFFER_SIZE - 1))	// Only 1 more byte pending to read?
			{
				I2C0_C1 |= I2C_C1_TXAK_MASK;	// Generate NACK in the next reception
			}
		}
		
		rx_buffer[byte_counter-1] = I2C0_D;	// Copy data register to buffer
	}
}
