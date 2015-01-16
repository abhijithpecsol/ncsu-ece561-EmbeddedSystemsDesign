/*
 * File:        pmc_driver.c
 * Purpose:     Provides routines for entering low power modes.
 *
 * Notes:	Since the wakeup mechanism for low power modes
 *              will be application specific, these routines
 *              do not include code to setup interrupts to exit
 *              from the low power modes. The desired means of
 *              low power mode exit should be configured before
 *              calling any of these functions.
 *
 *              These routines do not include protection to
 *              prevent illegal state transitions in the mode
 *              controller, and all routines that write to the
 *              PMPROT register write a value to allow all
 *              possible low power modes (it is write once, so
 *              if only the currently requested mode is enabled
 *              a different mode couldn't be enabled later on).
 *              
 */

#include "common.h"
#include "pmc_driver.h"
#include "mcg.h"

/* Globals from sysinit.c */
extern int core_clk_khz;
extern int core_clk_mhz;
extern int periph_clk_khz;

/* OutSRS routine - checks the value in the SRS registers and sends
 * messages to the terminal announcing the status at the start of the 
 * code.
 */
void outSRS(void){                         //[outSRS]
	if (RCM_SRS1 & RCM_SRS1_SACKERR_MASK)
         print_str("[outSRS]Stop Mode Acknowledge Error Reset\n");
	if (RCM_SRS1 & RCM_SRS1_EZPT_MASK)
         print_str("[outSRS]EZPORT Reset\n");
	if (RCM_SRS1 & RCM_SRS1_BDFR_MASK)
         print_str("[outSRS]Background Debug Force Reset\n");
	if (RCM_SRS0 & RCM_SRS0_POR_MASK)
		print_str("[outSRS]Power-on Reset\n");
	if (RCM_SRS0 & RCM_SRS0_PIN_MASK)
         print_str("[outSRS]External Pin Reset\n");
	if (RCM_SRS0 & RCM_SRS0_WDOG_MASK)
         print_str("[outSRS]Watchdog Reset\n");
	if (RCM_SRS0 & RCM_SRS0_ILOP_MASK)
         print_str("[outSRS]Illegal Opcode Reset\n");
	if (RCM_SRS0 & RCM_SRS0_ILAD_MASK)
         print_str("[outSRS]Illegal Address Reset\n");
	if (RCM_SRS0 & RCM_SRS0_LOC_MASK)
         print_str("[outSRS]MCG Out of Lock Reset\n");
	if (RCM_SRS0 & RCM_SRS0_LVD_MASK)
         print_str("[outSRS]LVD Reset\n");
	if (RCM_SRS0 & RCM_SRS0_WAKEUP_MASK){
	     print_str("[outSRS]Wakeup bit set from low power mode exit\n");
	 	 print_str("[outSRS]SMC_PMPROT   = %#02X \r\n", (SMC_PMPROT))  ;
	 	 print_str("[outSRS]SMC_PMCTRL   = %#02X \r\n", (SMC_PMCTRL))  ;
	 	 print_str("[outSRS]SMC_VLLSCTRL   = %#02X \r\n", (SMC_VLLSCTRL))  ;
	 	 print_str("[outSRS]SMC_PMSTAT   = %#02X \r\n", (SMC_PMSTAT))  ;

	       if ((SMC_PMCTRL & SMC_PMCTRL_STOPM_MASK)== 3){
	         	print_str("[outSRS] LLS exit \n") ;
	       }
	       if (((SMC_PMCTRL & SMC_PMCTRL_STOPM_MASK)== 4) &
	    		   ((SMC_VLLSCTRL & SMC_VLLSCTRL_VLLSM_MASK)== 1)){
	         	print_str("[outSRS] VLLS1 exit \n") ;
	       }
	       if (((SMC_PMCTRL & SMC_PMCTRL_STOPM_MASK)== 4) &
	    		   ((SMC_VLLSCTRL & SMC_VLLSCTRL_VLLSM_MASK)== 2)){
	        	print_str("[outSRS] VLLS2 exit \n") ;
	       }
	       if (((SMC_PMCTRL & SMC_PMCTRL_STOPM_MASK)== 4) &
	    		   ((SMC_VLLSCTRL & SMC_VLLSCTRL_VLLSM_MASK)== 3)){
	       		print_str("[outSRS] VLLS3 exit \n") ; 
	       }
	    
	}
  }

