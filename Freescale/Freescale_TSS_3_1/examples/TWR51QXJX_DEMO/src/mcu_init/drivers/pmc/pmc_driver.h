/*
 * File:		pmc.h
 * Purpose:     Provides low power mode entry routines
 *
 * Notes:
 */

#ifndef __PMC_H__
#define __PMC_H__

/********************************************************************/

// function prototypes
void outSRS(void);
void outsrs(void);
void enter_wait(void);
void enter_stop(void);
void enter_vlpr(void);
void exit_vlpr(void);
void enter_vlps(void);
void enter_lls(void);
void enter_vlls3(void);
void enter_vlls2(void);
void enter_vlls1(void);

void enable_lpwui(void);
void disable_lpwui(void);
void disable_io(void);

// function prototypes for MCG functions that will be moved eventually
extern void mcg_fei_2_blpi(void);
extern void mcg_blpi_2_fei(void);
extern void mcg_blpi_2_pee(void);

/********************************************************************/
#endif /* __PMC_H__ */
