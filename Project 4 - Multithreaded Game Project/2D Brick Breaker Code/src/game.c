#include <stdint.h>
#include <stdio.h>
#include <RTL.h>
#include <MKL25Z4.h>
#include <stdlib.h>
#include <math.h>
#include "TFT_LCD.h"
#include "font.h"
#include "tasks.h"
#include "MMA8451.h"
#include "sound.h"
#include "DMA.h"
#include "gpio_defs.h"
#include "game.h"
#include "utilization.h"

extern OS_MUT LCD_mutex;
extern OS_MBX tilt_mbx;
extern OS_TID t_Sound_Manager;

// Sets up the initial game state
void Game_Init(GAME_T * g) {
	uint8_t i, j;
	
	// set up colors
	g->colors.c_bg.R = 0;
	g->colors.c_bg.G = 0;
	g->colors.c_bg.B = 0;
	g->colors.c_pad.R = 255;
	g->colors.c_pad.G = 255;
	g->colors.c_pad.B = 255;
	g->colors.c_ball.R = 218;
	g->colors.c_ball.G = 165;
	g->colors.c_ball.B = 32;
	g->colors.c_brick_v_weak.R = 25;
	g->colors.c_brick_v_weak.G = 25;
	g->colors.c_brick_v_weak.B = 25;
	g->colors.c_brick_weak.R = 75;
	g->colors.c_brick_weak.G = 75;
	g->colors.c_brick_weak.B = 75;
	g->colors.c_brick_strong.R = 255;
	g->colors.c_brick_strong.G = 255;
	g->colors.c_brick_strong.B = 255;
	g->colors.c_brick_v_strong.R = 218;
	g->colors.c_brick_v_strong.G = 165;
	g->colors.c_brick_v_strong.B = 32;
	g->colors.c_brick_invulnerable.R = 65;
	g->colors.c_brick_invulnerable.G = 105;
	g->colors.c_brick_invulnerable.B = 225;
	
	// set positions of paddles
	g->paddles.bp.loc.X = (TFT_WIDTH - HORZ_PADDLE_WIDTH)/2;
	g->paddles.bp.loc.Y = TFT_HEIGHT - HORZ_PADDLE_HEIGHT - PADDLE_BUFFER;
	g->paddles.tp.loc.X = (TFT_WIDTH - HORZ_PADDLE_WIDTH)/2;
	g->paddles.tp.loc.Y = INFO_BAR_HEIGHT + PADDLE_BUFFER;
	g->paddles.lp.loc.X = PADDLE_BUFFER;
	g->paddles.lp.loc.Y = (TFT_HEIGHT - INFO_BAR_HEIGHT - VERT_PADDLE_HEIGHT)/2 + INFO_BAR_HEIGHT;
	g->paddles.rp.loc.X = TFT_WIDTH - VERT_PADDLE_WIDTH - PADDLE_BUFFER - 1;
	g->paddles.rp.loc.Y = (TFT_HEIGHT - INFO_BAR_HEIGHT - VERT_PADDLE_HEIGHT)/2 + INFO_BAR_HEIGHT;
	
	// draw paddles
	Redraw_Paddles(g);
	
	// set position and velocities of ball
	g->ball.loc.X = (TFT_WIDTH - BALL_SIDE_LENGTH)/2;
	g->ball.loc.Y = TFT_HEIGHT - PADDLE_BUFFER - HORZ_PADDLE_HEIGHT - BALL_SIDE_LENGTH - 2*PADDLE_BUFFER;
	g->ball.vx = 0;
	g->ball.vy = 0;
	
	// draw ball
	Redraw_Ball(g);
	
	// create bricks and set healths
	for (i = 0; i < NUM_BRICK_ROWS; i++){
		int16_t by = BRICK_START_Y + i*(BRICK_GAP + BRICK_SIDE_LENGTH);
		int16_t bx = BRICK_START_X;
		for (j = 0; j < NUM_BRICK_COLS; j++){
			BRICK_T * brick = &g->bricks[i][j];
			if (j > 0)	bx += BRICK_GAP + BRICK_SIDE_LENGTH;
			brick->loc.X = bx;
			brick->loc.Y = by;
			if ((j == 2 || j == 3) && (i == 4 || i == 5)){
				// very strong bricks in middle
				brick->health = BRICK_V_STRONG;
				Draw_Brick(brick, &g->colors.c_brick_v_strong);
			}
			else if ((j == 2 || j == 3) && (i == 2 || i == 3 || i == 6 || i == 7)) {
				// strong bricks around middle
				brick->health = BRICK_STRONG;
				Draw_Brick(brick, &g->colors.c_brick_strong);
			}
			else if (((j == 1 || j == 4) && (i >= 2 && i <= 7)) || ((j >= 1 && j <= 4) && (i == 1 || i == 8))){
				// weak bricks around strong bricks
				brick->health = BRICK_WEAK;
				Draw_Brick(brick, &g->colors.c_brick_weak);
			}
			else {
				// all others are very weak bricks
				brick->health = BRICK_V_WEAK;
				Draw_Brick(brick, &g->colors.c_brick_v_weak);
			}
			brick->protection = 0;
		}
	}
	
	// set initial life count
	g->lives = GAME_INIT_LIVES;
	
	// reset brick count
	g->hits = 0;
}

