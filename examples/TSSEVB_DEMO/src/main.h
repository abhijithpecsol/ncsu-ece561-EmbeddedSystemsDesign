/***************************************************************************************
*
* Demo project for Freescale TSS and TSSEVB Demo Board
*
* This program uses the TSS. The project includes three Demos to test the TSSEVB funtionality 
* along with the TSS Software. For more information regarding the Demo Software please refer
* to the "Demo Application Guide" document.   
*
* Before to run this example, verify that all jumpers are in their default
* position. See the "System Overview" chapter in the TSSEVB user's manual.
*                                                                                       
* --------------------------------------------------------------------------------------
*                                                                                                         
* @file   main.h
*
* @brief  Header file that contains all the macros and definitions of the project
*
* @version 1.1.0 0 Updated I2C Driver Macros.
*          1.0.0.0 Initial Release 
* 
* @date January - 19 - 2010                                                             
*                                                                                          
* --------------------------------------------------------------------------------------
*
* RTAC Americas
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2010 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
*****************************************************************************************/

#ifndef _MAIN_H_
    #define _MAIN_H_
    
    #include <hidef.h>
    #include "TSS_API.h"
    #include "derivative.h"
    
    /* MACROS */
    
    #define MAX_DEMO_NUM            4
    #define MIN_DEMO_NUM            1
    #define SHADOW_DEMO             1
    #define ALARM_CLOCK_DEMO        2
    #define MICROWAVE_DEMO          3
    #define WASHING_MACHINE_DEMO    4
    #define OVERLAY_CONFIG          5
    #define POPCORN                 0x05
    #define REHEAT                  0x06
    #define SOUP                    0x09
    #define COFFE                   0x0A
    #define START_PAUSE             0x02
    #define LIGHT_ON_OFF            0x02
    #define ON_OFF                  0x03
    #define OPEN                    0x00
    #define LOCK                    0x00
    #define EXTRA_RINSE             0x01
    #define TIMER                   0x02
    #define ENERGY_SAVING           0x03
    #define KEY_1                   0x05
    #define KEY_2                   0x09
    #define KEY_3                   0x0A
    #define KEY_4                   0x14
    #define KEY_5                   0x18
    #define KEY_6                   0x0C
    #define KEY_7                   0x12
    #define KEY_8                   0x30
    #define KEY_9                   0x24
    #define KEY_10                  0x21
    #define INCREMENT_30            0x03
    #define RESET                   0x03
    #define CANCEL_CLEAR            0x22
    #define MCVE_DELAY              0x20
    #define TURN_OFF_ALL            0x00
    #define TURN_ON_ALL             0x0b
    #define TURN_LIGHTS_ON          22
    #define NORMAL_WASH             0x00
    #define EXTRA_CARE_WASH         0x04
    #define CASUAL_WASH             0x08
    #define HAND_WASH               0x0C
    
    /* FUNCTIONS */

    void RotaryToAscii(uint8_t Direction, uint8_t Displacement);
    void RotaryToAsciiHours(uint8_t Direction, uint8_t Displacement);
    void vfsLCD_Write_NUMS(struct time nums);
    void vfnUpdateClock(void);
    void gfMuxElectrodes(void);
    void gfDimentionElectrodes(void);
    void gfKeyBoardElectrodes(void);
    void vfnMCVEdecrementTime(void);
    void vfnWriteDemoSelection(void);
    void vfnDigitDisplacement(void);
    void vfnSensitivityConfig(void);


/************************ BUZZ_H ************************************************/

    /***************************************************************************************
    * Defines
    ***************************************************************************************/
    #define PWM_1KHZ        0x27
    #define DUTY_CYCLE      0x14
    #define BUZZ_DURATION   0x30

    /***************************************************************************************
    * Functions
    ***************************************************************************************/
    void vfnBUZZ(void);
    void BUZZInit(void);


/************************ SCI_H ************************************************/


    /***************************************************************************************
    * Defines
    ***************************************************************************************/
       
    #define BR_TSS              65      /* BAUD RATE = 9600 (Baud rate mismatch = 0.160 %)  */
    #define TX_BUF_SIZE         3  	    /* TX buffer size = 2 characters                    */
    #define RX_BUF_SIZE         3  	    /* RX buffer size = 2 characters                    */
    #define ERROR               0x07

    /***************************************************************************************
    * Functions
    ***************************************************************************************/
    
    void SCIInit(void);
    void fnWriteDataTxBuffer(uint8_t u8Data);
    uint8_t fnReadDataRxBuffer(void);



/************************ LED_H ************************************************/


    #define LED_LED1_P    F
    #define LED_LED1_B    7

    #define LED_LED2_P    F
    #define LED_LED2_B    6

    #define LED_LED3_P    G
    #define LED_LED3_B    2

    #define LED_LED4_P    G
    #define LED_LED4_B    6


    #define LED_GPIO_DIR_IN     0
    #define LED_GPIO_DIR_OUT    1
    #define LED_LED_ON          LED_GPIO_CLEAR
    #define LED_LED_OFF         LED_GPIO_SET


    #define LED_GPIO_PIN_DD(port, pin)     PT##port##DD_PT##port##DD##pin  /* Defines the port-pin register name */
    #define LED_GPIO_PIN(port, pin)        PT##port##D_PT##port##D##pin    /* Defines the port-pin direction register name */

    #define LED_GPIO_OUT(port, pin)        (LED_GPIO_PIN_DD(port, pin) = LED_GPIO_DIR_OUT) /* Sets port-pin as output */
    #define LED_GPIO_SET(port, pin)        (LED_GPIO_PIN(port, pin) = 1U)       /* Writes "1" to the port-pin */
    #define LED_GPIO_CLEAR(port, pin)      (LED_GPIO_PIN(port, pin) = 0U)       /* Writes "0" to the port-pin */
    #define LED_GPIO_TOGGLE(port, pin)     (LED_GPIO_PIN(port, pin) = ~LED_GPIO_PIN(port, pin))/* Writes "0" to the port-pin */



    #define LED_INIT(X)                     LED_LED_OFF(LED_##X##_P,LED_##X##_B); LED_GPIO_OUT(LED_##X##_P,LED_##X##_B)
    #define LED_ON(X)                       LED_LED_ON(LED_##X##_P,LED_##X##_B)
    #define LED_OFF(X)                      LED_LED_OFF(LED_##X##_P,LED_##X##_B)
    #define LED_TOGGLE(X)                   LED_GPIO_TOGGLE(LED_##X##_P,LED_##X##_B)
  

