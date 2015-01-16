/*
 * sysinit.h
 *
 *  Created on: Mar 28, 2011
 *      Author: RXAA60
 */

#ifndef SYSINIT_H_
#define SYSINIT_H_

void __initialize_hardware(void);
void fb_clk_init(void);
void clkout_init(unsigned char clockout_select, unsigned char clkoutdiv);
extern void uart_init (UART_MemMapPtr , int , int );
void outsrs(void);


#endif /* SYSINIT_H_ */