// Redraw, including movement, the vertical paddles
void Redraw_Vertical_Paddles(GAME_T * g){
	PT_T p, pp;
	PADDLE_T * lp = &g->paddles.lp, * rp = &g->paddles.rp;
	uint8_t i;
	int8_t vel;
	
	// do this for both paddles
	for (i = 0; i < 2; i++){
		PADDLE_T * pad;
		if (i == 0) pad = lp;
		else pad = rp;
		
		// will always redraw the paddle
		p = pad->loc;
		pp.X = pad->loc.X + VERT_PADDLE_WIDTH;
		pp.Y = pad->loc.Y + VERT_PADDLE_HEIGHT;
		os_mut_wait(&LCD_mutex, WAIT_FOREVER);
		TFT_Fill_Rectangle(&p, &pp, &g->colors.c_pad);
		os_mut_release(&LCD_mutex);
			
		// if moving up the screen, erase below us
		vel = pad->v;
		if (vel < 0){
			p.Y += VERT_PADDLE_HEIGHT + 1;
			pp.Y -= vel;
			os_mut_wait(&LCD_mutex, WAIT_FOREVER);
			TFT_Fill_Rectangle(&p, &pp, &g->colors.c_bg);
			os_mut_release(&LCD_mutex);
		}
		// if moving down the screen, erase above us
		else if (vel > 0) {
			p.Y -= vel;	
			pp.Y -= VERT_PADDLE_HEIGHT + 1;
			os_mut_wait(&LCD_mutex, WAIT_FOREVER);
			TFT_Fill_Rectangle(&p, &pp, &g->colors.c_bg);
			os_mut_release(&LCD_mutex);
		}
	}
}

// Redraw, including movement, the horizontal paddles
void Redraw_Horizontal_Paddles(GAME_T * g){
	PT_T p, pp;
	PADDLE_T * tp = &g->paddles.tp, * bp = &g->paddles.bp;
	uint8_t i;
	int8_t vel;
	
	// do this for both paddles
	for (i = 0; i < 2; i++){
		PADDLE_T * pad;
		if (i == 0) pad = tp;
		else pad = bp;
				
		// will always redraw the paddle
		p = pad->loc;
		pp.X = pad->loc.X + HORZ_PADDLE_WIDTH;
		pp.Y = pad->loc.Y + HORZ_PADDLE_HEIGHT;
		os_mut_wait(&LCD_mutex, WAIT_FOREVER);
		TFT_Fill_Rectangle(&p, &pp, &g->colors.c_pad);
		os_mut_release(&LCD_mutex);
			
		// if moving left on the screen, erase to our right
		vel = pad->v;
		if (vel < 0){
			p.X += HORZ_PADDLE_WIDTH + 1;
			pp.X -= vel;
			os_mut_wait(&LCD_mutex, WAIT_FOREVER);
			TFT_Fill_Rectangle(&p, &pp, &g->colors.c_bg);
			os_mut_release(&LCD_mutex);
		}
		// if moving right on the screen, erase to our left
		else if (vel > 0) {
			p.X -= vel;
			pp.X -= HORZ_PADDLE_WIDTH + 1;
			os_mut_wait(&LCD_mutex, WAIT_FOREVER);
			TFT_Fill_Rectangle(&p, &pp, &g->colors.c_bg);
			os_mut_release(&LCD_mutex);
		}
	}
}