/************************ MAIN_H ************************************************/

    struct time{
      int8_t AMPM;
      int8_t HourMSB;
      int8_t HourLSB;
      int8_t MinuteMSB;
      int8_t MinuteLSB;
    };

    typedef struct 
    {                                                    
        uint8_t fOpen           :1;
        uint8_t fLock           :1;
        uint8_t fOnOff          :1;
        uint8_t fCountActive    :1;
        uint8_t fTimer          :1;
        uint8_t fDisplacement   :1;
        uint8_t fClockActive    :1;
        uint8_t fEnergySaving   :1;
    } DEMOFLAGS;

    typedef struct 
    {                                                    
        uint8_t fLights         :1;
        uint8_t fBuzzLong       :1;
        uint8_t                 :1;
        uint8_t                 :1;
        uint8_t                 :1;
        uint8_t                 :1;
        uint8_t                 :1;
        uint8_t                 :1;
    } DEMOFLAGS1;

/************************** LCD_HAL_H **************************************************/

/*^^^^^^^^^^^^^^^^      LCD Configuration                        ^^^^^^^^^^^^^^^^^^^^^^^^*/
//#define MC9RS08LE4
//#define MC9RS08LA8
//#define MC9S08LL16
#define MC9S08LG32

#define _LCDBACKPLANES          (8)           /* # of backplanes  */
#define _LCDCLKSOURCE           (1)           /* 0 -- External clock       1 --  Alternate clock  (Internal) */


#define _LCDCLKPSL              (3)           /* Clock divider to generate the LCD Waveforms */


#define _LCDCPSEL               (0)           /* 0 -- Resistor network   1 -- Charge pump */
#define _LCDVSUPPLY             (1)           /* 0  --  VLL2 from VDD    1 --   VLL3 from VDD  2 -- VLL1 from VLCD  3 -- VLL3 external */
#define _LCDLOADADJUST          (0)           /* Charge pump 		0 -- 8000 pf 1 -- 6000 pf  2 -- 4000 pf  3 -- 2000 pf */
                                              /* Resistor Network	0 -- 2000 pf 1 -- 2000 pf  2 -- 8000 pf   3 -- 8000 pf */

#define _LCDFRAMEINTERRUPT      (0)           /* 0 Disable Interrupt, 1 Enable a LCD interrupt that coincides with the LCD frame frequency */

#define _LCDBLINKRATE           (0)           /* Blink frequecy preescaler  any number between 0-7 */

#define _CHARNUM                (4)           /* Number of characters on the display */

#define _LCDTYPE                (2)           /* Number of LCDWF  nedded  to drive one character */

/* ^^^^^^^^^^^^^^^^      LCD Hardware Connection 			 ^^^^^^^^^^^^^^^^^^^^^^^^ */

#define EnableLCDpins(RegNum,Mask)			    LCDPEN##RegNum = Mask
#define EnableBackplanes(RegNum, Mask)	    LCDBPEN##RegNum = Mask   
#define SetBackplane(ComNum, LCDn)		      LCDWF##LCDn  = (1 <<  (##ComNum##))
#define CharacterPlace(LCDn)        			  LCDWF##LCDn
#define AllSegmentsON(LCDn,ComMask)		      LCDWF##LCDn = ComMask
#define SymbolON(LCDn, BitNumber)			      LCDWF##LCDn = (RamBuffer[LCDn] |= (1<< BitNumber))
#define SymbolOFF(LCDn, BitNumber)			    LCDWF##LCDn = (RamBuffer[LCDn] &= ~(1<< BitNumber))
#define MapSegment(BitNumber)  				      (1<<(##BitNumber##)) 
#define LCDWFRef          						      LCDWF0
#define LCD_AlphanumericPlace(Place)			  bLCD_CharPosition=_LCDTYPE*##(Place-1)
/*
EnableLCDpins
Habilitate the mcu pin with the characteristics of a LCD pin 
param: RegNum -- Number of register to write     Mask --- Mask that habilitate the LCDx pins
*/
#define   PEN0         EnableLCDpins (0,255)
#define   PEN1         EnableLCDpins (1,255)
#define   PEN2         EnableLCDpins (2,63)
#define   PEN3         EnableLCDpins (3,0)
#define   PEN4         EnableLCDpins (4,0)
#define   PEN5         EnableLCDpins (5,0)

/* 
EnableBackplanes
Enable the LCD pin like a Backplane functionality  
param: RegNum -- Number of register to write     LCDpin --- LCDn pin number that the Backplane it's connected 
*/
#define   BPEN0        EnableBackplanes (0, 255)
#define   BPEN1        EnableBackplanes (1, 0)
#define   BPEN2        EnableBackplanes (2, 0)
#define   BPEN3        EnableBackplanes (3, 0)
#define   BPEN4        EnableBackplanes (4, 0)
#define   BPEN5        EnableBackplanes (5, 0)
/*
SetBackplane
 Give the COM number  to the previous enable Backplane     
param: ComNum -- COM number     LCDpin --- LCDn number of pin that its connected  
*/
#define   SetCom1      SetBackplane (0, 0)
#define   SetCom2      SetBackplane (1, 1)
#define   SetCom3      SetBackplane (2, 2)
#define   SetCom4      SetBackplane (3, 3)
#define   SetCom5      SetBackplane (4, 4)
#define   SetCom6      SetBackplane (5, 5)
#define   SetCom7      SetBackplane (6, 6)
#define   SetCom8      SetBackplane (7, 7)

