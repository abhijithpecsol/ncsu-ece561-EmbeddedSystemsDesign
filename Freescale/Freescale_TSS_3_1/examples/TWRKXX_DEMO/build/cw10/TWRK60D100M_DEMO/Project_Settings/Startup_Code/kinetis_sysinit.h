/*
  FILE    : kinetis_sysinit.h
  PURPOSE : system initialization header for Kinetis ARM architecture
  LANGUAGE: C
  Copyright © 2012 Freescale semiConductor Inc. All Rights Reserved.
*/
#ifndef KINETIS_SYSINIT_H
#define KINETIS_SYSINIT_H

int pll_init(int crystal_val, unsigned char hgo_val, unsigned char erefs_val, signed char prdiv_val, signed char vdiv_val, unsigned char mcgout_select);

/* System Info */
#define CPU_MK60D10
#define CLK0_FREQ_HZ        50000000
#define CLK0_TYPE           CANNED_OSC
#define PLL0_PRDIV          25       /* divider eference by 25 = 2 MHz */
#define PLL0_VDIV           50       /* multiply reference by 50 = 100 MHz */
#if !(defined(SYNCH_MODE) | defined(NO_PLL_INIT))
  #define ASYNCH_MODE    /* PLL1 is source for MCGCLKOUT and DDR controller */
#endif


// Constants for use in pll_init
#define NO_OSCINIT 0
#define OSCINIT 1

#define OSC_0 0
#define OSC_1 1

#define LOW_POWER 0
#define HIGH_GAIN 1

#define CANNED_OSC  0
#define CRYSTAL 1

#define PLL_0 0
#define PLL_1 1

#define PLL_ONLY 0
#define MCGOUT 1

// MCG Mode defines

#define BLPI 1
#define FBI  2
#define FEI  3
#define FEE  4
#define FBE  5
#define BLPE 6
#define PBE  7
#define PEE  8

// IRC defines
#define SLOW_IRC 0
#define FAST_IRC 1

#ifdef __cplusplus
extern "C" {
#endif

/* Unlocking Watchdog sequence words*/
#define KINETIS_WDOG_UNLOCK_SEQ_1	0xC520
#define KINETIS_WDOG_UNLOCK_SEQ_2	0xD928

/* Word to be written in in STCTRLH after unlocking sequence in order to disable the Watchdog */
#define KINETIS_WDOG_DISABLED_CTRL	0xD2

/* 
	Initializes the Kinetis hardware: e.g. disables the Watchdog
*/
void __init_hardware();

/*
** ===================================================================
**     Method      :  isr_default
**
**     Description :
**         The default ISR.
** ===================================================================
*/
void isr_default(void);

/*
** ===================================================================
**     Method      :  isrINT_NMI
**
**     Description :
**         This ISR services the Non Maskable Interrupt interrupt.
** ===================================================================
*/
void isrINT_NMI(void);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef KINETIS_SYSINIT_H */
