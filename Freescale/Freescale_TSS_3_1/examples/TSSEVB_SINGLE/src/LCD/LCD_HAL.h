/*^^^^^^^^^^^^^^^^      LCD Configuration                        ^^^^^^^^^^^^^^^^^^^^^^^^*/
//#define MC9RS08LE4
//#define MC9RS08LA8
//#define MC9S08LL16
#define MC9S08LG32

#define _LCDBACKPLANES			(8)				// # of backplanes  
#define _LCDCLKSOURCE			  (1)				// 0 -- External clock       1 --  Alternate clock  (Internal)


#define _LCDCLKPSL					(3)				// Clock divider to generate the LCD Waveforms


#define _LCDCPSEL						(0)				// 0 -- Resistor network   1 -- Charge pump  
#define _LCDVSUPPLY					(1)  			// 0  --  VLL2 from VDD    1 --   VLL3 from VDD  2 -- VLL1 from VLCD  3 -- VLL3 external 
#define _LCDLOADADJUST			(0)			  // Charge pump 		0 -- 8000 pf 1 -- 6000 pf  2 -- 4000 pf  3 -- 2000 pf
                                                               //Resistor Network	0 -- 2000 pf 1 -- 2000 pf  2 -- 8000 pf   3 -- 8000 pf 

#define _LCDFRAMEINTERRUPT	(0)       // 0 Disabel  Interrupt 1 Enable aLCD interrupt that coincides with the LCD frame frequency

#define _LCDBLINKRATE 			(0)				// Blink frequecy preescaler  any number between 0-7

#define _CHARNUM					  (4)				// Number of characters on the display

#define _LCDTYPE						(2)				// Number of LCDWF  nedded  to drive one character 

#define This_symb_has_been_depreciated  (1)   // Due various availability of LCDRVC register in MC9S08LG32.h

/*^^^^^^^^^^^^^^^^      LCD Hardware Connection 			 ^^^^^^^^^^^^^^^^^^^^^^^^*/

