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
#include <Math.h>
#include "tft_lcd.h"
#include "tasks.h"
#include "mma8451.h"
#include "sound.h"
#include "gpio_defs.h"
#include "game.h"
#include <stdlib.h>
#include <time.h>

extern OS_MUT LCD_mutex, game_obj_mutex;
extern OS_TID t_Scroll_Cam;
extern OS_MBX roll_mbx, plat_mbx;
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
	
	// test second platform
	sp = Add_Platform();
	sp->loc.X = PLAT_START_POS_X-(PLAT_START_POS_X/2)-20;	// offset left
	sp->loc.Y = PLAT_START_POS_Y - 80;		// 100 ~ max jump
			
	// generate initial platforms
	Generate_Platform();
	Generate_Platform();
	Generate_Platform();
	
	// draw platforms
	Redraw_Platforms();
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
void Redraw_Character(CHARACTER_T * ch){
	uint8_t i;
	PT_T p, pp;
	
	int16_t x_dis = ch->Vx; 
	if (x_dis > 0 && ch->loc.X < x_dis){
		x_dis = ch->loc.X;
	}
	//else if (x_dis < 0 && abs(x_dis) > TFT_WIDTH - 1 - ch->loc.X){
	//	x_dis = (TFT_WIDTH - 1 - ch->loc.X);
	//}
	
	// Determine bounding box
	// Moving down the screen
	if (ch->Vy >= 0){
		p = ch->loc, pp = ch->loc;
		// only need to erase above us
		p.Y -= TERMINAL_VELOCITY;
		pp.Y += CHAR_BODY_HEIGHT + CHAR_LEG_HEIGHT;
	}
	// Moving up the screen
	else {
		p = ch->loc, pp = ch->loc;
		// only need to below above us
		pp.Y += (CHAR_BODY_HEIGHT + CHAR_LEG_HEIGHT + MAX_CHAR_DISPLACEMENT);
	}
	
	// moving right on the screen
	// first handle cases on the edge of the screen
	if (x_dis == 0){
		if (ch->loc.X == 0){
			x_dis = -1*MAX_CHAR_DISPLACEMENT;
		}
		else {
			x_dis = MAX_CHAR_DISPLACEMENT;
		}
	}
	if (x_dis >= 0){
		// only need to erase to the left
		p.X -= x_dis;
		pp.X += CHAR_BODY_WIDTH;
	}
	// moving left on the screen
	else {
		// only need to erase to the right
		pp.X += CHAR_BODY_WIDTH - x_dis;
	}
	
	// Do the actual drawing
	os_mut_wait(&LCD_mutex, WAIT_FOREVER);
	TFT_Start_Rectangle(&p, &pp);
	// Moving right across the screen
	if (x_dis >= 0){
		if (ch->Vy >= 0){
			TFT_Write_Rectangle_Pixel(&c_bg, (x_dis+CHAR_BODY_WIDTH+1)*TERMINAL_VELOCITY);
		}
		for (i = 0; i < CHAR_BODY_HEIGHT; i++){
			TFT_Write_Rectangle_Pixel(&c_bg, x_dis + 1);
			TFT_Write_Rectangle_Pixel(&c_char, CHAR_BODY_WIDTH);
			//TFT_Write_Rectangle_Pixel(&c_bg, MAX_CHAR_DISPLACEMENT);
		}
		for (i = 0; i < CHAR_LEG_HEIGHT; i++){
			TFT_Write_Rectangle_Pixel(&c_bg, x_dis + CHAR_LEG_INSET + 1);
			TFT_Write_Rectangle_Pixel(&c_char, CHAR_LEG_WIDTH);
			TFT_Write_Rectangle_Pixel(&c_bg, CHAR_LEG_GAP);
			TFT_Write_Rectangle_Pixel(&c_char, CHAR_LEG_WIDTH);
			//TFT_Write_Rectangle_Pixel(&c_bg, MAX_CHAR_DISPLACEMENT + CHAR_LEG_INSET);
			TFT_Write_Rectangle_Pixel(&c_bg, CHAR_LEG_INSET);
		}
		if (ch->Vy <= 0){
			TFT_Write_Rectangle_Pixel(&c_bg, (x_dis+CHAR_BODY_WIDTH+1)*MAX_CHAR_DISPLACEMENT);
		}
	}
	// Moving left across the screen
	else {
		if (ch->Vy >= 0){
			TFT_Write_Rectangle_Pixel(&c_bg, (-1*x_dis+CHAR_BODY_WIDTH+1)*TERMINAL_VELOCITY);
		}
		for (i = 0; i < CHAR_BODY_HEIGHT; i++){
			//TFT_Write_Rectangle_Pixel(&c_bg, MAX_CHAR_DISPLACEMENT + 1);
			TFT_Write_Rectangle_Pixel(&c_bg, 1);
			TFT_Write_Rectangle_Pixel(&c_char, CHAR_BODY_WIDTH);
			TFT_Write_Rectangle_Pixel(&c_bg, -1*x_dis);
		}
		for (i = 0; i < CHAR_LEG_HEIGHT; i++){
			//TFT_Write_Rectangle_Pixel(&c_bg, MAX_CHAR_DISPLACEMENT + CHAR_LEG_INSET + 1);
			TFT_Write_Rectangle_Pixel(&c_bg, CHAR_LEG_INSET + 1);
			TFT_Write_Rectangle_Pixel(&c_char, CHAR_LEG_WIDTH);
			TFT_Write_Rectangle_Pixel(&c_bg, CHAR_LEG_GAP);
			TFT_Write_Rectangle_Pixel(&c_char, CHAR_LEG_WIDTH);
			TFT_Write_Rectangle_Pixel(&c_bg, -1*x_dis + CHAR_LEG_INSET);
		}
		if (ch->Vy <= 0){
			TFT_Write_Rectangle_Pixel(&c_bg, (-1*x_dis+CHAR_BODY_WIDTH+1)*MAX_CHAR_DISPLACEMENT);
		}
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

// Removes a platform from existence. Being a FIFO, this removes every platform created before this platform.
void Delete_Platform(PLATFORM_T * p) {
	uint8_t index = platforms.first;
	do {
		platforms.num--;
		platforms.first++;
		if (platforms.first >= MAX_NUM_PLATFORMS) {
			platforms.first = 0;
		}
		if (index >= MAX_NUM_PLATFORMS) {
			index = 0;
		}
	}
	while (&platforms.platforms[index++] != p && platforms.num > 0);
}

// Generates a new platform
void Generate_Platform(void) {
	PLATFORM_T * new_platform;
	float pitch_val = pitch;
	
	// Create new platform
	new_platform = Add_Platform();
	
	if (new_platform != NULL) {
		
		// determine distance above highest platform
		uint8_t r = rand() % (MAX_JUMP_HEIGHT - MIN_HEIGHT_DIFFERENCE);
		uint8_t dist = r + MIN_HEIGHT_DIFFERENCE;
		
		// determine x position
		uint8_t x_pos = (rand() % (TFT_WIDTH - PLAT_WIDTH - 2*MIN_SIDE_THRESHOLD)) + MIN_SIDE_THRESHOLD;
		
		// find highest platform location
		uint8_t i, index = platforms.first;
		int32_t highest_val = TFT_HEIGHT;
		for (i = 0; i < platforms.num - 1; i++) {
			int32_t py = platforms.platforms[index].loc.Y;
			if (py < highest_val) {
				highest_val = py;
			}
			index++;
			if (index >= MAX_NUM_PLATFORMS){
				index = 0;
			}
		}
		
		// set new platform location
		new_platform->loc.X = x_pos;
		new_platform->loc.Y = highest_val - dist;
	}
}

// Draw platform in the selected color
void Draw_Platform(PLATFORM_T * p, COLOR_T * c) {
	if (p->loc.Y >= 0) {
		PT_T pp;
		pp.X = p->loc.X + PLAT_WIDTH;
		pp.Y = p->loc.Y + PLAT_HEIGHT;
		os_mut_wait(&LCD_mutex, WAIT_FOREVER);
		TFT_Fill_Rectangle(&p->loc, &pp, c);
		os_mut_release(&LCD_mutex);
	}
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
		if (p.Y >= 0) {
			pp.X = p.X + PLAT_WIDTH;
			pp.Y = p.Y + PLAT_HEIGHT;
			os_mut_wait(&LCD_mutex, WAIT_FOREVER);
			TFT_Start_Rectangle(&p, &pp);
			TFT_Write_Rectangle_Pixel(&c_plat, ((PLAT_WIDTH+1)*(PLAT_HEIGHT+1)));
			os_mut_release(&LCD_mutex);
		}
		index++;
		if (index >= MAX_NUM_PLATFORMS){
			index = 0;
		}
	}
}

// Move character based on velocity and adjust velocity
void Move_Character(CHARACTER_T * ch) {
	// if new roll is available, set Vy based on it
	Convert_Tilt(ch);
	// adjust position
	ch->loc.X += ch->Vx;
	ch->loc.Y += ch->Vy;
	// modify vertical velocity due to gravity
	if (ch->Vy < TERMINAL_VELOCITY){
		ch->Vy += ACCEL_GRAVITY;
	}
	else {
		ch->Vy = TERMINAL_VELOCITY;
	}
	// redraw character
	Redraw_Character(ch);
}

// Detect whether the character has hit a hall, platform, or monster
uint8_t Detect_Collision(CHARACTER_T * ch) {
	uint8_t i = 0;
	uint16_t index = 0;
	int16_t left_of_ch = ch->loc.X;
	int16_t right_of_ch = ch->loc.X + CHAR_BODY_WIDTH;
	int16_t bot_of_ch = ch->loc.Y + (CHAR_BODY_HEIGHT + CHAR_LEG_HEIGHT);
	
	// bottom of screen
	if (bot_of_ch >= TFT_HEIGHT - ch->Vy){		// Mr. Doodle will hit bottom in the next frame
		// TODO - game over
		// for now, just bounce up
		ch->Vy = JUMP_VELOCITY;
	}
	
	// side of the screen, switch Mr. Doodle's side
	#if 0
	if (left_of_ch + ch->Vx <= 0){
		Erase_Character(ch, &c_bg);		// need to erase first
		ch->loc.X = TFT_WIDTH - CHAR_BODY_WIDTH - MAX_CHAR_DISPLACEMENT;
	}
	else if (right_of_ch + ch->Vx >= TFT_WIDTH){
		Erase_Character(ch, &c_bg);		// need to erase first
		ch->loc.X = MAX_CHAR_DISPLACEMENT;
	}
	#else
	// alternative, stop Mr. Doodle dead in his tracks
	if (left_of_ch + ch->Vx <= 0){
		ch->Vx = 0;
		ch->loc.X = 0;			// lock to left wall
	}
	else if (right_of_ch + ch->Vx >= TFT_WIDTH - 1){
		ch->Vx = 0;
		ch->loc.X = TFT_WIDTH - CHAR_BODY_WIDTH - 1;		// lock to right wall
	}
	#endif
	
	// collision with platforms, only downward
	if (ch->Vy >= 0) {
		index = platforms.first;
		for (i = 0; i < platforms.num; i++){
			uint16_t left_of_platform = platforms.platforms[index].loc.X;
			uint16_t top_of_platform = platforms.platforms[index].loc.Y;
			if (left_of_ch <= left_of_platform + PLAT_WIDTH && right_of_ch >= left_of_platform ){
				if (bot_of_ch >= top_of_platform && bot_of_ch <= top_of_platform + PLAT_HEIGHT) {
					//if (os_mbx_check(plat_mbx) > 0){	// avoid double triggering which causes freezing
						// get location of platform landed on and put it in mailbox
						//uint32_t * msg = malloc(sizeof(uint32_t));
						//*msg = top_of_platform;
						//os_mbx_send(plat_mbx, (void *)msg, WAIT_FOREVER);
						
						// trigger event
						//os_evt_set(EV_SCROLLCAM, t_Scroll_Cam);
					//}
					// reset jump velocity of character
					ch->Vy = JUMP_VELOCITY;
					
					return HIT_PLATFORM;		// only need one collision
				}
			}
			index++;
			if (index >= MAX_NUM_PLATFORMS){
				index = 0;
			}
		}
	}
	
	return NO_HIT;
}

// Convert tilt to side to side movement speed
void Convert_Tilt(CHARACTER_T * ch) {
	float roll_val, abs_roll_val;
	void * msg;
	
	// get current roll value from mailbox, if it's there
	if (os_mbx_check(roll_mbx) == 0){
		os_mbx_wait(roll_mbx, &msg, WAIT_FOREVER);
		roll_val = *(float *)msg;
		free(msg);
	}
	else {
		return;
	}
	
	// do conversion
	abs_roll_val = fabs(roll_val);
	if ((ch->loc.X == 0 && roll_val < 0) || (ch->loc.X >= (TFT_WIDTH - CHAR_BODY_WIDTH - 1) && roll_val > 0)){
		ch->Vx = 0;
	}
	else {
		if (abs_roll_val < 5){
			ch->Vx = 0;
		}
		else if (abs_roll_val > 5 && abs_roll_val < 15){
			if (roll_val < 0){
				ch->Vx = -4;
			}
			else {
				ch->Vx = 4;
			}
		}
		else if (abs_roll_val > 15 && abs_roll_val < 30){
			if (roll_val < 0){
				ch->Vx = -8;
			}
			else {
				ch->Vx = 8;
			}
		}
		else if (abs_roll_val > 30){
			if (roll_val < 0){
				ch->Vx = -12;
			}
			else {
				ch->Vx = 12;
			}
		}
	}
}

