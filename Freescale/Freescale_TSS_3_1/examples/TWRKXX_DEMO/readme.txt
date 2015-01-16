TWRKXX_DEMO
==================================================================

This folder contains demo presenting TSS with the ARM®Cortex™-M4
Kinetis platform and ARM®Cortex™-M0+ Kinetis L platform.

Following MCU's are supported from ARM®Cortex™-M4 family in this demo:
MK20D5, MK20DZ10, MK40X256VMD100, MK53DZ10, MK60N512VMD100,
MK60DN512VMD10 and MK70F12 on the TWRK20D50M, TWRK20D72M,
TWRK40X256, TWRK53N512, TWRK60N512, TWRK60D100M and TWRK70F120M boards.

Following MCU's are supported from ARM®Cortex™-M0+ family in this demo:
MKL05Z48M, MKL25Z48M and MKL46Z48M on the TWRKL05Z48M, TWRKL25Z48M,
TWRKL46Z48M boards.

The demo presents touch sensing function by 2 or 4 onboard electrodes and
several daughter TWRPI boards. Supported TWRPI daughter boards are
TWRPI-KEYPAD (12 electrodes), TWRPI-ROTARY (9 electrodes),
TWRPI-SLIDER (5 electrodes), TWRPI-ROTARY2 (4 electrodes),
TWRPI-TOUCHPAD (12 electrodes), TWRPI-SHIELD1 (6 electrodes),
TWRPI-SHIELD2 (4 electrodes), TWRPI-KEYPAD2 (12 keys done by 6 multiplexed
electrodes) and TWRPI-PROXIMITY (4 keys and 1 proximity electrode).

Five types of controls (KEYPAD, ROTARY, ANALOGUE SLIDER, ANALOGUE ROTARY,
MATRIX) are presented. The sensing is performed by TSI module inside MCU.

Features & Information:
------------------------------------------------------------------
- TWRK20D50M tower board, MK20D5
- TWRK20D72M tower board, MK20DZ10
- TWRK40X256 tower board, MK40X256VMD100
- TWRK53N512 tower board, MK53DZ10
- TWRK60N512 tower board, MK60N512VMD100
- TWRK60D100M tower board, MK60DN512VMD10
- TWRK70F120M tower board, MK70F12
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
- Wakeup from Low Power MCU mode function presented
- Triggering function
- Proximity function

TWR-K20D50M Jumper Configuration (rev AX1)
(The settings listed indicates the “on” (or installed) position):
------------------------------------------------------------------
J1 = 1&2
J4 = 1&2
J6 = 1&2
J7 = open
J9 = open
J11 = 1&2
J13 = 1&2
J15 = 1&2
J16 = 1&2
J17 = open
J18 = open
J19 = 1&2
J20 = 1&2
J23 = 1&2
J25 = 1&2
J26 = 1&2
J29 = 3&5
J30 = 1&2
J32 = 1&2
J33 = 1&2
J34 = open
J35 = 1&2
J36 = 1&2

TWR-K20D72M Jumper Configuration
(The settings listed indicates the “on” (or installed) position):
------------------------------------------------------------------
J1 = 1&2
J2 = 1&2
J3 = 1&2
J4 = open
J6 = 1&2
J10 = open
J13 = 1&2
J15 = open
J16 = 1&2, 5&6
J17 = 3&5
J18 = open
J20 = 1&2
J21 = open
J23 = open
J24 = open
J25 = open
J26 = open
J27 = open

TWR-K40X256 Jumper Configuration
(The settings listed indicates the “on” (or installed) position):
------------------------------------------------------------------
J3 = 1&2
J5 = 2&3
J6 = open
J11 = 1&2
J12 = 1&2
J13 = open
J15 = open

TWR-K53N512 Jumper Configuration
(The settings listed indicates the “on” (or installed) position):
------------------------------------------------------------------
J1 = 1&2
J3 = 2&3
J4 = 1&2
J5 = open
J6 = 1&2
J7 = open*
J11 = 1&2
J12 = 1&2*
J14 = open
J15 = 1&2
J16 = 1&2
J17 = 1&2
J18 = 1&2
J20 = open

* Not default settings

TWR-K60N512 Jumper Configuration
(The settings listed indicates the “on” (or installed) position):
------------------------------------------------------------------
J1 = 1&2
J2 = open
J6 = 1&2
J8 = 1&2
J9 = 1&2
J10 = open
J12 = open

TWR-K60D100M Jumper Configuration
(The settings listed indicates the “on” (or installed) position):
------------------------------------------------------------------
J1 = 1&2
J2 = open
J4 = 1&2
J5 = 1&2
J7 = open
J10 = 1&2
J14 = 1&2
J12 = 1&2
J15 = open
J19 = open

