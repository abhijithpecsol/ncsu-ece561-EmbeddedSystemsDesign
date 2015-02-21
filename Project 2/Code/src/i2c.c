#include <MKL25Z4.H>
#include "i2c.h"

uint8_t i2cLockedUp = WORKING;

//init i2c0
void i2c_init(void)
{
	//clock i2c peripheral and port E
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	SIM->SCGC5 |= (SIM_SCGC5_PORTE_MASK);
	
	//set pins to I2C function
	PORTE->PCR[24] |= PORT_PCR_MUX(5);
	PORTE->PCR[25] |= PORT_PCR_MUX(5);
		
	//set to 100k baud
	//baud = bus freq/(scl_div+mul)
 	//~400k = 24M/(64); icr=0x12 sets scl_div to 64
	// this will be increased to 0x00 after mma initialization
 	I2C0->F = (I2C_F_ICR(0x12) | I2C_F_MULT(0));
	
	//enable i2c and set to master mode
	I2C0->C1 |= (I2C_C1_IICEN_MASK);
	
	// enable i2c interrupts
	//I2C0->C1 |= I2C_C1_IICIE_MASK;
	
	// enable i2c NVIC interrupt
	NVIC_SetPriority(I2C0_IRQn, 128);
	NVIC_ClearPendingIRQ(I2C0_IRQn);
	NVIC_EnableIRQ(I2C0_IRQn);
}

//send start sequence
void i2c_start()
{
	i2cLockedUp = WORKING;	// reset flag
	I2C_TRAN;							/*set to transmit mode */
	I2C_M_START;					/*send start	*/
}

//send device and register addresses
void i2c_read_setup(uint8_t dev, uint8_t address)
{
	I2C0->D = dev;			  	/*send dev address	*/
	I2C_WAIT_LU_CHECK;			/*wait for completion */
	
	I2C0->D =  address;			/*send read address	*/
	I2C_WAIT_LU_CHECK;			/*wait for completion */
		
	I2C_M_RSTART;				   	/*repeated start */
	I2C0->D = (dev|0x1);	 	/*send dev address (read)	*/
	I2C_WAIT_LU_CHECK;			/*wait for completion */
	
	I2C_REC;						   	/*set to recieve mode */

}

//read a byte and ack/nack as appropriate
uint8_t i2c_repeated_read(uint8_t isLastRead)
{
	uint8_t data;
	
	if(isLastRead)	{
		NACK;								/*set NACK after read	*/
	} else	{
		ACK;								/*ACK after read	*/
	}
	
	data = I2C0->D;				/*dummy read	*/
	I2C_WAIT_LU_CHECK_NONVOID;		/*wait for completion */
	
	if(isLastRead)	{
		I2C_M_STOP;					/*send stop	*/
	}
	data = I2C0->D;				/*read data	*/

	return  data;					
}



//////////funcs for reading and writing a single byte
//using 7bit addressing reads a byte from dev:address
uint8_t i2c_read_byte(uint8_t dev, uint8_t address)
{
	uint8_t data;
	
	I2C_TRAN;							/*set to transmit mode */
	I2C_M_START;					/*send start	*/
	I2C0->D = dev;			  /*send dev address	*/
	I2C_WAIT_LU_CHECK_NONVOID;		/*wait for completion */
	
	I2C0->D =  address;		/*send read address	*/
	I2C_WAIT_LU_CHECK_NONVOID;		/*wait for completion */
		
	I2C_M_RSTART;				   /*repeated start */
	I2C0->D = (dev|0x1);	 /*send dev address (read)	*/
	I2C_WAIT_LU_CHECK_NONVOID;		/*wait for completion */
	
	I2C_REC;						   /*set to recieve mode */
	NACK;									 /*set NACK after read	*/
	
	data = I2C0->D;					/*dummy read	*/
	I2C_WAIT_LU_CHECK_NONVOID;		/*wait for completion */
	
	I2C_M_STOP;							/*send stop	*/
	data = I2C0->D;					/*read data	*/

	return data;
}



//using 7bit addressing writes a byte data to dev:address
void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data)
{
	
	I2C_TRAN;								/*set to transmit mode */
	I2C_M_START;						/*send start	*/
	I2C0->D = dev;			  	/*send dev address	*/
	I2C_WAIT_LU_CHECK;			/*wait for completion */
	
	I2C0->D =  address;			/*send write address	*/
	I2C_WAIT_LU_CHECK;			/*wait for completion */
		
	I2C0->D = data;					/*send data	*/
	I2C_WAIT_LU_CHECK;			/*wait for completion */
	I2C_M_STOP;
	
}

