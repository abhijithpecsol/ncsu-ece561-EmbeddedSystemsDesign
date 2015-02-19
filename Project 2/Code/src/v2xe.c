/*----------------------------------------------------------------------------
 *
 * v2xe.c
 * Michael Meli
 *
 * Get magnetometer data from the V2Xe compass device.
 *
 *----------------------------------------------------------------------------*/

#include <MKL25Z4.h>
#include "spi.h"
#include "v2xe.h"
#include "delay.h"
#include "LEDs.h"

extern float roll, pitch;
int32_t mag_X=0, mag_Y=0, mag_Z=0;

// get data from v2xe
void get_v2xe_data(V2XEData * data){
	uint8_t index, count, type, buffer[64];
	uint8_t *ptr;
	index = 0;
	
	// send SendDataComponents command to get all the data in the order
	buffer[index++] = kSyncChar;							// frame starts with sync character
	buffer[index++] = kSetDataComponents;			// frame type
	buffer[index++] = 2;											// number of components to receive
	buffer[index++] = kRawX;									// raw x sensor value
	buffer[index++] = kRawY;									// raw y sensor value
	//buffer[index++] = kCalibratedX;						// calibrated x value (not yet calibrated?)
	//buffer[index++] = kCalibratedY;						// calibrated y value (not yet calibrated?)
	//buffer[index++] = kHeading;								// heading, based off calibration values
	buffer[index++] = kTerminator;						// terminator
	
	// transmit command
	count = index;
	index = 0;
	while (count--){
		// received data will be 0x00 while transmitting, so throw away
		spi_transmit(buffer[index++]);
	}
	
	// transmit request for data
	index = 0;
	buffer[index++] = kSyncChar;
	buffer[index++] = kGetData;
	buffer[index++] = kTerminator;
	count = index;
	index = 0;
	while (count--){
		spi_transmit(buffer[index++]);
	}
	
	// poll response to wait for sync char, indicating beginning of response
	// whatever is transmitted will be ignored by the v2xe
	Delay(1);
	while (spi_transmit(0x00) != kSyncChar);
	
	// next byte will be the response frame type
	type = spi_transmit(0x00);
	if (type == kDataResp)
	{
		// next byte will be the data component count
		count = spi_transmit(0x00);
		
		// get data pieces
		while(count--){
			// get the component data identifier
			type = spi_transmit(0x00);
			
			switch(type){
				case kRawX:
					// point ptr to where to store data
					ptr = (uint8_t*)&data->x;
					*ptr++ = spi_transmit(0x00);		// x is 4 bytes...
					*ptr++ = spi_transmit(0x00);
					*ptr++ = spi_transmit(0x00);
					*ptr++ = spi_transmit(0x00);
					break;
				
				case kRawY:
					ptr = (uint8_t*)&data->y;
					*ptr++ = spi_transmit(0x00);		// y is 4 bytes...
					*ptr++ = spi_transmit(0x00);
					*ptr++ = spi_transmit(0x00);
					*ptr++ = spi_transmit(0x00);
					break;
				
				case kCalibratedX:
					ptr = (uint8_t*)&data->xe;
					*ptr++ = spi_transmit(0x00);		
					*ptr++ = spi_transmit(0x00);
					*ptr++ = spi_transmit(0x00);
					*ptr++ = spi_transmit(0x00);
					break;					
				
				case kCalibratedY:
					ptr = (uint8_t*)&data->ye;
					*ptr++ = spi_transmit(0x00);		
					*ptr++ = spi_transmit(0x00);
					*ptr++ = spi_transmit(0x00);
					*ptr++ = spi_transmit(0x00);
					break;	
				
				case kHeading:
					ptr = (uint8_t*)&data->heading;
					*ptr++ = spi_transmit(0x00);		// y is 4 bytes...
					*ptr++ = spi_transmit(0x00);
					*ptr++ = spi_transmit(0x00);
					*ptr++ = spi_transmit(0x00);
					break;	
				
				default:
					// error condition
					Control_RGB_LEDs(1, 0, 0);
					break;
			}
		}
	}
	else {
		// error condition
		Control_RGB_LEDs(1, 0, 0);
		//while(1);
	}
}

// calibrate magnetometer
void calibrate_v2xe(){
	// todo - do 
}

// calculate tilt-compensated heading
void calculate_tilt_compensated_heading(void){
	// todo - do
}
