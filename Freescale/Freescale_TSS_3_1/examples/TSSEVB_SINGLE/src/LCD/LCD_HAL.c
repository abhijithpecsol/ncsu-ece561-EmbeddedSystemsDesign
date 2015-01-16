#include "derivative.h" /* include peripheral declarations */
#include "LCD.h"

byte bLCD_CharPosition = 0;
byte RamBuffer[RamBufferSize];


byte bpaPlace [ ] =
{
	Char1a,   // LCD8 --- Pin:9
	Char1b,   // LCD9 --- Pin:10
	Char2a,   // LCD10 --- Pin:11
	Char2b,   // LCD11 --- Pin:12
	Char3a,   // LCD12 --- Pin:13
	Char3b,   // LCD13 --- Pin:14
	Char4a,   // LCD14 --- Pin:15
	Char4b   // LCD15 --- Pin:16
};

byte baBackup [ ] =
{
   _Backupa,
   _Backupb,
};

const byte baAscii [ ] =
{
	_0
	_1
	_2
	_3
	_4
	_5
	_6
	_7
	_8
	_9
	_AllOff
	_SemiColon
	_MinusThan
	_Equal
	_GreaterThan
	_Question
	_AllOn
	_A
	_B
	_C
	_D
	_E
	_F
	_G
	_H
	_I
	_J
	_K
	_L
	_M
	_N
	_O
	_P
	_Q
	_R
	_S
	_T
	_U
	_V
	_X
	_Y
	_Z        

} ;



void vfnLCD_Init  (void)
{
	SCGC2_LCD = 1;									//Enable clock to LCD module
	LCDDISABLE;
	LCDSUPPLY_INIT;								// Configure  the Voltage Supply 
	LCDC1_INIT;										// Configure  the LCD1 register
	LCDC0_INIT;										// Configure  the LCD0 register   			     
//  #if (LCDRVC != This_symb_has_been_depreciated)  // Due various availability of LCDRVC register in MC9S08LG32.h
//	  LCDRVC_INIT;								  // Configure  the Regulated Voltage Control
//	#endif
	LCDBCTL_INIT; 									// Configure  the Blink Control register
	vfnEnablePins ();									//Give to the LCDx the functionality  of a LCD pin
	vfnLCD_ConfigureBackplanes (); 			//Enable the characterisitcs of a Backplane
	LCDENABLE; 												// Enable the LCD module
}	


  						         	
  void vfnEnablePins (void)
  {
  	PEN0;											// Give to the LCDx the functionality  of a LCD pin
  	PEN1;
  	PEN2;
  	PEN3;
	PEN4;
	PEN5;
  }
  
 
  void vfnLCD_ConfigureBackplanes (void)
  {

		LCDBPEN0 = 0;
		LCDBPEN1 = 0;
		LCDBPEN2 = 0;
		LCDBPEN3 = 0;
		LCDBPEN4 = 0;
		LCDBPEN5 = 0;

		  
		BPEN0; 										//Enable the characterisitcs of a Backplane
		BPEN1;
		BPEN2;
		BPEN3;
		BPEN4;
		BPEN5;

		SetCom1;	 									//Give to each backplane previously enbale his COM number
		SetCom2;
		SetCom3;
		SetCom4;
		SetCom5;	 								
		SetCom6;
		SetCom7;
		SetCom8;


  }

 void  vfnLCD_Write_Msg (byte  _POINTER lbpMessage)
 {
      byte lbCounter;          
     
     vfnLCD_Home ();						//Point to the first alphanumeric
     lbCounter = _CHARNUM;
     
     
    while (lbCounter--)						
    {
      if (*lbpMessage)						//Write  String						 
      {
         vfnLCD_Write_Char (*lbpMessage);
         lbpMessage ++;
      }
      else								//Write Blanks 
      {
      	vfnLCD_Write_Char(' ');
      }
    }
      
 }
 
 void  vfnLCD_Write_MsgPlace(byte _POINTER lbpMessage, byte n_chars, byte lbPlace) 
 {
    byte lb_Counter = 0;          
    
    vfnLCD_GoTo(lbPlace);						//Point to the alphanumeric
   
    while (n_chars--)							//Write String 
    {
       if (bLCD_CharPosition <= _CHARNUM*_LCDTYPE) 
       {
	       vfnLCD_Write_Char (*lbpMessage);
	       lbpMessage ++;     
       }
    }
 } 

void vfnLCD_Write_Char (byte lbValue)
{
          byte *lbpLCDWF = &LCDWFRef;
          byte lbOffset=0;
          byte lbCounter ;
          byte *lbpLCDWFCopy = &RamBuffer[0];

          if(lbValue >= 'a' && lbValue <= 'z') lbValue=lbValue-'a' + 'A';				//Change 	small letters to capitals
          if(lbValue < '0' || lbValue > 'Z') lbValue=':';							//Change ascii space to alloff 
          
          lbValue = (lbValue-'0')*_LCDTYPE;								//Find the value at the ascii table
          lbCounter = _LCDTYPE;										//Set the number of waveforms needed to write one alphanumeric
          
          while (lbCounter--)
          {
          
             lbOffset = bpaPlace[bLCD_CharPosition];						//Find the Waveform offset                       
             *(lbpLCDWFCopy + lbOffset)&=(~(baBackup[lbCounter] & 0xFF));		//Backup of the waveforms
             *(lbpLCDWFCopy + lbOffset) |= baAscii[lbValue];
             *(lbpLCDWF + lbOffset) = *(lbpLCDWFCopy + lbOffset);			//Write the value to the waveform
             lbValue++;  
              bLCD_CharPosition++;           
          }            
  
}


void vfnLCD_Set_Display (void)
{
	Segment1_ON;       										//Turn on all the segments
	Segment2_ON;
	Segment3_ON;
	Segment4_ON;
	Segment5_ON;
	Segment6_ON;
	Segment7_ON;
	Segment8_ON;
	Segment9_ON;
	Segment10_ON;
	Segment11_ON;       									
	Segment12_ON ;
	Segment13_ON;
	Segment14_ON;

}

void vfnLCD_Clear_Display (void)
{
	Segment1_OFF;       										//Turn off all the segments
	Segment2_OFF;
	Segment3_OFF;
	Segment4_OFF;
	Segment5_OFF;
	Segment6_OFF;
	Segment7_OFF;
	Segment8_OFF;
	Segment9_OFF;
	Segment10_OFF;
	Segment11_OFF;       									
	Segment12_OFF ;
	Segment13_OFF;
	Segment14_OFF;
	
}
