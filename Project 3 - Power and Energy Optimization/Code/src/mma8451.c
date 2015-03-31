#include <MKL25Z4.H>
#include "mma8451.h"
#include "i2c.h"
#include "delay.h"
#include "approx.h"
#include "my_math.h"
#include <math.h>

int16_t acc_X=0, acc_Y=0, acc_Z=0;
float roll=0.0, pitch=0.0;

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
			i2c_write_byte(MMA_ADDR, REG_CTRL1, 0x01);
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
	roll = approx_atan2f(acc_Y, acc_Z)*(180/M_PI);
	pitch = approx_atan2f(acc_X, approx_sqrtf(acc_Y*acc_Y + acc_Z*acc_Z))*(180/M_PI);
}


//mma data ready irq
// void PORTA_IRQHandler()
// {
// 	NVIC_ClearPendingIRQ(PORTA_IRQn);
// 	DATA_READY = 1;	
// }
