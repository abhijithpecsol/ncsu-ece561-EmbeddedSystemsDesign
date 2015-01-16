#ifndef _LCD_H_
    #define _LCD_H_

#include "LCD_HAL.h"

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
  #define vfnLCD_Contrast(Contrast)		LCDRVC_RVTRIM=Contrast	// Write the contrast value to the register on the mcu
#endif

#ifdef 	MC9S08LG32
  #define RamBufferSize 			45
  #define _POINTER				*
  #define vfnLCD_Contrast(Contrast)		LCDRVC_RVTRIM=Contrast	// Write the contrast value to the register on the mcu
#endif

extern byte RamBuffer[RamBufferSize];

/*{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{        Macros            }}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}*/
/*{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{   ----------------------------  }}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}*/
#define LCDBCTL_INIT     (LCDBCTL = (_LCDBLINKCONTROL<<7) | (_LCDALTMODE<<6) | (_LCDBLANKDISP<<5)  | (_LCDBLINKMODE<<3) | (_LCDBLINKRATE))  
#define LCDC1_INIT         (LCDC1 =  (_LCDFRAMEINTERRUPT<<7) | (_LCDFULLCPLDIRIVE<<2)   | (_LCDWAITMODE<<1) | (_LCDSTOPMODE) )
#define LCDRVC_INIT       (LCDRVC = (_LCDCONTRAST<<7) |   (_LVLCONTRAST))    
#define LCDSUPPLY_INIT (LCDSUPPLY =  (_LCDCPSEL<<7)  | (_LCDLOADADJUST<<4) | (_LCDVSUPPLY)) 
#define LCDC0_INIT         (LCDC0 = (_LCDENABLE<<7) | (_LCDCLKSOURCE<<6) | (_LCDCLKPSL<<3)| (_LCDDUTY))  
#define LCDENABLE          (LCDC0_LCDEN=1)
#define LCDDISABLE        (LCDC0_LCDEN=0)

#define vfnLCD_Home()		bLCD_CharPosition = 0	
#define vfnLCD_GoTo(Place)	bLCD_CharPosition = Place*_LCDTYPE


extern  byte bLCD_CharPosition;

/*{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{        Functions        }}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}*/
/*{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{   ----------------------------  }}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}*/

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

#define _LCDENABLE           (0)                                                 //0 All frontplanes and backplanes are disable
                                                                                                        //1 LCD module is enable
#define  _LCDDUTY              (_LCDBACKPLANES-1)                 //# of Backplanes 

/*~|~|~|~|~|~|~|~|~|~|~|~|~|~  LCD  Control Register 1  ~|~|~|~|~|~|~|~|~|~|~|~|~|*/
/*~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|*/
#define _LCDFULLCPLDIRIVE     (0)       // 0 GPIO shared with the LCD. Inputs levels and internal pullup reference to VDD
                                                        // 1 If VSUPPLY=11and RVEN=0. Inputs levels and internal pullup reference to VLL3
#define _LCDWAITMODE            (0)     // 0 Allows the LCD driver and charge pump to conitnue running during wait mode
                                                        //  1 Disable the LCD when the MCU goes into wait mode
#define _LCDSTOPMODE             (0)     // 0 Allows the LCD driver and charge pump to conitnue running during stop2 or stop3
                                                        //  1 Disable the LCD when and charge pump when the MCU goes into stop2 or stop3                                                               

/*~|~|~|~|~|~|~|~|~|~|~|~|~|~  LCD  Voltage Supply Register  ~|~|~|~|~|~|~|~|~|~|~|~*/
/*~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|*/
#define _LCDHIGHREF                  (0)      //0 Divide input VIREG=1.0v
                                                           //1 Do not divide the input VIREG=1.67v
#define _LCDBBYPASS                    (1)      //Determines wheter the internal LCD op amp buffer is bypassed
                                                            //0 Buffered mode
                                                            //1 Unbuffered mode
                            
/*~|~|~|~|~|~|~|~|~|~|~|~|~|~  LCD  Regulated Voltage Control |~|~|~|~|~|~|~|~|~|~*/
/*~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|*/
#define _LCDCONTRAST                    (1)       //Contrast by software   0 -- Disable    1-- Enable
#define _LVLCONTRAST                     (0)       //Any number between 0  and 15, if the number is bigger the glass get darker

/*~|~|~|~|~|~|~|~|~|~|~|~|~|~  LCD  Blink Control Register ~|~|~|~|~|~|~|~|~|~|~|~*/
/*~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|*/
#define _LCDBLINKCONTROL         (0)   //0 Disable blink mode
                                                                       //1 Enable blink mode
#define _LCDALTMODE                    (0)   //0 Normal display 
                                                                      //1 Alternate display for 4 backplanes or less the LCD backplane sequencer changes to otuput an alternate display
#define _LCDBLANKDISP                  (0)  //0 Do not blank display
                                                                      //1 Blank display if you put it in 0 the text before blank is manteined     
#define _LCDBLINKMODE                (0)  //0 Display blank during the blink period 
                                                                       //1 Display alternate displat during blink period (Ignored if duty is 5 or greater)

#endif /* _LCD_H_ */