// Redraws, including movement, of both horizontal and vertical paddles
void Redraw_Paddles(GAME_T * g){
	Redraw_Horizontal_Paddles(g);
	Redraw_Vertical_Paddles(g);
}

// Adjusts the positions of the paddles
void Move_Paddles(GAME_T * g) {
	g->paddles.bp.loc.X += g->paddles.bp.v;
	g->paddles.tp.loc.X += g->paddles.tp.v;
	g->paddles.lp.loc.Y += g->paddles.lp.v;
	g->paddles.rp.loc.Y += g->paddles.rp.v;
	
	// horizontal wall collisions
	if (g->paddles.bp.loc.X < PADDLE_BUFFER){
		g->paddles.bp.loc.X = PADDLE_BUFFER;
		g->paddles.tp.loc.X = PADDLE_BUFFER;
	}
	else if (g->paddles.bp.loc.X + HORZ_PADDLE_WIDTH > TFT_WIDTH - 1 - PADDLE_BUFFER){
		g->paddles.bp.loc.X = TFT_WIDTH - 1 - PADDLE_BUFFER - HORZ_PADDLE_WIDTH;
		g->paddles.tp.loc.X = TFT_WIDTH - 1 - PADDLE_BUFFER - HORZ_PADDLE_WIDTH;
	}
	
	// vertical wall collisions
	if (g->paddles.lp.loc.Y < INFO_BAR_HEIGHT + PADDLE_BUFFER){
		g->paddles.lp.loc.Y = INFO_BAR_HEIGHT + PADDLE_BUFFER;
		g->paddles.rp.loc.Y = INFO_BAR_HEIGHT + PADDLE_BUFFER;
	}
	else if (g->paddles.lp.loc.Y + VERT_PADDLE_HEIGHT > TFT_HEIGHT - 1 - PADDLE_BUFFER){
		g->paddles.lp.loc.Y = TFT_HEIGHT - 1 - PADDLE_BUFFER - VERT_PADDLE_HEIGHT;
		g->paddles.rp.loc.Y = TFT_HEIGHT - 1 - PADDLE_BUFFER - VERT_PADDLE_HEIGHT;
	}
}

// Convert pitch and roll to velocities of the paddles
void Convert_Tilt(GAME_T * g) {
	static float calibration = 30.0;
	static uint8_t calibration_ticks = 5;
	float roll_val, pitch_val;
	float abs_roll_val, abs_pitch_val;
	void * msg;
		
	// get values from mailbox, if they are there
	if (os_mbx_check(tilt_mbx) == 0){
		// get roll
		os_mbx_wait(tilt_mbx, &msg, WAIT_FOREVER);
		roll_val = *(float *)msg;
		free(msg);
		
		// get pitch
		os_mbx_wait(tilt_mbx, &msg, WAIT_FOREVER);
		pitch_val = *(float *)msg;
		free(msg);
	}
	else {
		return;
	}
		
	// convert roll
	abs_roll_val = fabs(roll_val);
	if ((g->paddles.bp.loc.X <= PADDLE_BUFFER && roll_val < 0) || (g->paddles.bp.loc.X >= (TFT_WIDTH - HORZ_PADDLE_WIDTH - 1) && roll_val > 0)){
		g->paddles.bp.v = 0;
		g->paddles.tp.v = 0;
	}
	else {
		if (abs_roll_val < 5){
			g->paddles.bp.v = 0;
			g->paddles.tp.v = 0;
		}
		else if (abs_roll_val > 5 && abs_roll_val < 15){
			if (roll_val < 0){
				g->paddles.bp.v = -4;
				g->paddles.tp.v = -4;
			}
			else {
				g->paddles.bp.v = 4;
				g->paddles.tp.v = 4;
			}
		}
		else if (abs_roll_val > 15 && abs_roll_val < 30){
			if (roll_val < 0){
				g->paddles.bp.v = -8;
				g->paddles.tp.v = -8;
			}
			else {
				g->paddles.bp.v = 8;
				g->paddles.tp.v = 8;
			}
		}
		else if (abs_roll_val > 30){
			if (roll_val < 0){
				g->paddles.bp.v = -12;
				g->paddles.tp.v = -12;
			}
			else {
				g->paddles.bp.v = 12;
				g->paddles.tp.v = 12;
			}
		}
	}
	
	// calibrate ticks with first 5 accelerometer readings
	if (calibration_ticks > 0){
		calibration *= (float)(6 - calibration_ticks);
		calibration += pitch_val;
		calibration_ticks--;
		calibration /= (float)(6 - calibration_ticks);
	}
	pitch_val = pitch_val - calibration;
	abs_pitch_val = fabs(pitch_val);
	if ((g->paddles.lp.loc.Y <= INFO_BAR_HEIGHT + PADDLE_BUFFER && pitch_val < 0) || (g->paddles.lp.loc.Y >= (TFT_HEIGHT - VERT_PADDLE_HEIGHT - 1) && pitch_val > 0)){
		g->paddles.lp.v = 0;
		g->paddles.rp.v = 0;
	}
	else {
		if (abs_pitch_val < 5){
			g->paddles.lp.v = 0;
			g->paddles.rp.v = 0;
		}
		else if (abs_pitch_val > 5 && abs_pitch_val < 15){
			if (pitch_val < 0){
				g->paddles.lp.v = -5;
				g->paddles.rp.v = -5;
			}
			else {
				g->paddles.lp.v = 5;
				g->paddles.rp.v = 5;
			}
		}
		else if (abs_pitch_val > 15 && abs_pitch_val < 30){
			if (pitch_val < 0){
				g->paddles.lp.v = -10;
				g->paddles.rp.v = -10;
			}
			else {
				g->paddles.lp.v = 10;
				g->paddles.rp.v = 10;
			}
		}
		else if (abs_pitch_val > 30){
			if (pitch_val < 0){
				g->paddles.lp.v = -15;
				g->paddles.rp.v = -15;
			}
			else {
				g->paddles.lp.v = 15;
				g->paddles.rp.v = 15;
			}
		}
	}
}