/********************************************************************/
/* WAIT mode entry routine. Puts the processor into wait mode.
 * In this mode the core clock is disabled (no code executing), but 
 * bus clocks are enabled (peripheral modules are operational). 
 *
 * Mode transitions:
 * RUN -> WAIT
 * VLPR -> VLPW
 *
 * This function can be used to enter normal wait mode or VLPW
 * mode. If you are executing in normal run mode when calling this
 * function, then you will enter normal wait mode. If you are in VLPR
 * mode when calling this function, then you will enter VLPW mode instead.
 *
 * NOTE: Some modules include a programmable option to disable them in 
 * wait mode. If those modules are programmed to disable in wait mode, 
 * they will not be able to generate interrupts to wake up the core.
 *
 * WAIT mode is exited using any enabled interrupt or RESET, so no
 * exit_wait routine is needed. 
 *
 * If in VLPW mode, the statue of the SMC_PMCTRL[LPWUI] bit determines if 
 * the processor exits to VLPR (LPWUI cleared) or normal run mode (LPWUI 
 * set). The enable_lpwui() and disable_lpwui()functions can be used to set
 * this bit to the desired option prior to calling enter_wait().
 * 
 * 
 * Parameters:
 * none
 */
void enter_wait(void)
{
    SIM_SOPT4 = SIM_SOPT4_WAITE_MASK;
    _Wait; 
}
/********************************************************************/
/* STOP mode entry routine. Puts the processor into normal stop mode.
 * In this mode core, bus and peripheral clocks are disabled.
 *
 * Mode transitions:
 * RUN -> STOP
 * VLPR -> VLPS
 *
 * This function can be used to enter normal stop mode or VLPS
 * mode. If you are executing in normal run mode when calling this
 * function, then you will enter normal stop mode. If you are in VLPR
 * mode when calling this function, then you will enter VLPS mode instead.
 *
 * STOP mode is exited using any enabled interrupt or RESET, so no
 * exit_stop routine is needed.
 *
 * If in VLPS mode, the state of the SMC_PMCTRL[LPWUI] bit determines if 
 * the processor exits to VLPR (LPWUI cleared) or normal run mode (LPWUI 
 * set). The enable_lpwui() and disable_lpwui()functions can be used to set
 * this bit to the desired option prior to calling enter_wait().
 *
 * Parameters:
 * none
 */
void enter_stop(void)
{
    /* Set the VLLSM field to 0b000 for normal STOP mode - Need to retain state of LPWUI bit 8 */
    SMC_PMCTRL = (SMC_PMCTRL && SMC_PMCTRL_LPWUI_MASK);    // retain LPWUI
    SMC_PMCTRL |=  SMC_PMCTRL_STOPM(0x0);                 // set VLLSM = 0b00
    SIM_SOPT4 = SIM_SOPT4_STOPE_MASK;  //enable stop mode entry

    _Stop;
}
/********************************************************************/
/* VLPR mode entry routine. Puts the processor into very low power
 * run mode. In this mode all clocks are enabled, but the core, bus,
 * and peripheral clocks are limited to 2MHz or less. The flash 
 * clock is limited to 1MHz or less. 
 *
 * Mode transitions:
 * RUN -> VLPR
 *
 * exit_vlpr() function or an interrupt with LPWUI set can be used 
 * to switch from VLPR back to RUN. The enable_lpwui() and disable_lpwui()
 * functions can be used to set LPWUI to the desired option prior to 
 * calling enter_vlpr().
 *
 * Parameters:
 * none
 */