/*
Character place
This macro defines the LDCWavefroms directions needed for write the alphanumerics        
param:  LCDpin --- LCDn number of pin that its connected */

#define   Char1a           (8)
#define   Char1b           (9)
#define   Char2a           (10)
#define   Char2b           (11)
#define   Char3a           (12)
#define   Char3b           (13)
#define   Char4a           (14)
#define   Char4b           (15)


/* 
AllSegmentsON
Turns on all the segments of the custom glass   
param: LCDn -- LCDWF that needs to be turn on	ComMask -- Bits that are turn on example: 8 COM = 255   2 COM = 3
*/
#define Segment1_ON               AllSegmentsON ( 8,255 )
#define Segment2_ON               AllSegmentsON ( 9,255 )
#define Segment3_ON               AllSegmentsON ( 10,255 )
#define Segment4_ON               AllSegmentsON ( 11,255 )
#define Segment5_ON               AllSegmentsON ( 12,255 )
#define Segment6_ON               AllSegmentsON ( 13,255 )
#define Segment7_ON               AllSegmentsON ( 14,255 )
#define Segment8_ON               AllSegmentsON ( 15,255 )
#define Segment9_ON               AllSegmentsON ( 16,255 )
#define Segment10_ON              AllSegmentsON ( 17,255 )
#define Segment11_ON              AllSegmentsON ( 18,255 )
#define Segment12_ON              AllSegmentsON ( 19,255 )
#define Segment13_ON              AllSegmentsON ( 20,255 )
#define Segment14_ON              AllSegmentsON ( 21,255 )

/* 
AllSegmentsOFF
Turns on all the segments of the custom glass   
param: LCDn -- LCDWF that needs to be turn on	ComMask -- Bits that are turn on example: 8 COM = 255   2 COM = 3*/
#define Segment1_OFF                AllSegmentsON ( 8, 0 )
#define Segment2_OFF                AllSegmentsON ( 9, 0 )
#define Segment3_OFF                AllSegmentsON ( 10, 0 )
#define Segment4_OFF                AllSegmentsON ( 11, 0 )
#define Segment5_OFF                AllSegmentsON ( 12, 0 )
#define Segment6_OFF                AllSegmentsON ( 13, 0 )
#define Segment7_OFF                AllSegmentsON ( 14, 0 )
#define Segment8_OFF                AllSegmentsON ( 15, 0 )
#define Segment9_OFF                AllSegmentsON ( 16, 0 )
#define Segment10_OFF               AllSegmentsON ( 17, 0 )
#define Segment11_OFF               AllSegmentsON ( 18, 0 )
#define Segment12_OFF               AllSegmentsON ( 19, 0 )
#define Segment13_OFF               AllSegmentsON ( 20, 0 )
#define Segment14_OFF               AllSegmentsON ( 21, 0 )


/* 
SymbolON
Turns on the  segment that its dedicated to write one Symbol     
param:  LCDpin --- LCDn number of pin that its connected   BitNumber -- COM Number that turns on that Symbol 
*/
#define _LCD_R5_ON         SymbolON(16,0)
#define _LCD_R4_ON         SymbolON(16,1)
#define _LCD_R3_ON         SymbolON(16,2)
#define _LCD_R2_ON         SymbolON(16,3)
#define _LCD_R1_ON         SymbolON(16,4)
#define _LCD_R8_ON         SymbolON(16,5)
#define _LCD_R7_ON         SymbolON(16,6)
#define _LCD_R6_ON         SymbolON(16,7)
#define _LCD_COL2_ON       SymbolON(17,0)
#define _LCD_FREESCALE_ON  SymbolON(17,1)
#define _LCD_CLOCK_ON      SymbolON(17,2)
#define _LCD_AM_ON         SymbolON(17,3)
#define _LCD_PM_ON         SymbolON(17,4)
#define _LCD_COL1_ON       SymbolON(17,5)
#define _LCD_DOT1_ON       SymbolON(17,6)
#define _LCD_R9_ON         SymbolON(17,7)
#define _LCD_V1_ON         SymbolON(18,0)
#define _LCD_V2_ON         SymbolON(18,1)
#define _LCD_V3_ON         SymbolON(18,2)
#define _LCD_V4_ON         SymbolON(18,3)
#define _LCD_V5_ON         SymbolON(18,4)
#define _LCD_V6_ON         SymbolON(18,5)
#define _LCD_V7_ON         SymbolON(18,6)
#define _LCD_V8_ON         SymbolON(18,7)
#define _LCD_E1_ON         SymbolON(19,0)
#define _LCD_E2_ON         SymbolON(19,1)
#define _LCD_E3_ON         SymbolON(19,2)
#define _LCD_E4_ON         SymbolON(19,3)
#define _LCD_N3_ON         SymbolON(19,4)
#define _LCD_N6_ON         SymbolON(19,5)
#define _LCD_N9_ON         SymbolON(19,6)
#define _LCD_N12_ON        SymbolON(19,7)
#define _LCD_N1_ON         SymbolON(20,0)
#define _LCD_N4_ON         SymbolON(20,1)
#define _LCD_N7_ON         SymbolON(20,2)
#define _LCD_N11_ON        SymbolON(20,3)
#define _LCD_N2_ON         SymbolON(20,4)
#define _LCD_N5_ON         SymbolON(20,5)
#define _LCD_N8_ON         SymbolON(20,6)
#define _LCD_N10_ON        SymbolON(20,7)
#define _LCD_D1_ON         SymbolON(21,0)
#define _LCD_D2_ON         SymbolON(21,1)
#define _LCD_D3_ON         SymbolON(21,2)
#define _LCD_D4_ON         SymbolON(21,3)
#define _LCD_S2_ON         SymbolON(21,4)
#define _LCD_S1_ON         SymbolON(21,5)
#define _LCD_S3_ON         SymbolON(21,6)
#define _LCD_S4_ON         SymbolON(21,7)