#define EnableLCDpins(RegNum,Mask)			LCDPEN##RegNum = Mask
#define  EnableBackplanes(RegNum, Mask)	LCDBPEN##RegNum = Mask   
#define  SetBackplane(ComNum, LCDn)		LCDWF##LCDn  = (1 <<  (##ComNum##))
#define CharacterPlace(LCDn)        				LCDWF##LCDn
#define AllSegmentsON(LCDn,ComMask)		LCDWF##LCDn = ComMask
#define SymbolON(LCDn, BitNumber)			LCDWF##LCDn = (RamBuffer[LCDn] |= (1<< BitNumber))
#define SymbolOFF(LCDn, BitNumber)			LCDWF##LCDn = (RamBuffer[LCDn] &= ~(1<< BitNumber))
#define MapSegment(BitNumber)  				(1<<(##BitNumber##)) 
#define LCDWFRef          							LCDWF0
#define LCD_AlphanumericPlace(Place)			bLCD_CharPosition=_LCDTYPE*##(Place-1)
/*
EnableLCDpins
Habilitate the mcu pin with the characteristics of a LCD pin 
param: RegNum -- Number of register to write     Mask --- Mask that habilitate the LCDx pins
*/
	#define PEN0       EnableLCDpins (0,255)
	#define PEN1       EnableLCDpins (1,255)
	#define PEN2       EnableLCDpins (2,63)
	#define PEN3       EnableLCDpins (3,0)
	#define PEN4       EnableLCDpins (4,0)
	#define PEN5       EnableLCDpins (5,0)

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
#define   SetCom1        SetBackplane (0, 0)
#define   SetCom2        SetBackplane (1, 1)
#define   SetCom3        SetBackplane (2, 2)
#define   SetCom4        SetBackplane (3, 3)
#define   SetCom5        SetBackplane (4, 4)
#define   SetCom6        SetBackplane (5, 5)
#define   SetCom7        SetBackplane (6, 6)
#define   SetCom8        SetBackplane (7, 7)

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
#define Segment10_ON               AllSegmentsON ( 17,255 )
#define Segment11_ON               AllSegmentsON ( 18,255 )
#define Segment12_ON               AllSegmentsON ( 19,255 )
#define Segment13_ON               AllSegmentsON ( 20,255 )
#define Segment14_ON               AllSegmentsON ( 21,255 )

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
#define Segment10_OFF                AllSegmentsON ( 17, 0 )
#define Segment11_OFF                AllSegmentsON ( 18, 0 )
#define Segment12_OFF                AllSegmentsON ( 19, 0 )
#define Segment13_OFF                AllSegmentsON ( 20, 0 )
#define Segment14_OFF                AllSegmentsON ( 21, 0 )


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
#define _LCD_COL2_ON         SymbolON(17,0)
#define _LCD_FREESCALE_ON         SymbolON(17,1)
#define _LCD_CLOCK_ON         SymbolON(17,2)
#define _LCD_AM_ON         SymbolON(17,3)
#define _LCD_PM_ON         SymbolON(17,4)
#define _LCD_COL1_ON         SymbolON(17,5)
#define _LCD_DOT1_ON         SymbolON(17,6)
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
#define _LCD_N12_ON         SymbolON(19,7)
#define _LCD_N1_ON         SymbolON(20,0)
#define _LCD_N4_ON         SymbolON(20,1)
#define _LCD_N7_ON         SymbolON(20,2)
#define _LCD_N11_ON         SymbolON(20,3)
#define _LCD_N2_ON         SymbolON(20,4)
#define _LCD_N5_ON         SymbolON(20,5)
#define _LCD_N8_ON         SymbolON(20,6)
#define _LCD_N10_ON         SymbolON(20,7)
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
#define   _LCD_COL2_OFF           SymbolOFF(17,0)
#define   _LCD_FREESCALE_OFF           SymbolOFF(17,1)
#define   _LCD_CLOCK_OFF           SymbolOFF(17,2)
#define   _LCD_AM_OFF           SymbolOFF(17,3)
#define   _LCD_PM_OFF           SymbolOFF(17,4)
#define   _LCD_COL1_OFF           SymbolOFF(17,5)
#define   _LCD_DOT1_OFF           SymbolOFF(17,6)
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
#define   _LCD_N12_OFF           SymbolOFF(19,7)
#define   _LCD_N1_OFF           SymbolOFF(20,0)
#define   _LCD_N4_OFF           SymbolOFF(20,1)
#define   _LCD_N7_OFF           SymbolOFF(20,2)
#define   _LCD_N11_OFF           SymbolOFF(20,3)
#define   _LCD_N2_OFF           SymbolOFF(20,4)
#define   _LCD_N5_OFF           SymbolOFF(20,5)
#define   _LCD_N8_OFF           SymbolOFF(20,6)
#define   _LCD_N10_OFF           SymbolOFF(20,7)
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
#define SEGJ		SEG4
#define SEGL	SEG5   
#define SEGE	SEG6    
#define SEGN	SEG7   
#define SEGO	SEG8   