void enter_vlpr(void)
{
    /* Write to PMPROT to allow all possible power modes */
    SMC_PMPROT = SMC_PMPROT_AVLP_MASK;
    
    /* Reduce system clock to < 2MHz */
    mcg_pee_2_blpi();
    
    /* Set the RUNM field to 0b10 for VLPR mode - Need to retain state of LPWUI bit 8 */
    SMC_PMCTRL =  ((SMC_PMCTRL_RUNM(2)) | (SMC_PMCTRL & SMC_PMCTRL_LPWUI_MASK));
                 // set RUNM = 0b10

    /* Wait for VLPS regulator mode to be confirmed */
    while(!(PMC_REGSC & PMC_REGSC_REGONS_MASK)){
    }
}
/********************************************************************/
/* VLPR mode exit routine. Puts the processor into normal run mode
 * from VLPR mode. You can transition from VLPR to normal run using
 * this function or an interrupt with LPWUI set. The enable_lpwui() 
 * and disable_lpwui() functions can be used to set LPWUI to the 
 * desired option prior to calling enter_vlpr().
 *
 * Mode transitions:
 * VLPR -> RUN
 *
 * Parameters:
 * none
 */
void exit_vlpr(void)
{
    /* Clear RUNM */
    SMC_PMCTRL &= ~(SMC_PMCTRL_RUNM(0x3));
                   
    /* Wait for normal RUN regulation mode to be confirmed */                   
    while(!(PMC_REGSC & PMC_REGSC_REGONS_MASK)){
    }
    
    /* Transition MCG back to the PLL enabled state */
    if (RFSYS_REG0 == 2 )           //saved off mode of mcg 2 = pee, 1 = fei both 50 Mhz
       mcg_blpi_2_pee();
    if (RFSYS_REG0 == 1)   
       mcg_blpi_2_fei(); 
    
}
/********************************************************************/
/* VLPS mode entry routine. Puts the processor into VLPS mode directly
 * from normal run mode. 
 *
 * Mode transitions:
 * RUN -> VLPS
 *
 * If VLPS mode is entered directly from normal RUN mode, then the 
 * LPWUI bit is forced to 1 by hardware. This means that when an
 * interrupt occurs you will exit to normal run mode instead of VLPR.
 *
 * If however VLPS mode is entered from VLPR the state of the LPWUI bit
 * determines the state the MCU will return to upon exit from VLPS.If LPWUI is set
 * and an interrupt occurs you will exit to normal run mode instead of VLPR. 
 * If LPWUI is clear and an interrupt occurs you will exit to VLPR.
 *
 * Parameters:
 * none
 */
void enter_vlps(void)
{
    SIM_SOPT4 = SIM_SOPT4_STOPE_MASK;

    /* Write to PMPROT to allow all possible power modes */
    SMC_PMPROT = SMC_PMPROT_AVLP_MASK;
           
    /* Set the STOPM field to 0b010 for VLPS mode - Need to retain state of LPWUI bit 8 */
    SMC_PMCTRL = ((SMC_PMCTRL & SMC_PMCTRL_LPWUI_MASK) | SMC_PMCTRL_STOPM(0x2));           
    // set STOPM = 0b10
        
    /* Wait for VLPS regulator mode to be confirmed */
    while(!(PMC_REGSC & PMC_REGSC_REGONS_MASK)){
    }
	   
    SIM_SOPT4 = SIM_SOPT4_STOPE_MASK;  //enable stop mode entry
   
    /* Now execute the stop instruction to go into VLPS */
    _Stop;
}
/********************************************************************/
/* LLS mode entry routine. Puts the processor into LLS mode from
 * normal run mode or VLPR. 
 *
 * Mode transitions:
 * RUN -> LLS
 * VLPR -> LLS
 *
 * NOTE: LLS mode will always exit to RUN mode even if you were 
 * in VLPR mode before entering LLS.
 *
 * Wakeup from LLS mode is controlled by the LLWU module. Most
 * modules cannot issue a wakeup interrupt in LLS mode, so make
 * sure to setup the desired wakeup sources in the LLWU before 
 * calling this function.
 *
 * Parameters:
 * none
 */
