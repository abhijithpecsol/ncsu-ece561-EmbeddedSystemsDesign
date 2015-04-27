/*
 * Camera.h
 *
 */
 
#ifndef camera_h
#define camera_h

#include "game.h"

// Camera scroller
#define SCROLL_DOWN_DIST_BOT	(50)		// how far from bottom to scroll down 
#define SCROLL_DOWN_LIMIT	(TFT_HEIGHT - SCROLL_DOWN_DIST_BOT)		// how far to scroll down relative to screen coordinates
#define CAM_SCROLL_SPEED	(3)					// speed of scrolling
#define SCROLL_THRESHOLD	(250)				// above this point, we will scroll down

// ****************
// HELPER FUNCTIONS
// ****************
void Scroll_Camera(CHARACTER_T * ch);				// Scrolls camera down
void Scroll_Character(CHARACTER_T * ch);		// Scrolls the character down
void Scroll_Platforms(void);								// Scrolls platforms down

#endif