/*
SymbolOFF
 Turns off the  segment   that its dedicated to write one Symbol     
param:  LCDpin --- LCDn number of pin that its connected   BitNumber -- COM Number that turns off that Symbol 
*/
#define   _LCD_R5_OFF           SymbolOFF(16,0)
#define   _LCD_R4_OFF           SymbolOFF(16,1)
#define   _LCD_R3_OFF           SymbolOFF(16,2)
#define   _LCD_R2_OFF           SymbolOFF(16,3)
#define   _LCD_R1_OFF           SymbolOFF(16,4)
#define   _LCD_R8_OFF           SymbolOFF(16,5)
#define   _LCD_R7_OFF           SymbolOFF(16,6)
#define   _LCD_R6_OFF           SymbolOFF(16,7)
#define   _LCD_COL2_OFF         SymbolOFF(17,0)
#define   _LCD_FREESCALE_OFF    SymbolOFF(17,1)
#define   _LCD_CLOCK_OFF        SymbolOFF(17,2)
#define   _LCD_AM_OFF           SymbolOFF(17,3)
#define   _LCD_PM_OFF           SymbolOFF(17,4)
#define   _LCD_COL1_OFF         SymbolOFF(17,5)
#define   _LCD_DOT1_OFF         SymbolOFF(17,6)
#define   _LCD_R9_OFF           SymbolOFF(17,7)
#define   _LCD_V1_OFF           SymbolOFF(18,0)
#define   _LCD_V2_OFF           SymbolOFF(18,1)
#define   _LCD_V3_OFF           SymbolOFF(18,2)
#define   _LCD_V4_OFF           SymbolOFF(18,3)
#define   _LCD_V5_OFF           SymbolOFF(18,4)
#define   _LCD_V6_OFF           SymbolOFF(18,5)
#define   _LCD_V7_OFF           SymbolOFF(18,6)
#define   _LCD_V8_OFF           SymbolOFF(18,7)
#define   _LCD_E1_OFF           SymbolOFF(19,0)
#define   _LCD_E2_OFF           SymbolOFF(19,1)
#define   _LCD_E3_OFF           SymbolOFF(19,2)
#define   _LCD_E4_OFF           SymbolOFF(19,3)
#define   _LCD_N3_OFF           SymbolOFF(19,4)
#define   _LCD_N6_OFF           SymbolOFF(19,5)
#define   _LCD_N9_OFF           SymbolOFF(19,6)
#define   _LCD_N12_OFF          SymbolOFF(19,7)
#define   _LCD_N1_OFF           SymbolOFF(20,0)
#define   _LCD_N4_OFF           SymbolOFF(20,1)
#define   _LCD_N7_OFF           SymbolOFF(20,2)
#define   _LCD_N11_OFF          SymbolOFF(20,3)
#define   _LCD_N2_OFF           SymbolOFF(20,4)
#define   _LCD_N5_OFF           SymbolOFF(20,5)
#define   _LCD_N8_OFF           SymbolOFF(20,6)
#define   _LCD_N10_OFF          SymbolOFF(20,7)
#define   _LCD_D1_OFF           SymbolOFF(21,0)
#define   _LCD_D2_OFF           SymbolOFF(21,1)
#define   _LCD_D3_OFF           SymbolOFF(21,2)
#define   _LCD_D4_OFF           SymbolOFF(21,3)
#define   _LCD_S2_OFF           SymbolOFF(21,4)
#define   _LCD_S1_OFF           SymbolOFF(21,5)
#define   _LCD_S3_OFF           SymbolOFF(21,6)
#define   _LCD_S4_OFF           SymbolOFF(21,7)
/*
MapSegments
Give the apropiate mask to set one segment of the display
param: BitNumber  -- Number of bit that needs to be turn on
*/ 
#define  SEG1         MapSegment (0)
#define  SEG2         MapSegment (1)
#define  SEG3         MapSegment (2)
#define  SEG4         MapSegment (3)
#define  SEG5         MapSegment (4)
#define  SEG6         MapSegment (5)
#define  SEG7         MapSegment (6)
#define  SEG8         MapSegment (7)

/*
Name Segments 
Create a mask to turn on alphanumeric segments
*/ 
#define SEGA	SEG1
#define SEGF	SEG2
#define SEGH	SEG3
#define SEGJ  SEG4
#define SEGL	SEG5   
#define SEGE	SEG6    
#define SEGN	SEG7   
#define SEGO	SEG8   

#define SEGP	SEG1   
#define SEGB	SEG2    
#define SEGK	SEG3   
#define SEGG	SEG4   
#define SEGC	SEG5    
#define SEGI	SEG6   
#define SEGM	SEG7   
#define SEGD	SEG8    

#define _Backupa	SEGA + SEGF + SEGH + SEGJ + SEGL + SEGE + SEGN + SEGO
#define _Backupb	SEGP + SEGB +SEGK + SEGG + SEGC + SEGI + SEGM + SEGD

