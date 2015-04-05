#include <MKL25Z4.H>
#include "mma8451.h"
#include "i2c.h"
#include "delay.h"
#include <math.h>
#include "gpio_defs.h"
#include "LEDs.h"

int16_t acc_X=0, acc_Y=0, acc_Z=0;
float roll=0.0, pitch=0.0;
uint8_t byte = 0;

//mma data ready
extern uint32_t DATA_READY;



//initializes mma8451 sensor
//i2c has to already be enabled
int init_mma()
{
	  //check for device
		if(i2c_read_byte(MMA_ADDR, REG_WHOAMI) == WHOAMI)	{
  		Delay(10);
			//set active mode, 14 bit samples and 100 Hz ODR (0x19)
			//i2c_write_byte(MMA_ADDR, REG_CTRL1, 0x01);
			
			// put in standby mode
			byte = i2c_read_byte(MMA_ADDR, 0x2A);
			Delay(10);
			byte &= 0xFE;
			i2c_write_byte(MMA_ADDR, 0x2A, byte);
			Delay(10);
			
			// put in standby mode
			byte = i2c_read_byte(MMA_ADDR, 0x2A);
			Delay(10);
			byte &= 0xC7;
			byte |= 0x20;
			i2c_write_byte(MMA_ADDR, 0x2A, byte);
			Delay(10);
			
			// enable orientation detection
			byte = i2c_read_byte(MMA_ADDR, 0x11);
			Delay(10);
			byte |= 0x40;
			i2c_write_byte(MMA_ADDR, 0x11, byte);
			Delay(10);
			
			// set the back/front angle trip points
			byte = i2c_read_byte(MMA_ADDR, 0x13);
			Delay(10);
			byte &= 0x3F;
			byte |= 0x40;
			i2c_write_byte(MMA_ADDR, 0x13, byte);
			Delay(10);
			
			// set the Z-lockout angle trip point
			byte = i2c_read_byte(MMA_ADDR, 0x1C);
			Delay(10);
			if (byte == 0x00){
				byte = i2c_read_byte(MMA_ADDR, 0x13);
				Delay(10);
			}
			byte &= 0xF8;
			byte |= 0x04;
			i2c_write_byte(MMA_ADDR, 0x13, byte);
			Delay(10);
			
			// set the trip threshold angle
			byte = i2c_read_byte(MMA_ADDR, 0x14);
			Delay(10);
			byte &= 0x07;
			byte |= (0x0C)<<3;
			i2c_write_byte(MMA_ADDR, 0x14, byte);
			Delay(10);
			
			// set the hysteresis angle
			byte = i2c_read_byte(MMA_ADDR, 0x14);
			Delay(10);
			byte &= 0xF8;
			byte |= 0x01;
			i2c_write_byte(MMA_ADDR, 0x14, byte);
			Delay(10);
			
			// enable interrupt detection
			byte = i2c_read_byte(MMA_ADDR, 0x2D);
			Delay(10);
			byte |= 0x10;
			i2c_write_byte(MMA_ADDR, 0x2D, byte);
			Delay(10);
			
			// route the interrupt to INT1
			byte = i2c_read_byte(MMA_ADDR, 0x2E);
			Delay(10);
			byte |= 0x10;
			i2c_write_byte(MMA_ADDR, 0x2E, byte);
			Delay(10);
			
			// set debounce counter
			i2c_write_byte(MMA_ADDR, 0x12, 0x05);
			Delay(10);
			
			// put device in active mode
			byte = i2c_read_byte(MMA_ADDR, 0x2A);
			Delay(10);
			byte |= 0x01;
			i2c_write_byte(MMA_ADDR, 0x2A, byte);
			Delay(10);
			
			return 1;
		}
		//else error
		return 0;
}

void read_full_xyz()
{
	int i;
	uint8_t data[6];
	
	i2c_start();
	i2c_read_setup(MMA_ADDR , REG_XHI);
	
	// Read five bytes in repeated mode
	for( i=0; i<5; i++)	{
		data[i] = i2c_repeated_read(0);
	}
	// Read last byte ending repeated mode
	data[i] = i2c_repeated_read(1);

	// Align for 14 bits
	acc_X = ((int16_t)(data[0]<<8)) | data[1];
	acc_Y = ((int16_t)(data[2]<<8)) | data[3];
	acc_Z = ((int16_t)(data[4]<<8)) | data[5];
}


void read_xyz(void)
{
	// sign extend byte to 16 bits - need to cast to signed since function
	// returns uint8_t which is unsigned
	acc_X = (int8_t) i2c_read_byte(MMA_ADDR, REG_XHI);
	Delay(100);
	acc_Y = (int8_t) i2c_read_byte(MMA_ADDR, REG_YHI);
	Delay(100);
	acc_Z = (int8_t) i2c_read_byte(MMA_ADDR, REG_ZHI);

}

void convert_xyz_to_roll_pitch(void) {
	roll = atan2f(acc_Y, acc_Z)*(180/M_PI);
	pitch = atan2f(acc_X, sqrtf(acc_Y*acc_Y + acc_Z*acc_Z))*(180/M_PI);
	
}

void Init_MMA_Int(void){
	SIM->SCGC5 |=  SIM_SCGC5_PORTA_MASK; /* enable clock for port A */

	/* Select GPIO and enable pull-up resistors and interrupts 
		on falling edges for pins connected to switches */
	PORTA->PCR[14] |= PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x0a);
	
	/* Set port D switch bit to inputs */
	PTA->PDDR &= ~MASK(14);

	/* Enable Interrupts */
	NVIC_SetPriority(PORTA_IRQn, 128); // 0, 64, 128 or 192
	NVIC_ClearPendingIRQ(PORTA_IRQn); 
	NVIC_EnableIRQ(PORTA_IRQn);
}


//mma data ready irq
void PORTA_IRQHandler()
{
 	NVIC_ClearPendingIRQ(PORTA_IRQn);
	Control_RGB_LEDs(1,0,0);
}