void i2c_wait(void){
	int waitCount = 0;
	
	while((I2C0->S & I2C_S_IICIF_MASK)==0) {
		waitCount++;
		if (waitCount > 1000){
			i2cLockedUp = LOCKED_UP;
			return;
		}
	}
	I2C0->S |= I2C_S_IICIF_MASK;
}

// i2c reset to recover from locked i2c bus
void i2c_reset(void){
	// bus is already busy => cancel previous operation
	if (I2C0->S & I2C_S_BUSY_MASK){
		// send start signal
		I2C0->C1 &= ~I2C_C1_IICEN_MASK;			
		I2C0->C1 |= I2C_C1_TX_MASK;					// set transmit mode
		I2C0->C1 |= I2C_C1_MST_MASK;				// start signal generated
		I2C0->C1 |= I2C_C1_IICEN_MASK;
		
		// send write command to release SDA line
		I2C0->C1 |= I2C_C1_MST_MASK;				// set master mode
		I2C0->C1 |= I2C_C1_TX_MASK;					// set tx mode
		I2C0->D = 0xFF;
		while((I2C0->S & I2C_S_IICIF_MASK)==0) {}		// wait interrupt
		I2C0->S |= I2C_S_IICIF_MASK; 				// clear interrupt bit
			
		// clear arbitration error flag
		I2C0->S |= I2C_S_ARBL_MASK;
			
		// send start
		I2C0->C1 &= ~I2C_C1_IICEN_MASK;  
		I2C0->C1 |= I2C_C1_TX_MASK;					// set tx mode
		I2C0->C1 |= I2C_C1_MST_MASK;				// start signal generatred
		I2C0->C1 |= I2C_C1_IICEN_MASK;
		
		// pause
		while(I2C0->S & I2C_S_BUSY_MASK);
			
		// send stop 
    I2C0->C1 &= ~I2C_C1_IICEN_MASK;  
		I2C0->C1 &= ~I2C_C1_MST_MASK;  			// set SLAVE mode 
		I2C0->C1 &= ~I2C_C1_TX_MASK;    		// set Rx
		I2C0->C1 |= I2C_C1_IICEN_MASK;  
			
		while (I2C0->S & I2C_S_BUSY_MASK){
				// wait
		}
		
		// clear arbitration error and interrupt flag
		I2C0->S |= I2C_S_IICIF_MASK;  
    I2C0->S |= I2C_S_ARBL_MASK; 
	}
}

//
// Interrupt based attempt
// Ignore if polling
//
#if POLLING==0
uint8_t i2cCount = 0;
uint8_t i2cState = 0;
uint8_t devAddress;
uint8_t readAddress;
uint8_t data[6];

void I2C0_IRQHandler(){	
	// clear pending irq
	NVIC_ClearPendingIRQ(I2C0_IRQn);
	I2C0->S |= I2C_S_IICIF_MASK;
	
	// transmit
	if (I2C0->C1 & I2C_C1_TX_MASK){
		if (i2cState == READ_SETUP){
			switch (i2cCount++){
				case 0:
					I2C0->D = readAddress;
					break;
				
				case 1:
					I2C_M_RSTART;		// repeated start
					I2C0->D = (devAddress|0x1);
					break;
				
				case 2:
					ACK;					// acknowledge
					I2C_REC;			// set to receive data
					i2cState = REPEATED_READ;
					i2cCount = 0;
					break;
			}
		}
	}
	// receive
	else {
		if (i2cState == REPEATED_READ){
			data[i2cCount] = I2C0->D;			// read byte of data
			
			if (i2cCount < 5){
				ACK;				// acknowledge after every byte
			}
			else if (i2cCount == 5){
				NACK;				// no acknowledge after last byte
				I2C_M_STOP;	// stop
				i2cState = DATA_RECEIVED;				// update state to indicate data is ready for processing
				I2C0->C1 &= ~I2C_C1_IICIE_MASK;	// disable interrupt
			}
			else {		// error
				i2cState = 0;
				I2C0->C1 &= ~I2C_C1_IICIE_MASK;
			}
			i2cCount++;
		}
	}
}

// interrupt i2c process
void i2c_int_start(uint8_t dev, uint8_t address){
	// save dev and read address
	devAddress = dev;
	readAddress = address;
	
	// begin
	I2C_TRAN;										/*set to transmit mode */
	I2C_M_START;								/*send start	*/
	I2C0->C1 |= I2C_C1_IICIE_MASK;		// enable interrupt
	NVIC_EnableIRQ(I2C0_IRQn);
	I2C0->D = devAddress;			  /*send dev address	*/
	i2cState = READ_SETUP;
	i2cCount = 0;
}
#endif
