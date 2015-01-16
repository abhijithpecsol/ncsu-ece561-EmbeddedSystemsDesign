TSSEVB_SINGLE demo
==================================================================

This folder contains demo presenting TSS with the HCS08 platform
MC9S08LG32 MCU on TSSEVB demo board. The demo is simplified version of 
TSSEVB_DEMO which presents just 1 electrode (E1 - the biggest one) and 
1 keypad  control.

Features & Information:
------------------------------------------------------------------
- TSSEVB demo board, MC9S08LG32 
- CodeWarrior Development Studio for Microcontrollers version 6.3 or higher
- CodeWarrior for MCU v 10.1 or higher
- The application was created using built-in CodeWarrior project creation
  wizard (no Processor Expert support)
- Electrode Graphing Tool supported 
- FreeMASTER Visualization GUI supported
- BUSclk 10MHz used
- Onboard LCD with detailed status information
- Onboard HCS08 OpenSource BDM (OSBDM) programmer available
- BDM programing port available

FreeMASTER
------------------------------------------------------------------
Connection options:
- HCS08 OpenSource BDM (OSBDM) <- Default
- P&E Multilink BDM HCS08 connection

Additional Instructions
------------------------------------------------------------------
- If an older version of TSSEVB board is used you will probably need to 
  update the MC9S08JM60 Comm. MCU firmware in order the communication with 
  EGT tool works properly. To re-program the MC9S08JM60 Comm. MCU, 
  follow instructions in TSSEVB User Guide (TSSEVBUG) Section 3.1.1. 
- Update of OSBDM firmware is highly recommended, because it can cause 
  malfunction of connection with FreeMASTER and possibility to 
  program the software with last development environments (especially 
  CodeWarrior for MCU 10.1).The last firmware is available on website 
  www.freescale.com. The update is possible by OSBDM header placed on 
  top side of TSSEVB board.
  The direct update of OSBDM firmware is functional up to the OSBDM firmware 
  version 20.1. For higher versions the user needs to manually connect PIN2 
  on LG32 MCU and V_MAIN (VCC, or C18) by 10kOhms resistor.

References
------------------------------------------------------------------
Touch Sensing Software Evaluation Board Users Guide (document TSSEVBUG)
Touch Sensing Software Electrode Graphing Tool Users Guide (document TSSEGTUG)
Touch Sensing Software Evaluation Board Quick Start Guide (document TSSEVBQSG)
Touch Sensing Software API Reference Manual (document TSSAPIRM)
Touch Sensing Software Users Guide (document TSSUG)
Open Source BDM-JM60 Users Guide (document OSBDM-JM60_USERS_GUIDE)


All mentioned documents are available on www.freescale.com webpage.

----
Copyright (c) 1999-2013, Freescale Semiconductor
ALL RIGHTS RESERVED, www.freescale.com