//Ascii table 16 segments
#define  _0      ( SEGA +  SEGF + !SEGH + !SEGJ + !SEGL +  SEGE + !SEGN +  SEGO)  ,     ( SEGP +  SEGB + !SEGK + !SEGG +  SEGC + !SEGI + !SEGM +  SEGD)  ,   //Char = 0 ,   offset =0
#define  _1      (!SEGA + !SEGF + !SEGH + !SEGJ + !SEGL + !SEGE + !SEGN + !SEGO)  ,     (!SEGP +  SEGB + !SEGK + !SEGG +  SEGC + !SEGI + !SEGM + !SEGD)  ,   //Char = 1 ,   offset =2
#define  _2      ( SEGA + !SEGF + !SEGH + !SEGJ +  SEGL +  SEGE + !SEGN +  SEGO)  ,     ( SEGP +  SEGB + !SEGK +  SEGG + !SEGC + !SEGI + !SEGM +  SEGD)  ,   //Char = 2 ,   offset =4
#define  _3      ( SEGA + !SEGF + !SEGH + !SEGJ +  SEGL + !SEGE + !SEGN +  SEGO)  ,     ( SEGP +  SEGB + !SEGK +  SEGG +  SEGC + !SEGI + !SEGM +  SEGD)  ,   //Char = 3 ,   offset =6
#define  _4      (!SEGA +  SEGF + !SEGH + !SEGJ +  SEGL + !SEGE + !SEGN + !SEGO)  ,     (!SEGP +  SEGB + !SEGK +  SEGG +  SEGC + !SEGI + !SEGM + !SEGD)  ,   //Char = 4 ,   offset =8
#define  _5      ( SEGA +  SEGF + !SEGH + !SEGJ +  SEGL + !SEGE + !SEGN +  SEGO)  ,     ( SEGP + !SEGB + !SEGK +  SEGG +  SEGC + !SEGI + !SEGM +  SEGD)  ,   //Char = 5 ,   offset =10
#define  _6      (!SEGA +  SEGF + !SEGH + !SEGJ +  SEGL +  SEGE + !SEGN +  SEGO)  ,     (!SEGP + !SEGB + !SEGK +  SEGG +  SEGC + !SEGI + !SEGM +  SEGD)  ,   //Char = 6 ,   offset =12
#define  _7      ( SEGA + !SEGF + !SEGH + !SEGJ + !SEGL + !SEGE + !SEGN + !SEGO)  ,     ( SEGP +  SEGB + !SEGK + !SEGG +  SEGC + !SEGI + !SEGM + !SEGD)  ,   //Char = 7 ,   offset =14
#define  _8      ( SEGA +  SEGF + !SEGH + !SEGJ +  SEGL +  SEGE + !SEGN +  SEGO)  ,     ( SEGP +  SEGB + !SEGK +  SEGG +  SEGC + !SEGI + !SEGM +  SEGD)  ,   //Char = 8 ,   offset =16
#define  _9      ( SEGA +  SEGF + !SEGH + !SEGJ +  SEGL + !SEGE + !SEGN + !SEGO)  ,     ( SEGP +  SEGB + !SEGK +  SEGG +  SEGC + !SEGI + !SEGM + !SEGD)  ,   //Char = 9 ,   offset =18
#define  _AllOff      (!SEGA + !SEGF + !SEGH + !SEGJ + !SEGL + !SEGE + !SEGN + !SEGO)  ,     (!SEGP + !SEGB + !SEGK + !SEGG + !SEGC + !SEGI + !SEGM + !SEGD)  ,   //Char = : ,   offset =20
#define  _SemiColon   (!SEGA + !SEGF + !SEGH + !SEGJ + !SEGL + !SEGE + !SEGN + !SEGO)  ,     (!SEGP + !SEGB + !SEGK +  SEGG + !SEGC +  SEGI + !SEGM + !SEGD)  ,   //Char = ; ,   offset =22
#define  _MinusThan   (!SEGA + !SEGF + !SEGH + !SEGJ + !SEGL + !SEGE + !SEGN + !SEGO)  ,     (!SEGP + !SEGB +  SEGK + !SEGG + !SEGC +  SEGI + !SEGM + !SEGD)  ,   //Char = < ,   offset =24
#define  _Equal       ( SEGA + !SEGF + !SEGH + !SEGJ +  SEGL + !SEGE + !SEGN + !SEGO)  ,     ( SEGP + !SEGB + !SEGK +  SEGG + !SEGC + !SEGI + !SEGM + !SEGD)  ,   //Char = = ,   offset =26
#define  _GreaterThan (!SEGA + !SEGF +  SEGH + !SEGJ + !SEGL + !SEGE +  SEGN + !SEGO)  ,     (!SEGP + !SEGB + !SEGK + !SEGG + !SEGC + !SEGI + !SEGM + !SEGD)  ,   //Char = > ,   offset =28
#define   _Question   ( SEGA + !SEGF + !SEGH + !SEGJ +  SEGL +  SEGE + !SEGN + !SEGO)  ,     ( SEGP +  SEGB + !SEGK +  SEGG + !SEGC + !SEGI + !SEGM + !SEGD)  ,   //Char = ? ,   offset =30
#define  _AllOn       ( SEGA +  SEGF +  SEGH +  SEGJ +  SEGL +  SEGE +  SEGN +  SEGO)  ,     ( SEGP +  SEGB +  SEGK +  SEGG +  SEGC +  SEGI +  SEGM +  SEGD)  ,   //Char = @ ,   offset =32
#define  _A      ( SEGA +  SEGF + !SEGH + !SEGJ +  SEGL +  SEGE + !SEGN + !SEGO)  ,     ( SEGP +  SEGB + !SEGK +  SEGG +  SEGC + !SEGI + !SEGM + !SEGD)  ,   //Char = A ,   offset =34
#define  _B      ( SEGA +  SEGF + !SEGH + !SEGJ +  SEGL +  SEGE + !SEGN +  SEGO)  ,     ( SEGP +  SEGB + !SEGK +  SEGG +  SEGC + !SEGI + !SEGM +  SEGD)  ,   //Char = B ,   offset =36
#define  _C      ( SEGA +  SEGF + !SEGH + !SEGJ + !SEGL +  SEGE + !SEGN +  SEGO)  ,     ( SEGP + !SEGB + !SEGK + !SEGG + !SEGC + !SEGI + !SEGM +  SEGD)  ,   //Char = C ,   offset =38
#define  _D      (!SEGA +  SEGF +  SEGH + !SEGJ + !SEGL +  SEGE +  SEGN + !SEGO)  ,     (!SEGP + !SEGB + !SEGK + !SEGG + !SEGC + !SEGI + !SEGM + !SEGD)  ,   //Char = D ,   offset =40
#define  _E      ( SEGA +  SEGF + !SEGH + !SEGJ +  SEGL +  SEGE + !SEGN +  SEGO)  ,     ( SEGP + !SEGB + !SEGK +  SEGG + !SEGC + !SEGI + !SEGM +  SEGD)  ,   //Char = E ,   offset =42
#define  _F      ( SEGA +  SEGF + !SEGH + !SEGJ +  SEGL +  SEGE + !SEGN + !SEGO)  ,     ( SEGP + !SEGB + !SEGK +  SEGG + !SEGC + !SEGI + !SEGM + !SEGD)  ,   //Char = F ,   offset =44
#define  _G      ( SEGA +  SEGF + !SEGH + !SEGJ +  SEGL +  SEGE + !SEGN +  SEGO)  ,     ( SEGP + !SEGB + !SEGK +  SEGG +  SEGC + !SEGI + !SEGM +  SEGD)  ,   //Char = G ,   offset =46
#define  _H      (!SEGA +  SEGF + !SEGH + !SEGJ +  SEGL +  SEGE + !SEGN + !SEGO)  ,     (!SEGP +  SEGB + !SEGK +  SEGG +  SEGC + !SEGI + !SEGM + !SEGD)  ,   //Char = H ,   offset =48
#define  _I      (!SEGA + !SEGF + !SEGH +  SEGJ + !SEGL + !SEGE + !SEGN + !SEGO)  ,     (!SEGP + !SEGB + !SEGK + !SEGG + !SEGC + !SEGI +  SEGM + !SEGD)  ,   //Char = I ,   offset =50
#define  _J      (!SEGA + !SEGF + !SEGH + !SEGJ + !SEGL + !SEGE + !SEGN +  SEGO)  ,     (!SEGP +  SEGB + !SEGK + !SEGG +  SEGC + !SEGI + !SEGM +  SEGD)  ,   //Char = J ,   offset =52
#define  _K      (!SEGA +  SEGF + !SEGH + !SEGJ +  SEGL +  SEGE + !SEGN + !SEGO)  ,     (!SEGP + !SEGB +  SEGK + !SEGG + !SEGC +  SEGI + !SEGM + !SEGD)  ,   //Char = K ,   offset =54
#define  _L      (!SEGA +  SEGF + !SEGH + !SEGJ + !SEGL +  SEGE + !SEGN +  SEGO)  ,     (!SEGP + !SEGB + !SEGK + !SEGG + !SEGC + !SEGI + !SEGM +  SEGD)  ,   //Char = L ,   offset =56
#define  _M      (!SEGA +  SEGF +  SEGH + !SEGJ + !SEGL +  SEGE + !SEGN + !SEGO)  ,     (!SEGP +  SEGB +  SEGK + !SEGG +  SEGC + !SEGI + !SEGM + !SEGD)  ,   //Char = M ,   offset =58
#define  _N      (!SEGA +  SEGF +  SEGH + !SEGJ + !SEGL +  SEGE + !SEGN + !SEGO)  ,     (!SEGP +  SEGB + !SEGK + !SEGG +  SEGC +  SEGI + !SEGM + !SEGD)  ,   //Char = N ,   offset =60
#define  _O      ( SEGA +  SEGF + !SEGH + !SEGJ + !SEGL +  SEGE + !SEGN +  SEGO)  ,     ( SEGP +  SEGB + !SEGK + !SEGG +  SEGC + !SEGI + !SEGM +  SEGD)  ,   //Char = O ,   offset =62
#define  _P      ( SEGA +  SEGF + !SEGH + !SEGJ +  SEGL +  SEGE + !SEGN + !SEGO)  ,     ( SEGP +  SEGB + !SEGK +  SEGG + !SEGC + !SEGI + !SEGM + !SEGD)  ,   //Char = P ,   offset =64
#define  _Q      ( SEGA +  SEGF + !SEGH + !SEGJ +  SEGL +  SEGE + !SEGN +  SEGO)  ,     ( SEGP +  SEGB + !SEGK +  SEGG +  SEGC + !SEGI + !SEGM +  SEGD)  ,   //Char = Q ,   offset =66
#define  _R      ( SEGA +  SEGF + !SEGH + !SEGJ +  SEGL +  SEGE + !SEGN + !SEGO)  ,     ( SEGP +  SEGB + !SEGK +  SEGG + !SEGC +  SEGI + !SEGM + !SEGD)  ,   //Char = R ,   offset =68
#define  _S      ( SEGA +  SEGF + !SEGH + !SEGJ +  SEGL + !SEGE + !SEGN +  SEGO)  ,     ( SEGP + !SEGB + !SEGK +  SEGG +  SEGC + !SEGI + !SEGM +  SEGD)  ,   //Char = S ,   offset =70
#define  _T      ( SEGA + !SEGF + !SEGH +  SEGJ + !SEGL + !SEGE + !SEGN + !SEGO)  ,     ( SEGP + !SEGB + !SEGK + !SEGG + !SEGC + !SEGI +  SEGM + !SEGD)  ,   //Char = T ,   offset =72
#define  _U      (!SEGA +  SEGF + !SEGH + !SEGJ + !SEGL +  SEGE + !SEGN +  SEGO)  ,     (!SEGP +  SEGB + !SEGK + !SEGG +  SEGC + !SEGI + !SEGM +  SEGD)  ,   //Char = U ,   offset =74
#define  _V      (!SEGA + !SEGF +  SEGH + !SEGJ + !SEGL + !SEGE + !SEGN + !SEGO)  ,     (!SEGP +  SEGB + !SEGK + !SEGG +  SEGC +  SEGI + !SEGM + !SEGD)  ,   //Char = V ,   offset =76
#define  _W      (!SEGA +  SEGF + !SEGH +  SEGJ + !SEGL +  SEGE + !SEGN +  SEGO)  ,     (!SEGP +  SEGB + !SEGK + !SEGG +  SEGC + !SEGI +  SEGM +  SEGD)  ,   //Char = W ,   offset =78
#define  _X      (!SEGA + !SEGF +  SEGH + !SEGJ + !SEGL + !SEGE +  SEGN + !SEGO)  ,     (!SEGP + !SEGB +  SEGK + !SEGG + !SEGC +  SEGI + !SEGM + !SEGD)  ,   //Char = X ,   offset =80
#define  _Y      (!SEGA + !SEGF +  SEGH + !SEGJ + !SEGL + !SEGE + !SEGN + !SEGO)  ,     (!SEGP + !SEGB +  SEGK + !SEGG + !SEGC + !SEGI +  SEGM + !SEGD)  ,   //Char = Y ,   offset =82
#define  _Z      ( SEGA + !SEGF + !SEGH + !SEGJ + !SEGL + !SEGE +  SEGN +  SEGO)  ,     ( SEGP +  SEGB + !SEGK +  SEGG + !SEGC + !SEGI + !SEGM +  SEGD)  ,   //Char = Z ,   offset =84



