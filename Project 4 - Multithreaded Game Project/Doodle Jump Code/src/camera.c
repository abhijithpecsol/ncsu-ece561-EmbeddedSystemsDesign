/*
 * Camera.c
 *
 * This file will contain various helper functions for camera scrolling.
 * Header file contains appropriate defines.
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <RTL.h>
#include <MKL25Z4.h>
#include <stdlib.h>
#include "tasks.h"
#include "camera.h"

extern OS_MUT LCD_mutex;
extern COLOR_T c_char, c_plat, c_bg;
extern PLATFORM_FIFO_T platforms;

// Scrolls camera down
// TODO
void Scroll_Camera(CHARACTER_T * ch){	
	// scroll down character
	Scroll_Character(ch);
	
	// scroll and redrawn platforms
	Scroll_Platforms();
	
	// scroll monsters/bullets/etc. coming soon
	// TODO
	
	// generate new platforms/monsters/bullets/etc.
	// TODO
}

// Scrolls the character down
void Scroll_Character(CHARACTER_T * ch) {
	int16_t vx_temp = ch->Vx;
	ch->Vx = 0;											// just going straight down; direction of Vx is used to determine whether to erase above or below
	ch->Vy *= -1;										// when jumping, we are going up the screen, but want to scroll down the screen
	ch->loc.Y += CAM_SCROLL_SPEED;	// adjust position
	Redraw_Character(ch);
	ch->Vy *= -1;										// want to be going back up again
	ch->Vx = vx_temp;
}

// Scrolls platforms down
void Scroll_Platforms(void) {
	uint8_t i, index;
	index = platforms.first;
	for (i = 0; i < platforms.num; i++){
		// p = original position, pp = bottom right of new position 
		PT_T p = platforms.platforms[index].loc, pp;
		platforms.platforms[index].loc.Y += CAM_SCROLL_SPEED;
		pp.X = p.X + PLAT_WIDTH;		
		pp.Y = p.Y + CAM_SCROLL_SPEED + PLAT_HEIGHT;
		// if the platform would be off screen, just erase and delete the old platform
		if (pp.Y >= TFT_HEIGHT){
			platforms.platforms[index].loc.Y -= CAM_SCROLL_SPEED;
			Erase_Platform(&platforms.platforms[index], &c_bg);
			Delete_Platform(&platforms.platforms[index]);
		}
		// otherwise, erase above and redraw platform in new position
		else if (p.Y > 0) {
			os_mut_wait(&LCD_mutex, WAIT_FOREVER);
			TFT_Start_Rectangle(&p, &pp);
			TFT_Write_Rectangle_Pixel(&c_bg, ((PLAT_WIDTH+1)*(CAM_SCROLL_SPEED)));
			TFT_Write_Rectangle_Pixel(&c_plat, ((PLAT_WIDTH+1)*(PLAT_HEIGHT+1)));
			os_mut_release(&LCD_mutex);
		}
		index++;
		if (index >= MAX_NUM_PLATFORMS) {
			index = 0;
		}
	}
}