// Redraw, including movement, the ball
void Redraw_Ball(GAME_T * g){
	PT_T p, pp;
	BALL_T * ball = &g->ball;
	int8_t vel_y, vel_x;
	
	// will always redraw the ball
	p = ball->loc;
	pp.X = ball->loc.X + BALL_SIDE_LENGTH;
	pp.Y = ball->loc.Y + BALL_SIDE_LENGTH;
	
	os_mut_wait(&LCD_mutex, WAIT_FOREVER);
	TFT_Fill_Rectangle(&p, &pp, &g->colors.c_ball);
	os_mut_release(&LCD_mutex);
	
	// if moving up the screen, erase below us
	vel_y = ball->vy;
	if (vel_y < 0){
		p.Y += BALL_SIDE_LENGTH + 1;
		pp.Y -= vel_y;
		os_mut_wait(&LCD_mutex, WAIT_FOREVER);
		TFT_Fill_Rectangle(&p, &pp, &g->colors.c_bg);
		os_mut_release(&LCD_mutex);
	}
	// if moving down the screen, erase above us
	else if (vel_y > 0) {
		p.Y -= vel_y;	
		pp.Y -= BALL_SIDE_LENGTH + 1;
		os_mut_wait(&LCD_mutex, WAIT_FOREVER);
		TFT_Fill_Rectangle(&p, &pp, &g->colors.c_bg);
		os_mut_release(&LCD_mutex);
	}
	// if moving left on the screen, erase to our right
	p = ball->loc;
	pp.X = ball->loc.X + BALL_SIDE_LENGTH;
	pp.Y = ball->loc.Y + BALL_SIDE_LENGTH;
	vel_x = ball->vx;
	if (vel_x < 0){
		p.X += BALL_SIDE_LENGTH + 1;
		pp.X -= vel_x;
		if (vel_y < 0){
			pp.Y -= vel_y;
		}
		else {
			p.Y -= vel_y;
		}
		os_mut_wait(&LCD_mutex, WAIT_FOREVER);
		TFT_Fill_Rectangle(&p, &pp, &g->colors.c_bg);
		os_mut_release(&LCD_mutex);
	}
	// if moving right on the screen, erase to our left
	else if (vel_x > 0) {
		p.X -= vel_x;
		pp.X -= BALL_SIDE_LENGTH + 1;
		if (vel_y < 0){
			pp.Y -= vel_y;
		}
		else {
			p.Y -= vel_y;
		}
		os_mut_wait(&LCD_mutex, WAIT_FOREVER);
		TFT_Fill_Rectangle(&p, &pp, &g->colors.c_bg);
		os_mut_release(&LCD_mutex);
	}
}