/*********************************************** LCD_H ***********************************************************/


#ifdef MC9RS08LA8
   #define RamBufferSize 			29
   #define  _POINTER				*__paged
#endif

#ifdef MC9RS08LE4
  #define RamBufferSize 22
  #define  _POINTER				*__paged
#endif

#ifdef MC9S08LL16
  #define RamBufferSize 			32
  #define _POINTER				*
  #define vfnLCD_Contrast(Contrast)		LCDRVC_RVTRIM=Contrast	/* Write the contrast value to the register on the mcu */
#endif

#ifdef 	MC9S08LG32
  #define RamBufferSize 			45
  #define _POINTER				*
  #define vfnLCD_Contrast(Contrast)		LCDRVC_RVTRIM=Contrast	/* Write the contrast value to the register on the mcu */
#endif

extern byte RamBuffer[RamBufferSize];

/*{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{        Macros            }}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}*/
/*{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{   ---------------------- }}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}*/
#define LCDBCTL_INIT     (LCDBCTL = (_LCDBLINKCONTROL<<7) | (_LCDALTMODE<<6) | (_LCDBLANKDISP<<5)  | (_LCDBLINKMODE<<3) | (_LCDBLINKRATE))  
#define LCDC1_INIT       (LCDC1 =  (_LCDFRAMEINTERRUPT<<7) | (_LCDFULLCPLDIRIVE<<2)   | (_LCDWAITMODE<<1) | (_LCDSTOPMODE) )
#define LCDSUPPLY_INIT   (LCDSUPPLY =  (_LCDCPSEL<<7)  | (_LCDLOADADJUST<<4) | (_LCDVSUPPLY)) 
#define LCDC0_INIT       (LCDC0 = (_LCDENABLE<<7) | (_LCDCLKSOURCE<<6) | (_LCDCLKPSL<<3)| (_LCDDUTY))  
#define LCDENABLE        (LCDC0_LCDEN=1)
#define LCDDISABLE       (LCDC0_LCDEN=0)