void enter_lls(void)
{
    SIM_SOPT4 = SIM_SOPT4_STOPE_MASK;

    /* Write to PMPROT to allow all possible power modes */
    SMC_PMPROT = SMC_PMPROT_ALLS_MASK;
        
    /* Set the VLLSM field to 0b011 for LLS mode - Need to retain state of LPWUI bit 8 */
    SMC_PMCTRL = SMC_PMCTRL_STOPM(0x3)| (SMC_PMCTRL & SMC_PMCTRL_LPWUI_MASK);  // retain LPWUI
	   
    SIM_SOPT4 = SIM_SOPT4_STOPE_MASK;  //enable stop mode entry
    _Stop;
    
}
/********************************************************************/
/* VLLS3 mode entry routine. Puts the processor into VLLS3 mode from
 * normal run mode or VLPR. 
 *
 * Mode transitions:
 * RUN -> VLLS3
 * VLPR -> VLLS3
 *
 * NOTE: VLLSx modes will always exit to RUN mode even if you were 
 * in VLPR mode before entering VLLSx.
 *
 * Wakeup from VLLSx mode is controlled by the LLWU module. Most
 * modules cannot issue a wakeup interrupt in VLLSx mode, so make
 * sure to setup the desired wakeup sources in the LLWU before 
 * calling this function.
 *
 * Parameters:
 * none  
 */
void enter_vlls3(void)
{
    SIM_SOPT4 = SIM_SOPT4_STOPE_MASK;

    /* Write to PMPROT to allow all possible power modes */
    SMC_PMPROT =  SMC_PMPROT_AVLLS_MASK;
        
    /* Set the VLLSM field to 0b100 for VLLS3 mode - Need to retain state of LPWUI bit 8 */
    SMC_PMCTRL =  SMC_PMCTRL_STOPM(0x4);           //  Set STOPM = 4
    SMC_PMCTRL |= (SMC_PMCTRL & SMC_PMCTRL_LPWUI_MASK);  // retain LPWUI

    SMC_VLLSCTRL =  SMC_VLLSCTRL_VLLSM(3);           // set VLLSM = 0b11
	   
    SIM_SOPT4 = SIM_SOPT4_STOPE_MASK;  //enable stop mode entry
    _Stop;
}
/********************************************************************/
/* VLLS2 mode entry routine. Puts the processor into VLLS2 mode from
 * normal run mode or VLPR. 
 *
 * Mode transitions:
 * RUN -> VLLS2
 * VLPR -> VLLS2
 *
 * NOTE: VLLSx modes will always exit to RUN mode even if you were 
 * in VLPR mode before entering VLLSx.
 *
 * Wakeup from VLLSx mode is controlled by the LLWU module. Most
 * modules cannot issue a wakeup interrupt in VLLSx mode, so make
 * sure to setup the desired wakeup sources in the LLWU before 
 * calling this function.
 *
 * Parameters:
 * none  
 */
void enter_vlls2(void)
{
    SIM_SOPT4 = SIM_SOPT4_STOPE_MASK;

    /* Write to PMPROT to allow all possible power modes */
    SMC_PMPROT = SMC_PMPROT_AVLLS_MASK;
        
    /* Set the VLLSM field to 0b100 for VLLS2 mode - Need to retain state of LPWUI bit 8 */
    SMC_PMCTRL =  SMC_PMCTRL_STOPM(0x4);           //  Set STOPM = 4
    SMC_PMCTRL |= (SMC_PMCTRL & SMC_PMCTRL_LPWUI_MASK);  // retain LPWUI

    SMC_VLLSCTRL =  SMC_VLLSCTRL_VLLSM(2);           // set VLLSM = 0b10
        
    SIM_SOPT4 = SIM_SOPT4_STOPE_MASK;  //enable stop mode entry
    _Stop;
}
/********************************************************************/
/* VLLS1 mode entry routine. Puts the processor into VLLS1 mode from
 * normal run mode or VLPR. 
 *
 * Mode transitions:
 * RUN -> VLLS1
 * VLPR -> VLLS1
 *
 * NOTE: VLLSx modes will always exit to RUN mode even if you were 
 * in VLPR mode before entering VLLSx.
 *
 * Wakeup from VLLSx mode is controlled by the LLWU module. Most
 * modules cannot issue a wakeup interrupt in VLLSx mode, so make
 * sure to setup the desired wakeup sources in the LLWU before 
 * calling this function.
 *
 * Parameters:
 * none  
 */
