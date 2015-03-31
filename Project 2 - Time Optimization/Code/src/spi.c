/*----------------------------------------------------------------------------
 *
 * spi.c
 * Michael Meli
 *
 * Handle configuration/sending/receive for spi
 *
 *----------------------------------------------------------------------------*/

#include <MKL25Z4.h>
#include "spi.h"

// init spi
void spi_init(){
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;				// clock port C
	SIM->SCGC4 |= SIM_SCGC4_SPI0_MASK;				// clock spi0
	
	PORTC->PCR[4] = PORT_PCR_MUX(2);					// PTC4 -> SPI0_PCS0
	PORTC->PCR[5] = PORT_PCR_MUX(2);					// PTC5 -> SPI0_SCK
	PORTC->PCR[6] = PORT_PCR_MUX(2);					// PTC6 -> SPI0_MOSI
	PORTC->PCR[7] = PORT_PCR_MUX(2);					// PTC7 -> SPI0_MISO
	
	SPI0->C1 = SPI_C1_MSTR_MASK | SPI_C1_SSOE_MASK;		// set master mode and automatic slave select
	SPI0->C2 = SPI_C2_MODFEN_MASK;										// master SS pin acts as slave select output automatically
	
	//SPI0->BR = (SPI_BR_SPPR(0x00) | SPI_BR_SPR(0x00));		// 1.5 MHz clock hopefully
	SPI0->BR = (SPI_BR_SPPR(0x02) | SPI_BR_SPR(0x08));

	SPI0->C1 |= SPI_C1_SPE_MASK;							// enable spi
}

// transmit through SPI and return the byte received
uint8_t spi_transmit(uint8_t byte){
	// send
	while(!(SPI0->S & SPI_S_SPTEF_MASK)){}		// wait for transmit buffer to empty
	SPI0->D = byte;				// send byte
		
	// receive
	while(!(SPI0->S & SPI_S_SPRF_MASK)){}			// wait for read buffer to fill
	byte = SPI0->D;
	return byte;
}