#define vfnLCD_Home()		    bLCD_CharPosition = 0	
#define vfnLCD_GoTo(Place)	bLCD_CharPosition = Place*_LCDTYPE


extern  byte bLCD_CharPosition;

/*{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{        Functions        }}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}*/
/*{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{   --------------------- }}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}*/

/*||||||||||||||||||||||       vfnLCD_Init            ||||||||||||||||||||||
brief:  Initialize all the registers on the mcu module 
param:  void                            return:  void    						*/  
void vfnLCD_Init  (void);

/*||||||||||||||||||||||       vfnLCD_EnablePins   ||||||||||||||||||||
brief: Enable all the pins of the mcu directly connected to the  LCD on use
param:  void                            return:  void    						*/  
void vfnEnablePins (void);

 /*||||||||||||||||||||||       vfnLCD_ConfigureBackplanes ||||||||||||||
brief: Enable the function of Backplane and Set the COM number that corresponds 
param:  void                            return:  void     						*/  
void vfnLCD_ConfigureBackplanes (void);

 /*||||||||||||||||||||||       vfnLCD_Clear_Display   ||||||||||||||||||||||
brief: Turns off all the segments of the LCD
param:  none			return: void
*/
void vfnLCD_Clear_Display (void);

 /*||||||||||||||||||||||       vfnLCD_Set_Display   ||||||||||||||||||||||
brief: Turns on  all the segments of the LCD
param:  none			return: void
*/
void vfnLCD_Set_Display (void);
 /*||||||||||||||||||||||       vfnLCD_Write_Char   ||||||||||||||||||||||
brief:  Writes one char after the last character was write
param:  byte --- Ascii to write                            return:  void    
*/
void vfnLCD_Write_Char (byte lbValue);


 /*||||||||||||||||||||||       vfnLCD_Write_Msg    ||||||||||||||||||||||
brief:  Writes one message on the LCD   if the message is longer than the number of character only write
the maximum number of characters
param:  pointer to unsigned char  --- the first character  on the array to write   				
return:  void    													*/
void  vfnLCD_Write_Msg (byte _POINTER lbpMessage);

/*||||||||||||||||||||||       vfnLCD_Write_MsgPlace    ||||||||||||||||||||||
brief:  Writes one message on the LCD   if the message is longer than the number of character  of character only write
the maximum number of characters and you can select the position to write
param:  pointer to unsigned char  --- the first character  on the array to write
param: unsigned char --- the numbers of characterts to write 
param: unsigned char --- place to write  				
return:  void */   			
 void  vfnLCD_Write_MsgPlace(byte _POINTER lbpMessage, byte n_chars, byte lbPlace); 


/*~|~|~|~|~|~|~|~|~|~|~|~|~|~  LCD  Control Register 0  ~|~|~|~|~|~|~|~|~|~|~|~|~*/
/*~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|*/