void enter_vlls1(void)
{
    SIM_SOPT4 = SIM_SOPT4_STOPE_MASK;

    /* Write to PMPROT to allow all possible power modes */
    SMC_PMPROT =  SMC_PMPROT_AVLLS_MASK;
        
    /* Set the VLLSM field to 0b100 for VLLS1 mode - Need to retain state of LPWUI bit 8 */
    SMC_PMCTRL =  SMC_PMCTRL_STOPM(0x4);           //  Set STOPM = 4
    SMC_PMCTRL |= (SMC_PMCTRL & SMC_PMCTRL_LPWUI_MASK);  // retain LPWUI

    SMC_VLLSCTRL =  SMC_VLLSCTRL_VLLSM(1);           // set VLLSM = 0b01
        
    SIM_SOPT4 = SIM_SOPT4_STOPE_MASK;  //enable stop mode entry
    _Stop;
}
/********************************************************************/
/* Enable low power wake up on interrupt. This function can be used
 * to set the LPWUI bit. When this bit is set VLPx modes will exit
 * to normal run mode. When this bit is cleared VLPx modes will exit
 * to VLPR mode.
 *
 * The disable_lpwui() function can be used to clear the LPWUI bit.
 *
 * Parameters:
 * none
 */
void enable_lpwui(void)
{
    SMC_PMCTRL |= SMC_PMCTRL_LPWUI_MASK;
}
/********************************************************************/
/* Disable low power wake up on interrupt. This function can be used
 * to clear the LPWUI bit. When this bit is set VLPx modes will exit
 * to normal run mode. When this bit is cleared VLPx modes will exit
 * to VLPR mode.
 *
 * The enable_lpwui() function can be used to set the LPWUI bit.
 *
 * Parameters:
 * none
 */
void disable_lpwui(void)
{
    SMC_PMCTRL &= ~SMC_PMCTRL_LPWUI_MASK;
}

