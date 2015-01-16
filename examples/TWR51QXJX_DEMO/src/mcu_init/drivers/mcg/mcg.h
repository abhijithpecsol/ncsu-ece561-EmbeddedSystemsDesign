/*
 * File:    pll_init.h
 * Purpose: pll_driver specific declarations
 *
 * Notes:
 */
#ifndef __MCG_H__
#define __MCG_H__
/********************************************************************/

unsigned char pll_init(unsigned char, unsigned char);
void pbe_pee(void);
void mcg_blpi_2_pee(void);
void mcg_blpi_2_fei(void);
void mcg_blpi_2_pee(void);
void mcg_fei_2_blpi(void);
void mcg_pee_2_blpi(void);
void mcg_blpi_2_pee(void);
void mcg_pbe_2_pee(void);

void atc (unsigned int slow_fast, unsigned long int frequency);
extern void uart_init (UART_MemMapPtr , int , int );



enum clk_option
{
  PLL50,
  PLL100,
  PLL96,
  PLL48
};

enum crystal_val
{
  XTAL2,
  XTAL4,
  XTAL6,
  XTAL8,
  XTAL10,
  XTAL12,
  XTAL14,
  XTAL16,
  XTAL18,
  XTAL20,
  XTAL22,
  XTAL24,
  XTAL26,
  XTAL28,
  XTAL30,
  XTAL32
};




/********************************************************************/
#endif /* __MCG_H__ */