#define _LCDENABLE           (0)                    /* 0 All frontplanes and backplanes are disable */
                                                    /* 1 LCD module is enable */
#define  _LCDDUTY            (_LCDBACKPLANES-1)     /* # of Backplanes */

/*~|~|~|~|~|~|~|~|~|~|~|~|~|~  LCD  Control Register 1  ~|~|~|~|~|~|~|~|~|~|~|~|~|*/
/*~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|*/
#define _LCDFULLCPLDIRIVE    (0)       /* 0 GPIO shared with the LCD. Inputs levels and internal pullup reference to VDD */
                                       /* 1 If VSUPPLY=11and RVEN=0. Inputs levels and internal pullup reference to VLL3 */
#define _LCDWAITMODE         (0)       /* 0 Allows the LCD driver and charge pump to conitnue running during wait mode */
                                       /* 1 Disable the LCD when the MCU goes into wait mode */
#define _LCDSTOPMODE         (0)       /* 0 Allows the LCD driver and charge pump to conitnue running during stop2 or stop3 */
                                       /* 1 Disable the LCD when and charge pump when the MCU goes into stop2 or stop3 */

/*~|~|~|~|~|~|~|~|~|~|~|~|~|~  LCD  Voltage Supply Register  ~|~|~|~|~|~|~|~|~|~|~|~*/
/*~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|*/
#define _LCDHIGHREF          (0)       /* 0 Divide input VIREG=1.0v */
                                       /* 1 Do not divide the input VIREG=1.67v */
#define _LCDBBYPASS          (1)       /* Determines wheter the internal LCD op amp buffer is bypassed */
                                       /* 0 Buffered mode */
                                       /* 1 Unbuffered mode */
                            
/*~|~|~|~|~|~|~|~|~|~|~|~|~|~  LCD  Regulated Voltage Control |~|~|~|~|~|~|~|~|~|~*/
/*~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|*/
#define _LCDCONTRAST         (1)       /* Contrast by software   0 -- Disable    1-- Enable */
#define _LVLCONTRAST         (0)       /* Any number between 0  and 15, if the number is bigger the glass get darker */

/*~|~|~|~|~|~|~|~|~|~|~|~|~|~  LCD  Blink Control Register ~|~|~|~|~|~|~|~|~|~|~|~*/
/*~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|*/
#define _LCDBLINKCONTROL     (0)       /* 0 Disable blink mode */
                                       /* 1 Enable blink mode */
#define _LCDALTMODE          (0)       /* 0 Normal display */
                                       /* 1 Alternate display for 4 backplanes or less the LCD backplane sequencer changes to otuput an alternate display */
#define _LCDBLANKDISP        (0)       /* 0 Do not blank display */
                                       /* 1 Blank display if you put it in 0 the text before blank is manteined */
#define _LCDBLINKMODE        (0)       /* 0 Display blank during the blink period */
                          


/*****************************************************  I2CDrv_H ************************************************************/

#define     EIGHT_BIT_LOGICAL_ADDRESS
#undef      SIXTEEN_BIT_LOGICAL_ADDRESS
#define     DEVICE_SLAVE_ADDRESS                      (0x58)
                                    
#ifndef IICA
    #define     IICA                IIC1A   
    #define     IICF                IIC1F
    #define     IICC                IIC1C
    #define     IICS                IIC1S
    #define     IICD                IIC1D
    #define     IICC2               IIC1C2
    #define     IICC_IICEN_MASK     IIC1C_IICIE_MASK
    #define     IICC_IICIE_MASK     IIC1C_IICEN_MASK
    #define     IICS_ARBL_MASK      IIC1S_ARBL_MASK
    #define     IICS_IAAS_MASK      IIC1S_IAAS_MASK
    #define     IICS_TCF_MASK       IIC1S_TCF_MASK
    #define     IICS_RXAK_MASK      IIC1S_RXAK_MASK
    #define     IICC_TXAK_MASK      IIC1C_TXAK_MASK
    #define     IICS_IICIF_MASK     IIC1S_IICIF_MASK
    #define     IICC_TX             IIC1C_TX
    #define     IICS_SRW            IIC1S_SRW
#endif

#ifndef VectorNumber_Viic
    #ifdef VectorNumber_Viic1
      #define VectorNumber_Viic   VectorNumber_Viic1
    #endif
    #ifdef VectorNumber_Viicx
      #define VectorNumber_Viic   VectorNumber_Viicx
    #endif
#endif

#define GENERAL_REGISTERS       0x00
#define STATUS_ELECTRODES       0x10 
#define ELECTRODES_ENABLERS     0x20 
#define SENSITIVITY             0x40 
#define INSTANT_DELTAS          0x30 
#define SENSITIVITY1            0x50 
#define SENSITIVITY2            0x60
#define SENSITIVITY3            0x70 
#define BASE_LINES              0x80
#define CHECKSUM                0xE0
#define ELECTRODE_NUMBER        0xF0

#define FLAG_SET(BitNumber, Register)        (Register |=(1<<BitNumber))
#define FLAG_CLR(BitNumber, Register)        (Register &=~(1<<BitNumber))
#define FLAG_CHK(BitNumber, Register)        (Register & (1<<BitNumber))

enum ISR
{
    I2C_Flag = 1
};

typedef enum
{
  I2C_INTERRUPT_FLAG,
  I2C_AAS_FLAG,
  I2C_ADDRESS_BYTE_FLAG,
}_eI2CFlags;

/**
 * \brief       This function contains the I2C communications state machine.
                This could be placed at the Interrupt function.
 * \author      Antonio Ramos Salido Maurer
 */  
void I2CDrv(void);
/**
 * \brief       Initialization of the I2C module as slave.
 * \author      Antonio Ramos Salido Maurer
 */  
void I2CInit(void);
uint8_t* u16AddressDecoder(uint8_t);
void EGTTask(void);

/*****************************************************************************/

#endif /* _MAIN_H_ */