/********************************************************************/
/*  disable the i/o pin */
void disable_io(void){
	MXC_PTAPF2 = MXC_PTAPF2_A5(00);  //disable clock out mux
    MXC_PTBPF4 = MXC_PTBPF4_B1(00);  //disable fb_clock
	if (TERM_PORT == UART0_BASE_PTR)
	{
		#ifdef twrjf
        // Disable Port Muxing of Uart0, TXD on PTA7 and RXD on PTD6
        MXC_PTAPF1 = MXC_PTAPF1_A7(0); //UART0_TX
        MXC_PTDPF1 = MXC_PTDPF1_D6(0); //UART0_RX
        #else  // JF cpu CARD
		// Disable Port Muxing of Uart0, TXD on PTC6 and RXD on PTC7
    	MXC_PTCPF1 = (MXC_PTCPF1_C7(02) | MXC_PTCPF1_C6(00));  // UART0_TX  disabled RX is wakeup
        #endif
	}
	if (TERM_PORT == UART1_BASE_PTR)
	{
		// Disable Port Muxing of Uart1, TXD on PTA2 and RXD on PTA3
		MXC_PTAPF3 = MXC_PTAPF3_A2(0)| MXC_PTAPF3_A3(0);
	}
	   
}
void outsrs(void){
	char i;
	/* OutSRS routine - checks the value in the SRS registers and sends
	 * messages to the terminal announcing the status at the start of the 
	 * code.
	 */
	PCTLA_DS = 0x03;
	SIM_SCGC6 = SIM_SCGC6_PORTA_MASK;
	MXC_PTAPF4 = MXC_PTAPF4_A0(1)| MXC_PTAPF4_A1(1); //PTA0 A1 - output
	PTA_D = 0x00;  // output low
	             //  PTA1 indicates output of SRS1 or 0 if 
			     //  =1 output is SRS1 i 
	             //  =0 output is SRS0
	PTA_DD = 0x03; // PTA0 as output
	PTA_D = 0x01 ;   // output Preamble to SRS output
	PTA_D = 0x02 ;   // output 
	PTA_D = 0x03 ;   // output 
	PTA_D = 0x00 ;   // output 
	//	void outSRS(void){                         //[outSRS]
		if (RCM_SRS0 & RCM_SRS0_POR_MASK>>7){
			for (i=0;i<8;i++){
			PTA_D = ((RCM_SRS0 & RCM_SRS0_POR_MASK)>>7);   
	    	PTA_D = 0x00;
			}
		}
		if (RCM_SRS0 & RCM_SRS0_PIN_MASK){
			for (i=0;i<7;i++){
			PTA_D = ((RCM_SRS0 & RCM_SRS0_PIN_MASK)>>6);   
	    	PTA_D = 0x00;
			}
		}
		if (RCM_SRS0 & RCM_SRS0_WDOG_MASK){
			for (i=0;i<6;i++){
			PTA_D = ((RCM_SRS0 & RCM_SRS0_WDOG_MASK)>>5);   
	    	PTA_D = 0x00;
			}
		}
		if (RCM_SRS0 & RCM_SRS0_ILOP_MASK){
			for (i=0;i<5;i++){
			PTA_D = ((RCM_SRS0 & RCM_SRS0_ILOP_MASK)>>4);   
	    	PTA_D = 0x00;
			}
		}
		if (RCM_SRS0 & RCM_SRS0_ILAD_MASK){
			for (i=0;i<4;i++){
			PTA_D = ((RCM_SRS0 & RCM_SRS0_ILAD_MASK)>>3);   
	    	PTA_D = 0x00;
			}
		}
		if (RCM_SRS0 & RCM_SRS0_LOC_MASK){
			for (i=0;i<3;i++){
			PTA_D = ((RCM_SRS0 & RCM_SRS0_LOC_MASK)>>2);   
	    	PTA_D = 0x00;
			}
		}
		if (RCM_SRS0 & RCM_SRS0_LVD_MASK){
			for (i=0;i<2;i++){
			PTA_D = ((RCM_SRS0 & RCM_SRS0_LVD_MASK)>>1);   
	    	PTA_D = 0x00;
			}
		}
		if (RCM_SRS0 & RCM_SRS0_WAKEUP_MASK){
			for (i=0;i<1;i++){
			PTA_D = (RCM_SRS0 & RCM_SRS0_WAKEUP_MASK);   
	    	PTA_D = 0x00;
			}		    
		}
		PTA_D = 0x02 ;   // output SRS1
		if (RCM_SRS1 & RCM_SRS1_SACKERR_MASK){
			for (i=0;i<5;i++){
			PTA_D = (((RCM_SRS1 & RCM_SRS1_SACKERR_MASK)>>5)| 0x02);   
	    	PTA_D = 0x02;
			}
			}
		if (RCM_SRS1 & RCM_SRS1_EZPT_MASK){
			for (i=0;i<4;i++){
	        PTA_D = (((RCM_SRS1 & RCM_SRS1_EZPT_MASK)>>4)| 0x02);   
	        PTA_D = 0x02;
			}
		}
		if (RCM_SRS1 & RCM_SRS1_BDFR_MASK){
			for (i=0;i<3;i++){
			PTA_D = (((RCM_SRS1 & RCM_SRS1_BDFR_MASK)>>3)| 0x02);   
	    	PTA_D = 0x02;
			}
		}
}