// Adjust the position of the ball
void Move_Ball(GAME_T * g){
	g->ball.loc.X += g->ball.vx;
	g->ball.loc.Y += g->ball.vy;
}

// Detect the ball making a collision with a paddle
void Detect_Paddle_Collision(GAME_T * g){
	PT_T center_ball;
	PADDLE_T * pos;
	int16_t center_paddle;
	
	// determine center of ball
	center_ball.X = g->ball.loc.X + (BALL_SIDE_LENGTH/2);
	center_ball.Y = g->ball.loc.Y + (BALL_SIDE_LENGTH/2);
	
	// narrow down which paddle we could be making contact with, if at all, to reduce calculations
	// top paddle possible
	if (center_ball.Y < (TFT_HEIGHT/3)){
		int16_t top_ball = center_ball.Y - (BALL_SIDE_LENGTH/2);
		pos = &g->paddles.tp;
		if (g->ball.vy < 0 && top_ball <= pos->loc.Y + HORZ_PADDLE_HEIGHT && center_ball.X >= pos->loc.X && center_ball.X <= pos->loc.X + HORZ_PADDLE_WIDTH) {
			center_paddle = pos->loc.X + (HORZ_PADDLE_WIDTH/2);
			Calc_Bounce_Velocities(&g->ball, center_ball.X, center_paddle, TOP_PADDLE);
			return;
		}
	}
	// bottom paddle possible
	else if (center_ball.Y > (2*TFT_HEIGHT/3)){
		int16_t bottom_ball = center_ball.Y + (BALL_SIDE_LENGTH/2);
		pos = &g->paddles.bp;
		if (g->ball.vy > 0 && bottom_ball >= pos->loc.Y && center_ball.X >= pos->loc.X && center_ball.X <= pos->loc.X + HORZ_PADDLE_WIDTH) {
			center_paddle = pos->loc.X + (HORZ_PADDLE_WIDTH/2);
			Calc_Bounce_Velocities(&g->ball, center_ball.X, center_paddle, BOTTOM_PADDLE);
			return;
		}
	}
	// left paddle possible
	if (center_ball.X < (TFT_WIDTH/3)){
		int16_t left_ball = center_ball.X - (BALL_SIDE_LENGTH/2);
		pos = &g->paddles.lp;
		if (g->ball.vx < 0 && left_ball <= pos->loc.X + VERT_PADDLE_WIDTH && center_ball.Y >= pos->loc.Y && center_ball.Y <= pos->loc.Y + VERT_PADDLE_HEIGHT) {
			center_paddle = pos->loc.Y + (VERT_PADDLE_HEIGHT/2);
			Calc_Bounce_Velocities(&g->ball, center_ball.Y, center_paddle, LEFT_PADDLE);
			return;
		}
	}
	// right paddle possible
	else if (center_ball.X > (2*TFT_WIDTH/3)){
		int16_t right_ball = center_ball.X + (BALL_SIDE_LENGTH/2);
		pos = &g->paddles.rp;
		if (g->ball.vx > 0 && right_ball >= pos->loc.X && center_ball.Y >= pos->loc.Y && center_ball.Y <= pos->loc.Y + VERT_PADDLE_HEIGHT) {
			center_paddle = pos->loc.Y + (VERT_PADDLE_HEIGHT/2);
			Calc_Bounce_Velocities(&g->ball, center_ball.Y, center_paddle, RIGHT_PADDLE);
			return;
		}
	}
}

