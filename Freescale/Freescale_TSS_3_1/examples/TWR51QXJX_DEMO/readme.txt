TWR51QXJX_DEMO
==================================================================

This folder contains demo presenting TSS with the Coldfire+ plaform 
MCF51JF128 and MCF51QM128 on the TWR-MCF51JF and TWR-MCF51QM board. 
The demo presents touch sensing function by 2 onboard electrodes, 
daughter board TWRPI-KEYPAD (6 electrodes), TWRPI-ROTARY (6 electrodes),
TWRPI-SLIDER(5 electrodes), TWRPI-ROTARY2(4 electrodes), 
TWRPI-SHIELD1 (6 electrodes), TWRPI-SHIELD2 (4 electrodes), TWRPI-KEYPAD2 
(12 keys done by 6 multiplexed electrodes) and TWRPI-PROXIMITY (4 keys 
and 1 proximity electrode). Four kinds of controls (KEYPAD, ROTARY, 
ANALOGUE SLIDER, ANALOGUE ROTARY) are presented. The sensing is performed 
by TSI module inside MCU.

Features & Information:
------------------------------------------------------------------
- TWR-MCF51JF tower board, MCF51JF128 
- TWR-MCF51QM tower board, MCF51QM128 
- CodeWarrior for MCU v 10.1 or higher supported
- TSI module performing sensing function
- FreeMASTER Visualization GUI supported via serial connection provided by 
  TWR-SER board connected with elevator boards 
- BUSclk 25MHz used
- Onboard OpenSource BDM (OSBDM) programmer available
- BDM programing port available
- Wakeup from Low Power MCU mode function presented
- Triggering function

TWR-MCF51JF128 Jumper Configuration 
(The settings listed indicates the “on” (or installed) position):
------------------------------------------------------------------
J1 = 1&2
J2 = 2&3
J4 = open
J5 = 1&2
J6 = 1&2
J7 = 1&2
J8 = 1&2
J13 = 1&2
J14 = 3&5
J17 = open
J18 = 1&2
J24 = open

* Not default settings

TWR-MCF51QM128 Jumper Configuration 
(The settings listed indicates the “on” (or installed) position):
------------------------------------------------------------------
J1 = 1&2
J2 = 2&3
J3 = open
J4 = 1&2
J5 = 1&2
J11 = 1&2
J11 = 3&5
J14 = open
J15 = 1&2
J21 = open

* Not default settings

Demo Control
------------------------------------------------------------------
- The touch is indicated by two onboard LED's. Each control indicates 
  status different way:
  > Onboard electrodes indicate each touch by related electrode LED.
  > TWRPI-KEYPAD daughter board indicates each electrode by binary 
    combination of LED's lighting. 
  > The TWRPI-ROTARY indicates by shifting of one LED if movement on rotary 
    control was detected. Center pad is indicated by LED1.
  > The TWRPI-SLIDER indicates by shifting of one LED if movement on analogue 
    slider control was detected. Additional pads are indicated by LED1 and LED2. 
  > The TWRPI-ROTARY2 indicates by shifting of one LED if movement on analogue 
    rotary control was detected. Center pads is indicated by LED1. 
  > The TWRPI-SHIELD1 indicates touch by second LED. Water film presence is
    indicated by first LED.
  > The TWRPI-SHIELD2 indicates touch by second LED. Touch on the shielding
    electrode is singnaled by first LED.
  > The TWRPI-KEYPAD2 daughter board indicates each electrode by binary 
    combination of LED's lighting.    
  > The TWRPI-PROXIMITY indicates proximity detection by all LED blinking. 
    If the proximity is detected then applciation switich to E0-3 detection. 
    After 10 seconds of no activity application goes back to proximity detection.    
- When daughter board is plugged into the TWRPI connector then demo 
  application automatically detects board and TSS system is reinitialized.
- The user can enter MCU to Low Power (LLS) mode by SW1 on TWR-MCF51JF 
  and TWR-MCF51QM board. Wake up is performed by touching of EL1 
  (within control) on onboard electrodes, by EL1 (within control) on 
  TWRPI-KEYPAD, by Central button on TWRPI-ROTARY daughter board and 
  by proximity electrode on TWRPI-PROXIMITY daughter board
  The MCU is woken up by EL5 (within control) on TWRPI-SLIDER, 
  or by center pad on TWRPI-ROTARY2.

Demo Settings
------------------------------------------------------------------

main.h
- #define TSS_USE_DCTRACKER 1/0 - Enable DC tracker for whole demo
- #define TSS_USE_TRIGGER_MODE ALWAYS/SW/AUTO - Enable required 
  TSS trigger mode for whole demo (ALWAYS mode = ~17ms TSI period, 
  SW&AUTO mode = set 20ms TSI period for presentation)
TSS_SystemSetup.h  
- User can select keydetector version by TSS_USE_KEYDETECTOR_VERSION macro 
  in TSS_SystemSetup.h
- User can select if sensitivity autocalibration is used by
  TSS_USE_AUTO_SENS_CALIBRATION macro for whole demo
  
FreeMASTER
------------------------------------------------------------------
Connection options:
- SCI RS232 serial connection via TWR-SER board, speed 115200 bauds

Notes
------------------------------------------------------------------
Since the board TWR-MCF51JF and TWR-MCF51QM provide just 6 TSI channels 
on TWRPI connector there are not used all electrodes available on 
TWRPI-KEYPAD and TWRPI-ROTARY daughter board.


References
------------------------------------------------------------------
TWR-MCF51JF User Manual (Document TWRMCF51JFUM)
TWR-MCF51JF Quick Start Guide (Document MCF51JFQSG)
TWR-MCF51QM User Manual (Document TWRMCF51QMUM)
TWR-MCF51QM Quick Start Guide (Document MCF51QMQSG)
Tower System Serial Module - User Manual (DOcument TWRSERUM)
Touch Sensing Software API Reference Manual (document TSSAPIRM)
Touch Sensing Software Users Guide (document TSSUG)


All mentioned documents are available on www.freescale.com webpage.

----
Copyright (c) 1999-2013, Freescale Semiconductor
ALL RIGHTS RESERVED, www.freescale.com
