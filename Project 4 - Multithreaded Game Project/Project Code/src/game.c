/*
 * Game.c
 *
 * This file will contain various helper functions for the game processing.
 * Header file contains various structs and data structures as well.
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <RTL.h>
#include <MKL25Z4.h>
#include "tft_lcd.h"
#include "tasks.h"
#include "mma8451.h"
#include "sound.h"
#include "gpio_defs.h"
#include "game.h"

extern OS_MUT LCD_mutex;
COLOR_T c_char, c_plat, c_bg;
PLATFORM_FIFO_T platforms;

// Create initial game state
void Game_Init(CHARACTER_T * ch) {
	PLATFORM_T * start_plat;
	PLATFORM_T * sp;
	
	// platform color
	c_plat.R = 126;
	c_plat.G = 192;
	c_plat.B = 56;
	
	// mr. doodle color
	c_char.R = 150;
	c_char.G = 200;
	c_char.B = 255;
	
	// background color
	c_bg.R = 0;
	c_bg.G = 0;
	c_bg.B = 0;
	
	// initial position of character
	ch->loc.X = CHAR_START_POS_X;
	ch->loc.Y = CHAR_START_POS_Y;
	
	// initial position of start platform
	start_plat = Add_Platform();
	start_plat->loc.X = PLAT_START_POS_X;
	start_plat->loc.Y = PLAT_START_POS_Y;
	Draw_Platform(start_plat, &c_plat);
	
	//sp = Add_Platform();
	//sp->loc.X = PLAT_START_POS_X;
	//sp->loc.Y = PLAT_START_POS_Y - 100;		// 100 ~ max jump
	//Draw_Platform(sp, &c_plat);
}

// Draw character at it's existing position in the selected color
void Draw_Character(CHARACTER_T * ch, COLOR_T * c) {
	PT_T p, pp;
		
	// body
	p = ch->loc;
	pp.X = ch->loc.X + CHAR_BODY_WIDTH;
	pp.Y = ch->loc.Y + CHAR_BODY_HEIGHT;
	os_mut_wait(&LCD_mutex, WAIT_FOREVER);
	TFT_Fill_Rectangle(&p, &pp, c);
	os_mut_release(&LCD_mutex);
	
	// left leg
	p.X = p.X + CHAR_LEG_INSET;
	p.Y = p.Y + CHAR_BODY_HEIGHT;
	pp.X = p.X + CHAR_LEG_WIDTH;
	pp.Y = p.Y + CHAR_LEG_HEIGHT;
	os_mut_wait(&LCD_mutex, WAIT_FOREVER);
	TFT_Fill_Rectangle(&p, &pp, c);
	os_mut_release(&LCD_mutex);
	
	// right leg
	p.X = p.X - CHAR_LEG_INSET + CHAR_BODY_WIDTH - CHAR_LEG_INSET - CHAR_LEG_WIDTH;
	pp.X = p.X + CHAR_LEG_WIDTH;
	os_mut_wait(&LCD_mutex, WAIT_FOREVER);
	TFT_Fill_Rectangle(&p, &pp, c);
	os_mut_release(&LCD_mutex);
}

// Erase character in the selected background color
void Erase_Character(CHARACTER_T * ch, COLOR_T * bg){
	PT_T p, pp;
	p = ch->loc;
	pp.X = ch->loc.X + CHAR_BODY_WIDTH;
	pp.Y = ch->loc.Y + CHAR_BODY_HEIGHT + CHAR_LEG_HEIGHT;
	os_mut_wait(&LCD_mutex, WAIT_FOREVER);
	TFT_Fill_Rectangle(&p, &pp, bg);
	os_mut_release(&LCD_mutex);
}

// Erases and redraws the character fast, avoids flickering.
// TODO - MAX_CHAR_DISPLACEMENT can be replaced with velocity in each direction for better code
// TODO - only need to erase away from the direction of travel
void Redraw_Character(CHARACTER_T * ch){
	uint8_t i;
	PT_T p, pp;
	
	if (ch->Vy >= 0){
		p = ch->loc, pp = ch->loc;
		p.X -= MAX_CHAR_DISPLACEMENT;
		p.Y -= MAX_CHAR_DISPLACEMENT;
		pp.X += (CHAR_BODY_WIDTH + MAX_CHAR_DISPLACEMENT);
		pp.Y += CHAR_BODY_HEIGHT + CHAR_LEG_HEIGHT;
	}
	else {
		p = ch->loc, pp = ch->loc;
		p.X -= MAX_CHAR_DISPLACEMENT;
		pp.X += (CHAR_BODY_WIDTH + MAX_CHAR_DISPLACEMENT);
		pp.Y += (CHAR_BODY_HEIGHT + CHAR_LEG_HEIGHT + MAX_CHAR_DISPLACEMENT);
	}
	
	os_mut_wait(&LCD_mutex, WAIT_FOREVER);
	TFT_Start_Rectangle(&p, &pp);
	if (ch->Vy >= 0){
		TFT_Write_Rectangle_Pixel(&c_bg, (2*MAX_CHAR_DISPLACEMENT+CHAR_BODY_WIDTH+1)*MAX_CHAR_DISPLACEMENT);
	}
	for (i = 0; i < CHAR_BODY_HEIGHT; i++){
		TFT_Write_Rectangle_Pixel(&c_bg, MAX_CHAR_DISPLACEMENT + 1);
		TFT_Write_Rectangle_Pixel(&c_char, CHAR_BODY_WIDTH);
		TFT_Write_Rectangle_Pixel(&c_bg, MAX_CHAR_DISPLACEMENT);
	}
	for (i = 0; i < CHAR_LEG_HEIGHT; i++){
		TFT_Write_Rectangle_Pixel(&c_bg, MAX_CHAR_DISPLACEMENT + CHAR_LEG_INSET + 1);
		TFT_Write_Rectangle_Pixel(&c_char, CHAR_LEG_WIDTH);
		TFT_Write_Rectangle_Pixel(&c_bg, CHAR_LEG_GAP);
		TFT_Write_Rectangle_Pixel(&c_char, CHAR_LEG_WIDTH);
		TFT_Write_Rectangle_Pixel(&c_bg, MAX_CHAR_DISPLACEMENT + CHAR_LEG_INSET);
	}
	if (ch->Vy <= 0){
		TFT_Write_Rectangle_Pixel(&c_bg, (2*MAX_CHAR_DISPLACEMENT+CHAR_BODY_WIDTH+1)*MAX_CHAR_DISPLACEMENT);
	}
	os_mut_release(&LCD_mutex);
}

// Returns a pointer to a platform that may be created and displayed. If one cannot be created, returns null.
PLATFORM_T * Add_Platform(void) {
	PLATFORM_T * pt;
	if (platforms.num < MAX_NUM_PLATFORMS){
		pt = &(platforms.platforms[platforms.next]);
		platforms.next++;
		if (platforms.next >= MAX_NUM_PLATFORMS){
			platforms.next = 0;
		}
		platforms.num++;
	}
	return pt;
}

// Draw platform in the selected color
void Draw_Platform(PLATFORM_T * p, COLOR_T * c) {
	PT_T pp;
	pp.X = p->loc.X + PLAT_WIDTH;
	pp.Y = p->loc.Y + PLAT_HEIGHT;
	os_mut_wait(&LCD_mutex, WAIT_FOREVER);
	TFT_Fill_Rectangle(&p->loc, &pp, c);
	os_mut_release(&LCD_mutex);
}

// Erase platform with selected background color
void Erase_Platform(PLATFORM_T * p, COLOR_T * bg) {
	Draw_Platform(p, bg);
}

// Redraw all platforms
void Redraw_Platforms(void) {
	uint8_t i = 0;
	uint8_t index = index = platforms.first;
	for (i = 0; i < platforms.num; i++){
		PT_T p = platforms.platforms[index].loc, pp;
		pp.X = p.X + PLAT_WIDTH;
		pp.Y = p.Y + PLAT_HEIGHT;
		//Draw_Platform(&platforms.platforms[index], &c_plat);
		os_mut_wait(&LCD_mutex, WAIT_FOREVER);
		TFT_Start_Rectangle(&p, &pp);
		TFT_Write_Rectangle_Pixel(&c_plat, ((PLAT_WIDTH+1)*(PLAT_HEIGHT+1)));
		os_mut_release(&LCD_mutex);
		index++;
		if (index >= MAX_NUM_PLATFORMS){
			index = 0;
		}
	}
}

// Move character based on velocity and adjust velocity
void Move_Character(CHARACTER_T * ch) {
	// adjust position
	ch->loc.X += ch->Vx;
	ch->loc.Y += ch->Vy;
	// modify vertical velocity due to gravity
	ch->Vy += ACCEL_GRAVITY;
	// redraw character
	Redraw_Character(ch);
}

// Detect whether the character has hit a hall, platform, or monster
void Detect_Collision(CHARACTER_T * ch) {
	uint8_t i = 0;
	uint8_t index = 0;
	uint8_t left_of_ch = ch->loc.X;
	uint8_t right_of_ch = ch->loc.X + CHAR_BODY_WIDTH;
	uint8_t bot_of_ch = ch->loc.Y + (CHAR_BODY_HEIGHT + CHAR_LEG_HEIGHT);
	
	// collision with platforms, only downward
	if (ch->Vy >= 0) {
		index = platforms.first;
		for (i = 0; i < platforms.num; i++){
			uint8_t left_of_platform = platforms.platforms[index].loc.X;
			uint8_t top_of_platform = platforms.platforms[index].loc.Y;
			if (left_of_ch >= left_of_platform && right_of_ch <= left_of_platform + PLAT_WIDTH){
				if (bot_of_ch >= top_of_platform && bot_of_ch <= top_of_platform + PLAT_HEIGHT) {
					ch->Vy = JUMP_VELOCITY;
					break;		// only need one collision
				}
			}
			index++;
			if (index >= MAX_NUM_PLATFORMS){
				index = 0;
			}
		}
	}
}