// Calculate the new velocities of a ball after contacting the paddle depending on where it hits
void Calc_Bounce_Velocities(BALL_T * b, int16_t cb, int16_t cp, uint8_t paddle){
	int8_t parallel_vel;
	int8_t perpen_vel;
	int16_t normalized_dist;
	
	// trigger sound
	os_evt_set(EV_PLAYSOUND, t_Sound_Manager);
	
	// adjust all paddles to common orientation
	if (paddle == BOTTOM_PADDLE){
		parallel_vel = b->vx;
		perpen_vel = b->vy;
		normalized_dist = cb - cp;
	}
	else if (paddle == TOP_PADDLE){
		parallel_vel = -1*b->vx;
		perpen_vel = -1*b->vy;
		normalized_dist = cp - cb;
	}
	else if (paddle == LEFT_PADDLE){
		parallel_vel = b->vy;
		perpen_vel = -1*b->vx;
		normalized_dist = cb - cp;
	}
	else if (paddle == RIGHT_PADDLE){
		parallel_vel = -1*b->vy;
		perpen_vel = b->vx;
		normalized_dist = cp - cb;
	}
	
	// determine adjustments based on where on the paddle we hit
	if (abs(normalized_dist) <= PADDLE_NORM_ZONE) {
		// if we hit in the "normal" zone, just reflect away at the incident angle
		perpen_vel *= -1;
	}
	else if (normalized_dist <= 0){
		if (parallel_vel >= 0){
			// hit left from left, reflect back the way it came
			parallel_vel *= -1;
			perpen_vel *= -1;
		}
		else {
			// hit left from right, reflect at the incident angle
			perpen_vel *= -1;
		}
	}
	else {
		if (parallel_vel >= 0){
			// hit right from left, reflect at the incident angle
			perpen_vel *= -1;
		}
		else {
			// hit right from right, reflect back the way it came
			parallel_vel *= -1;
			perpen_vel *= -1;
		}
	}
	
	// convert back from common orientation to unique paddle orientation
	if (paddle == BOTTOM_PADDLE){
		b->vx = parallel_vel;
		b->vy = perpen_vel;
	}
	else if (paddle == TOP_PADDLE){
		b->vx = -1*parallel_vel;
		b->vy = -1*perpen_vel;
	}
	else if (paddle == LEFT_PADDLE){
		b->vy = parallel_vel;
		b->vx = -1*perpen_vel;
	}
	else if (paddle == RIGHT_PADDLE){
		b->vy = -1*parallel_vel;
		b->vx = perpen_vel;
	}
}

// Draws a single brick in a certain color, but only if the brick is active
void Draw_Brick(BRICK_T * b, COLOR_T * c){
	PT_T p, pp;
	p = b->loc;
	pp.X = b->loc.X + BRICK_SIDE_LENGTH;
	pp.Y = b->loc.Y + BRICK_SIDE_LENGTH;
	os_mut_wait(&LCD_mutex, WAIT_FOREVER);
	TFT_Fill_Rectangle(&p, &pp, c);
	os_mut_release(&LCD_mutex);
}

// Redraws all bricks. Necessary to avoid ball erase making it look as though contact happened.
void Redraw_Bricks(GAME_T * g){
	uint8_t i, j;
	// check each brick
	for (i = 0; i < NUM_BRICK_ROWS; i++){
		for (j = 0; j < NUM_BRICK_COLS; j++){
			BRICK_T * brick = &g->bricks[i][j];
			if (brick->protection > 0){
				brick->protection--;
				Draw_Brick(brick, &g->colors.c_brick_invulnerable);		// invulnerable while protected
			}
			else if (brick->health == BRICK_DEAD){
				Draw_Brick(brick, &g->colors.c_bg);
			}
			else if (brick->health == BRICK_V_WEAK){
				Draw_Brick(brick, &g->colors.c_brick_v_weak);
			}
			else if (brick->health == BRICK_WEAK){
				Draw_Brick(brick, &g->colors.c_brick_weak);
			}
			else if (brick->health == BRICK_STRONG){
				Draw_Brick(brick, &g->colors.c_brick_strong);
			}
			else if (brick->health == BRICK_V_STRONG){
				Draw_Brick(brick, &g->colors.c_brick_v_strong);
			}
		}
	}
}