TWR-K70F120M Jumper Configuration
(The settings listed indicates the “on” (or installed) position):
------------------------------------------------------------------
J1 = 1&2
J2 = open
J8 = 1&2
J10 = open
J12 = open
J16 = open
J17 = 1&2
J19 = 1&2
J20 = 1&2

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
- The touch is indicated by two or four onboard LED's. If board
  has only two electrodes, macros LED3 and LED4 are not functional.
  Each control indicates status different way:
  > Onboard electrodes indicate each touch by related electrode LED.
  > TWRPI-KEYPAD daughter board indicates each electrode by binary
    combination of LED's lighting.
  > The TWRPI-ROTARY indicates by one LED shifting if movement on rotary
    control was detected. Center pad is indicated by LED1.
  > The TWRPI-SLIDER indicates by shifting of one LED if movement on analogue
    slider control was detected. Additional pads are indicated by LED1 and LED2.
  > The TWRPI-ROTARY2 indicates by shifting of one LED if movement on analogue
    rotary control was detected. Center pads is indicated by LED1.
  > The TWRPI-TOUCHPAD indicates position on axis Y.
  > The TWRPI-SHIELD1 indicates each button touch by related LED. In case of
    water film presence, last on board LED is turned on.
  > The TWRPI-SHIELD2 indicates each button by related LED. If the shielding
    electrode is touched, last on board LED is turned on.
  > The TWRPI-KEYPAD2 daughter board indicates each electrode by binary
    combination of LED's lighting.
  > The TWRPI-PROXIMITY indicates proximity detection by all LED blinking.
    If the proximity is detected then application switches to E0-3 detection.
    After 10 seconds of no activity application goes back to proximity detection.
- When daughter board is plugged into the TWRPI connector then demo
  application automatically detects board and TSS system is reinitialized.
- The user can enter MCU into Low Power mode by SW2 on TWRK70F120M board,
  by SW2 on TWRK20D50M, by SW4 on TWRK40X256, by SW2 on TWRK53N512,
  and by SW2 on TWRK60N512 board. For TWRK40X256, TWRK53N512 and TWRK60N512
  board is selected LLS Low Power mode. For TWRK70F120M and TWRK20D50M is
  used VLLS1 Low Power mode. Wake up on all TWRKXX boards is
  performed by touching of EL1 (within control) on onboard electrodes,
  by EL6 (within control) on TWRPI-KEYPAD, by central button on
  TWRPI-ROTARY daughter board and by proximity electrode on
  TWRPI-PROXIMITY daughter board. The MCU is also woken up by EL5 (within control )
  on TWRPI-SLIDER, or by center pad on TWRPI-ROTARY2.

Demo Settings
------------------------------------------------------------------

main.h
- #define APP_TSS_USE_DCTRACKER 1/0 - Enable DC tracker for whole demo
- #define APP_TSS_USE_TRIGGER_MODE ALWAYS/SW/AUTO - Enable required
  TSS trigger mode for whole demo (ALWAYS mode = ~1ms TSI period,
  SW mode = set 20ms TSI period, AUTO mode = set 10ms TSI period
  for presentation)
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
- In the case of MCU with maskset 2N22D and newer please add line
  '#define TSS_TSI_VERSION 2' into the TSS_SystemSetup.h. Otherwise TSI
  measurement will not be functional.
- If TWR_SER board is plugged into Tower module, it may cause conflicts with
  TSI channels. Those channels are automatically disabled and not functional.
- The K40/K60 MCU's with a mask set version 0M33Z contains known bug in LLWU
  hardware module related with wake-up from LLS mode. See errata e2613 in
  'Mask Set Errata for Mask 0M33Z' document. These MCU's are not able to
  present wake-up from LLS mode by TSI module correctly, so it is strongly
  recommended to use the latest silicon version.
- TWRK20D72M board needs to be directly powered via onboard USB connector.
- if TWR-KL05Z48M is used with Freemaster GUI, Tower Elevator Module needs to be powered
  and the board as well.

List of not functional channels
------------------------------------------------------------------
- There is a conflict with TSI channel 0 on TWRK60N512 and TWRK70F120M board.
- There is a conflict with TSI channel 7 and 8 on TWRK20D50M board and
  TWRK20D72M board. These electrodes are disabled.
- There is a conflict with TSI channel 0 on TWRK53N512. This electrode is
  disabled.
- There is a conflict with TSI channel 2 and 3 on TWRKL05Z48M with UART0 pins. 
  These electrodes are automatically disabled if TSS_USE_FREEMASTER_GUI macro 
  is enabled. In that case, some daugher boards will no provide all key inputs, 
  so some presented functions may not work correctly, like proximity function 
  on TWRPI_PROXIMITY etc. If macro TSS_USE_FREEMASTER_GUI is disabled and 
  TWR-SER board is unplugged, then all inputs are available and functions 
  are presented correctly.

References
------------------------------------------------------------------
TWR-K20D50 User's Manual (Document TWR-K20D50-UM)
TWR-K20D72 User's Manual (Document TWR-K20D72-UM)
TWR-K40X256 User's Manual (Document TWR-K40X256-UM)
TWR-K53N512 User's Manual (Document TWR-K52N512-UM)
TWR-K60N512 User's Manual (Document TWR-K60N512-UM)
TWR-K60D100M User's Manual (Document TWRK60D100MUM)
TWR-K70F120M User's Manual (Document TWRK70F120MUM)
TWR-KL05Z48M User's Manual (Document TWR-KL05Z-UM)
TWR-KL25Z48M User's Manual (Document TWR-KL25Z-UM)
TWR-KL46Z48M User's Manual (Document TWR-KL46Z-UM)
Tower System Serial Module - User Manual (DOcument TWRSERUM)
Touch Sensing Software API Reference Manual (document TSSAPIRM)
Touch Sensing Software Users Guide (document TSSUG)


All mentioned documents are available on www.freescale.com webpage.

----
Copyright (c) 1999-2013, Freescale Semiconductor
ALL RIGHTS RESERVED, www.freescale.com
