TWRKXX_DEMO_EMC
==================================================================

This folder contains demo presenting TSS with robust configuration for 
EMC testing (mainly for EN61000-4-6 standard). The demo presents 
ARM®Cortex™-M0 Kinetis L platform with TSI noise mode enabled and 
AFID touch detection method.
Following MCU are supported in this demo: TWRKL05Z48M, TWRKL25Z48M and
TWRKL46Z48M boards. The demo presents touch sensing 
function by 2 onboard electrodes and several TWRPI daughter boards.

Supported TWRPI daughter boards are TWRPI-KEYPAD (12 electrodes),
TWRPI-ROTARY (9 electrodes), TWRPI-SLIDER(5 electrodes) and  
TWRPI-KEYPAD2 (12 keys done by 6 multplexed electrodes).

Three types of controls (KEYPAD, ROTARY and group KEYPAD 
are presented. The capacitive sensing is performed by TSI module 
inside MCU.

Features & Information:
------------------------------------------------------------------
- TWRKL05Z48M tower board, MKL05Z32
- TWRKL25Z48M tower board, MKL25Z128
- TWRKL46Z48M tower board, MKL46Z256
- CodeWarrior for MCU v 10.3 or higher supported
- IAR Embedded Workbench for ARM Version 6.50.2 or higher supported
- KEIL uVision 4.71.2 or higher supported
- TSI module performing sensing function
- FreeMASTER Visualization GUI supported via serial connection provided by 
  TWR-SER board connected with elevator boards
- Onboard OpenSource JTAG (OSJTAG) programmer available
- JTAG programing port available for J-Link Connection

TWR-KL05Z48 Jumper Configuration 
(The settings listed indicates the “on” (or installed) position):
------------------------------------------------------------------

J4 = 1&2*
J5 = 3&2*
J6 = 1&2*
J7 = open
J8 = 3&2
J9 = 1&2
J10 = open*
J11 = open*
J14 = 1&2
J16 = 1&2
J17 = 1&2
J18 = 1&2
J19 = 1&2
J20 = 1&2
J21 = 1&2
J22 = 1&2
J24 = open*
J25 = open*
J26 = open
J27 = open
J28 = 1&2

* Not default settings

TWR-KL25Z48 Jumper Configuration 
(The settings listed indicates the “on” (or installed) position):
------------------------------------------------------------------

J1 = 1&2
J3 = 1&3
J6 = open
J7 = 1&2
J8 = 1&2
J9 = 1&2
J14 = open
J15 = open
J17 = open
J18 = 1&2*
J19 = 1&2
J20 = open
J21 = open
J22 = 1&2
J23 = 1&2
J24 = 1&2*
J25 = 1&2
J26 = 1&2*

* Not default settings

TWR-KL46Z48 Jumper Configuration 
(The settings listed indicates the “on” (or installed) position):
------------------------------------------------------------------

J3 = 1&2
J4 = 1&2
J7 = 1&3
J10 = 1&2*
J11 = 1&2*
J13 = 1&2
J15 = 1&2
J16 = 1&2*
J17 = open
J18 = 1&2
J19 = 1&2
J20 = 1&2
J21 = 1&2
J22 = open*
J23 = 1&2*
J24 = open*
J25 = open
J26 = open*
J27 = 1&2
J28 = 1&2

* Not default settings

Demo Control
------------------------------------------------------------------
- The touch is indicated by two onboard LED's. 
  Each control indicates status different way:
  > Onboard electrodes indicate each touch by related electrode LED.
  > TWRPI-KEYPAD daughter board indicates each electrode by binary 
    combination of LED's lighting. 
  > The TWRPI-ROTARY indicates by one LED shifting if movement on rotary 
    control was detected. Center pad is indicated by LED1.
  > The TWRPI-SLIDER indicates by shifting of one LED if movement on 
    slider control was detected. Additional pads are indicated by LED1 and LED2. 
  > The TWRPI-KEYPAD2 daughter board indicates each electrode by binary 
    combination of LED's lighting.
- When daughter board is plugged into the TWRPI connector then demo 
  application automatically detects board and TSS system is reinitialized.

Demo Settings
------------------------------------------------------------------

TSS_SystemSetup.h
- User can select keydetector version by TSS_USE_KEYDETECTOR_VERSION 
  macro for whole demo
- User can select if sensitivity autocalibration is used by
  TSS_USE_AUTO_SENS_CALIBRATION macro for whole demo

FreeMASTER
------------------------------------------------------------------
Connection options:
- SCI RS232 serial connection via TWR-SER board, speed 115200 bauds

Notes
------------------------------------------------------------------
  
The demo does not present analog controls, low power and shielding functions,
because TSI noise mode is incompatible with these functions.  
The AFID Keydetector algorithms are based on mathematical filters 
rather than on baseline tracking and delta signal evaluation. 
This makes the AFID incompatible with shielding noise avoidance method 
and with the under-water operation. All other TSS features like analog controls,
low power wake-up, or proximity are fully supported by AFID. 

List of not functional channels
------------------------------------------------------------------
There is a conflict with TSI channel 2 and 3 on TWRKL05Z48M with UART0 pins. 
These electrodes are automatically disabled if TSS_USE_FREEMASTER_GUI macro 
is enabled. In that case, some daugher boards will no provide all key inputs, 
so some presented functions may not work correctly, like proximity function 
on TWRPI_PROXIMITY etc. If macro TSS_USE_FREEMASTER_GUI is disabled and 
TWR-SER board is unplugged, then all inputs are available and functions 
are presented correctly.
  
References
------------------------------------------------------------------
TWR-KL05Z48M User's Manual (Document TWR-KL05Z-UM)
TWR-KL25Z48M User's Manual (Document TWR-KL25Z-UM)
TWR-KL46Z48M User's Manual (Document TWR-KL46Z-UM)
Tower System Serial Module - User Manual (Document TWRSERUM)
Touch Sensing Software API Reference Manual (document TSSAPIRM)
Touch Sensing Software Users Guide (document TSSUG)


All mentioned documents are available on www.freescale.com webpage.

----
Copyright (c) 1999-2013, Freescale Semiconductor
ALL RIGHTS RESERVED, www.freescale.com
