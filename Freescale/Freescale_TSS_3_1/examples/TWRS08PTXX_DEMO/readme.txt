TWRS08PTXX_DEMO
==================================================================

This folder contains demo presenting TSS with the HCS08 plaform
MC9S08PT60 on the TWR-S08PT60 board. The demo presents touch 
sensing function by 4 onboard electrodes, daughter board TWRPI-KEYPAD 
(12 electrodes), TWRPI-ROTARY (9 electrodes), TWRPI-SLIDER(5 electrodes),
TWRPI-ROTARY2(5 electrodes),TWRPI-SHIELD1 (6 electrodes), TWRPI-SHIELD2 
(4 electrodes), TWRPI-KEYPAD2 (12 keys done by 6 multiplexed electrodes) 
and TWRPI-PROXIMITY (4 keys and 1 proximity electrode). Five kinds of 
controls (KEYPAD, GROUP KEYPAD, ROTARY, ANALOG SLIDER, ANALOG ROTARY, 
MATRIX) are presented. The sensing is performed by TSI module inside MCU.

Features & Information:
------------------------------------------------------------------
- Freescale TWR-S08PT60 tower board, MC9S08PT60
- CodeWarrior for MCU v 10.1 or higher supported
- TSI module performing sensing function
- FreeMASTER Visualization GUI supported via serial connection 
  provided by TWR-SER board connected with elevator boards 
- Onboard OpenSource BDM (OSBDM) programmer available
- Standard BDM connector available for external BDM programmer
- TSS Triggering function
- BUSclk 16MHz used

TWR-S08PT60 Jumper Configuration 
(The settings listed indicates the “on” (or installed) position):
------------------------------------------------------------------
J1  = 2&3 *
J2  = 2&3 *
J3  = 1&2
J4  = open
J5  = 1&2 *
      4&5 *
      7&8 *
      10&11 *
      other open * 
J6  = 3&5
J7  = 1&2
J9  = 2&3
J12 = open
J15 = open
J16 = 1&2 (1&2 - onboard OSBDM use / open - external BDM use)
J17 = 2&3
J18 = 2&3
J20 = 1&2 *
J21 = 1&2 *
J22 = 1&2 *
J25 = 2&3
J26 = 2&3
J27 = 1&2 * (1&2 - MCU 5V / 2&3 - MCU 3,3V)
J28 = 1&2 (1&2 - onboard OSBDM use / open - external BDM use)
J29 = 1&2

* Not default settings

Demo Control
------------------------------------------------------------------
- The touch is indicated by four onboard LED's.
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
    
Demo Settings
------------------------------------------------------------------

main.h
- #define APP_TSS_USE_DCTRACKER 1/0 - Enable DC tracker for whole demo
- #define APP_TSS_USE_TRIGGER_MODE ALWAYS/SW/AUTO - Enable required 
  TSS trigger mode for whole demo (ALWAYS mode = ~5ms TSI period, 
  SW&AUTO mode = set 20ms TSI period for presentation)
common.h
- #define MASKSET PT60_2N40C/PT60_OTHER - Specification of MCU maskset
TSS_SystemSetup.h  
- User can select keydetector version by TSS_USE_KEYDETECTOR_VERSION macro 
  in TSS_SystemSetup.h
- User can select if sensitivity autocalibration is used by
  TSS_USE_AUTO_SENS_CALIBRATION macro for whole demo
  
FreeMASTER
------------------------------------------------------------------
Connection options:
- Onboard OSBDM - Default
- SCI RS232 serial connection (SCI1 on PT60) via UART on-board 
  UART HDR connector, baudrate 57600
- External P&E Multilink BDM HCS08 connection

Notes
------------------------------------------------------------------
- Latest version of OSBDM firmware may not be supported by FreeMASTER,
  so please use rather P&E Multilink BDM or on-board UART connection.
- If the user wants to use serial connection with FreeMASTER,
  then it is recommended to use on-board UART HDR with external 
  binding cable (attached in the box). In this case the J5 header needs 
  to have closed pin 2&3 and 5&6. The FreeMASTER needs to be reconfigured 
  for serial connection with 57600 baudrate. For this option is better 
  to set TSS_USE_SIMPLE_LOW_LEVEL to 1 in TSS_SystemSetup.h.
  The J5 configuration change causes unconnection of two TWRPI electrodes. 
  If TWRPI-KEYPAD is used the EL7 and EL8 (within control) are not 
  functional. If TWRPI-ROTARY is used the EL3 and EL5 (within control) 
  are not functional.
- If the TWR-S08PT60 board is connected with TWR_SER via elevator boards
  there is a conflict of TSI electrodes with TWR_SER board. If TWRPI-KEYPAD 
  is used the EL7 (within control) is not functional. If TWRPI-ROTARY 
  is used the EL5 (within control) is not functional.
- The PT60 with maskset 2N40C contains known bug in SCI hardware module.
  See errata e3869 in 'Mask Set Errata for Mask 2N40C' document.
  This errata causes that FreeMASTER SCI RS232 serial connection 
  (SCI1 on PT60) is not functional on PT60 mcu. 
  If the user wants to use example serial driver for any purpose 
  on 2N40C maskset MCU, then he needs to set macro MASKSET to PT60_2N40C 
  in common.h file.

References
------------------------------------------------------------------
MC9S08PT60 Reference Manual (document MC9S08PT60RM)
TWR-S08PT60 User Manual (document TWRS08PT60UM)
TWR-S08PT60 Quick Start Guide (document TWRS08PT60QSG)
Tower System Serial Module - User Manual (DOcument TWRSERUM)
Touch Sensing Software API Reference Manual (document TSSAPIRM)
Touch Sensing Software Users Guide (document TSSUG)


All mentioned documents are available on www.freescale.com webpage.

----
Copyright (c) 1999-2013, Freescale Semiconductor
ALL RIGHTS RESERVED, www.freescale.com