// Detects the first ball collision with a brick and takes the appropriate action
void Detect_Brick_Collision(GAME_T * g){
	uint8_t i, j;
	int16_t left_ball, right_ball, top_ball, bottom_ball;
	
	// get ball hitbox
	left_ball = g->ball.loc.X;
	right_ball = g->ball.loc.X + BALL_SIDE_LENGTH;
	top_ball = g->ball.loc.Y;
	bottom_ball = g->ball.loc.Y + BALL_SIDE_LENGTH;
	
	// check each brick
	for (i = 0; i < NUM_BRICK_ROWS; i++){
		for (j = 0; j < NUM_BRICK_COLS; j++){
			BRICK_T * brick = &g->bricks[i][j];
			if (brick->health != BRICK_DEAD && brick->protection == 0){
				int16_t left_brick, right_brick, top_brick, bottom_brick;
				
				// get brick hitbox
				left_brick = brick->loc.X;
				right_brick = brick->loc.X + BRICK_SIDE_LENGTH;
				top_brick = brick->loc.Y;
				bottom_brick = brick->loc.Y + BRICK_SIDE_LENGTH;
				
				if ((top_ball >= top_brick && top_ball <= bottom_brick) || (bottom_ball >= top_brick && bottom_ball <= bottom_brick)){
					if ((left_ball >= left_brick && left_ball <= right_brick) || (right_ball >= left_brick && right_ball <= right_brick)){
						int8_t in_left, in_right, in_top, in_bottom;
						int8_t * max;
						
						// trigger sound
						os_evt_set(EV_PLAYSOUND, t_Sound_Manager);
						
						// reduce health if not protected
						brick->health--;
						
						// add protection if necessary to prevent double hits
						if (brick->health > BRICK_DEAD){
							if (brick->health == BRICK_STRONG) brick->protection = BRICK_PROTECT_STRONG;
							if (brick->health == BRICK_WEAK) brick->protection = BRICK_PROTECT_WEAK;
							if (brick->health == BRICK_V_WEAK) brick->protection = BRICK_PROTECT_V_WEAK;
						}
						else {
							g->hits++;
						}
						
						// determine what's the biggest length in
						in_right = right_brick - left_ball;
						if (in_right > BRICK_SIDE_LENGTH) in_right = 0;
						in_left = right_ball - left_brick;
						if (in_left > BRICK_SIDE_LENGTH) in_left = 0;
						in_top = bottom_ball - top_brick;
						if (in_top > BRICK_SIDE_LENGTH) in_top = 0;
						in_bottom = top_brick - bottom_ball;
						if (in_bottom > BRICK_SIDE_LENGTH) in_bottom = 0;
						max = &in_right;
						if (in_left > *max) max = &in_left;
						if (in_top > *max) max = &in_top;
						if (in_bottom > *max) max = &in_bottom;
						
						// according to the length that is biggest in, reflect
						if (max == &in_left || max == &in_right){
							// on the top/bottom of the brick
							g->ball.vy *= -1;
						}
						else if (max == &in_top || max == &in_bottom){
							// on the left/right of the brick
							g->ball.vx *= -1;
						}
						
						return;
					}
				}
			}
		}
	}
}

// Redraws the info bar
void Redraw_Info_Bar(GAME_T * g){
	char buffer[20];
	
	// utilization tracking
	// analysis has determined the max stack occurs here due to the char buffer
	#if TRACK_STACK == 1
		Update_Stack_Pointer(__current_sp(), TASK_GAME);
	#endif
	
	sprintf(buffer, "[STATUS]   Lives: %d", g->lives);
	os_mut_wait(&LCD_mutex, WAIT_FOREVER);
	TFT_Text_PrintStr_RC(0,0, buffer);
	os_mut_release(&LCD_mutex);
}

// Detects the ball making a collision with the wall (a loss)
void Detect_Wall_Collision(GAME_T * g){
	uint8_t detected = 0;
	int8_t * vel;
	
	// bottom wall
	if (g->ball.loc.Y + BALL_SIDE_LENGTH > TFT_HEIGHT - PADDLE_BUFFER){
		detected++;			
		vel = &g->ball.vy;	// y-direction
	}	
	// top wall
	else if (g->ball.loc.Y < INFO_BAR_HEIGHT + PADDLE_BUFFER){
		detected++;			
		vel = &g->ball.vy;	// y-direction
	}			
	// right wall
	else if (g->ball.loc.X + BALL_SIDE_LENGTH > TFT_WIDTH - PADDLE_BUFFER){
		detected++;
		vel = &g->ball.vx;	// x-direction
	}
	// left wall
	else if (g->ball.loc.X < PADDLE_BUFFER){
		detected++;
		vel = &g->ball.vx;	// x-direction
	}
	
	// handle detection if it occurred
	if (detected){
		// if lives remain (or we are cheating), bounce the ball off the wall
		if (g->lives > 1 || g->state == GAME_CHEAT){
			*vel *= -1;
			if (g->state != GAME_CHEAT){
				g->lives--;
			}
		}
		// if out of lives, end game
		else {
			g->lives = 0;
			g->state = GAME_LOSS;
		}
	}
}

// Initiates playing of the game after it has been initialized
void Start_Game(GAME_T * g){
	g->ball.vy = BALL_INITIAL_VY;
	g->ball.vx = BALL_INITIAL_VX;
	g->state = GAME_PLAYING;
}