#define SEGP	SEG1   
#define SEGB	SEG2    
#define SEGK	SEG3   
#define SEGG	SEG4   
#define SEGC	SEG5    
#define SEGI		SEG6   
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
#define  _SemiColon      (!SEGA + !SEGF + !SEGH + !SEGJ + !SEGL + !SEGE + !SEGN + !SEGO)  ,     (!SEGP + !SEGB + !SEGK +  SEGG + !SEGC +  SEGI + !SEGM + !SEGD)  ,   //Char = ; ,   offset =22
#define  _MinusThan      (!SEGA + !SEGF + !SEGH + !SEGJ + !SEGL + !SEGE + !SEGN + !SEGO)  ,     (!SEGP + !SEGB +  SEGK + !SEGG + !SEGC +  SEGI + !SEGM + !SEGD)  ,   //Char = < ,   offset =24
#define  _Equal      ( SEGA + !SEGF + !SEGH + !SEGJ +  SEGL + !SEGE + !SEGN + !SEGO)  ,     ( SEGP + !SEGB + !SEGK +  SEGG + !SEGC + !SEGI + !SEGM + !SEGD)  ,   //Char = = ,   offset =26
#define  _GreaterThan      (!SEGA + !SEGF +  SEGH + !SEGJ + !SEGL + !SEGE +  SEGN + !SEGO)  ,     (!SEGP + !SEGB + !SEGK + !SEGG + !SEGC + !SEGI + !SEGM + !SEGD)  ,   //Char = > ,   offset =28
#define   _Question      ( SEGA + !SEGF + !SEGH + !SEGJ +  SEGL +  SEGE + !SEGN + !SEGO)  ,     ( SEGP +  SEGB + !SEGK +  SEGG + !SEGC + !SEGI + !SEGM + !SEGD)  ,   //Char = ? ,   offset =30
#define  _AllOn      ( SEGA +  SEGF +  SEGH +  SEGJ +  SEGL +  SEGE +  SEGN +  SEGO)  ,     ( SEGP +  SEGB +  SEGK +  SEGG +  SEGC +  SEGI +  SEGM +  SEGD)  ,   //Char = @ ,   offset =32
#define  _A      ( SEGA +  SEGF + !SEGH + !SEGJ +  SEGL +  SEGE + !SEGN + !SEGO)  ,     ( SEGP +  SEGB + !SEGK +  SEGG +  SEGC + !SEGI + !SEGM + !SEGD)  ,   //Char = A ,   offset =34
#define  _B      ( SEGA +  SEGF + !SEGH + !SEGJ +  SEGL +  SEGE + !SEGN +  SEGO)  ,     ( SEGP +  SEGB + !SEGK +  SEGG +  SEGC + !SEGI + !SEGM +  SEGD)  ,   //Char = B ,   offset =36
#define  _C      ( SEGA +  SEGF + !SEGH + !SEGJ + !SEGL +  SEGE + !SEGN +  SEGO)  ,     ( SEGP + !SEGB + !SEGK + !SEGG + !SEGC + !SEGI + !SEGM +  SEGD)  ,   //Char = C ,   offset =38
#define  _D      ( SEGA +  SEGF + !SEGH + !SEGJ + !SEGL +  SEGE + !SEGN +  SEGO)  ,     ( SEGP +  SEGB + !SEGK + !SEGG +  SEGC + !SEGI + !SEGM +  SEGD)  ,   //Char = D ,   offset =40
#define  _E      ( SEGA +  SEGF + !SEGH + !SEGJ +  SEGL +  SEGE + !SEGN +  SEGO)  ,     ( SEGP + !SEGB + !SEGK +  SEGG + !SEGC + !SEGI + !SEGM +  SEGD)  ,   //Char = E ,   offset =42
#define  _F      ( SEGA +  SEGF + !SEGH + !SEGJ +  SEGL +  SEGE + !SEGN + !SEGO)  ,     ( SEGP + !SEGB + !SEGK +  SEGG + !SEGC + !SEGI + !SEGM + !SEGD)  ,   //Char = F ,   offset =44
#define  _G      ( SEGA +  SEGF + !SEGH + !SEGJ +  SEGL +  SEGE + !SEGN +  SEGO)  ,     ( SEGP + !SEGB + !SEGK +  SEGG +  SEGC + !SEGI + !SEGM +  SEGD)  ,   //Char = G ,   offset =46
#define  _H      (!SEGA +  SEGF + !SEGH + !SEGJ +  SEGL +  SEGE + !SEGN + !SEGO)  ,     (!SEGP +  SEGB + !SEGK +  SEGG +  SEGC + !SEGI + !SEGM + !SEGD)  ,   //Char = H ,   offset =48
#define  _I      (!SEGA + !SEGF + !SEGH +  SEGJ + !SEGL + !SEGE + !SEGN + !SEGO)  ,     (!SEGP + !SEGB + !SEGK + !SEGG + !SEGC + !SEGI +  SEGM + !SEGD)  ,   //Char = I ,   offset =50
#define  _J      (!SEGA + !SEGF + !SEGH + !SEGJ + !SEGL + !SEGE + !SEGN +  SEGO)  ,     (!SEGP +  SEGB + !SEGK + !SEGG +  SEGC + !SEGI + !SEGM +  SEGD)  ,   //Char = J ,   offset =52
#define  _K      (!SEGA +  SEGF + !SEGH + !SEGJ +  SEGL +  SEGE + !SEGN + !SEGO)  ,     (!SEGP + !SEGB +  SEGK +  SEGG + !SEGC +  SEGI + !SEGM + !SEGD)  ,   //Char = K ,   offset =54
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
