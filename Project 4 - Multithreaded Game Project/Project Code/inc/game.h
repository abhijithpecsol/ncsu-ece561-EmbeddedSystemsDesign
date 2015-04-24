/*
 * Game.h
 *
 */

#include <stdint.h>
#include "tft_lcd.h"

// Main character
#define CHAR_BODY_WIDTH 	(26)
#define CHAR_BODY_HEIGHT	(40)
#define CHAR_LEG_WIDTH		(5)
#define CHAR_LEG_HEIGHT		(10)
#define CHAR_LEG_INSET		(5)
#define CHAR_LEG_GAP			(CHAR_BODY_WIDTH - 2*CHAR_LEG_WIDTH - 2*CHAR_LEG_INSET)
#define CHAR_START_POS_X	((TFT_WIDTH - CHAR_BODY_WIDTH)/2)
#define CHAR_START_POS_Y	(PLAT_START_POS_Y - CHAR_BODY_HEIGHT - CHAR_LEG_HEIGHT)
#define ACCEL_GRAVITY			(1)		// downward is +y
#define JUMP_VELOCITY			(-13)
#define TERMINAL_VELOCITY	(25)
#define MAX_CHAR_DISPLACEMENT	(-(JUMP_VELOCITY))
typedef struct {
	PT_T loc;					// top left location of character
	int16_t Vx, Vy;		// movement velocity
} CHARACTER_T;

// Platforms
#define PLAT_WIDTH				(50)
#define PLAT_HEIGHT				(10)
#define PLAT_START_POS_DIST_BOT	(10)
#define PLAT_START_POS_X	((TFT_WIDTH - PLAT_WIDTH)/2)
#define PLAT_START_POS_Y	(TFT_HEIGHT - PLAT_START_POS_DIST_BOT - PLAT_HEIGHT)
#define MAX_NUM_PLATFORMS	(15)
typedef struct {		// Represents a platform
	PT_T loc;					// top left location of platform
} PLATFORM_T;
typedef struct {		// Holds a set number of platforms
	PLATFORM_T platforms[MAX_NUM_PLATFORMS];		// TODO - switch to using pointers and malloc to save memory
	uint8_t first;
	uint8_t next;
	uint8_t num;
} PLATFORM_FIFO_T;

// Camera scroller
#define NO_HIT						(0x00)
#define HIT_PLATFORM			(0x01)
#define SCROLL_DOWN_LIMIT	(25)
#define CAM_SCROLL_SPEED	(3)


// ****************
// HELPER FUNCTIONS
// ****************
void Game_Init(CHARACTER_T * ch);											// Create initial game state
void Draw_Character(CHARACTER_T * ch, COLOR_T * c);		// Draw character in the selected color
void Erase_Character(CHARACTER_T * ch, COLOR_T * bg);	// Erase character with the selected background color
void Redraw_Character(CHARACTER_T * ch);							// Erases and redraws the character fast, avoids flickering.
PLATFORM_T * Add_Platform(void);											// Returns a pointer to a platform that may be created. If one cannot be created, returns null.
void Draw_Platform(PLATFORM_T * p, COLOR_T * c);			// Draw platform in the selected color
void Erase_Platform(PLATFORM_T * p, COLOR_T * bg);		// Erase platform with the selected background color
void Redraw_Platforms(void);													// Redraw all platforms to counter erased content
void Move_Character(CHARACTER_T * ch);								// Move character based on velocity and adjust velocity
uint8_t Detect_Collision(CHARACTER_T * ch);						// Detect whether the character has hit a hall, platform, or monster
void Convert_Tilt(CHARACTER_T * ch);									// Convert tilt to side to side movement speed
void Scroll_Camera(CHARACTER_T * ch);									// Scrolls camera down
