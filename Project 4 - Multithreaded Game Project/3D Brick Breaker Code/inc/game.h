#ifndef game_h
#define game_h

#include <stdint.h>
#include "tft_lcd.h"

// PADDLES
#define PADDLE_BUFFER				(5)
#define HORZ_PADDLE_WIDTH 	(50)
#define HORZ_PADDLE_HEIGHT	(6)
#define VERT_PADDLE_WIDTH		(HORZ_PADDLE_HEIGHT)
#define VERT_PADDLE_HEIGHT	(HORZ_PADDLE_WIDTH)
#define ORIENTATION_VERT		(0)
#define ORIENTATION_HORZ		(1)
#define BOTTOM_PADDLE				(1)
#define TOP_PADDLE					(2)
#define LEFT_PADDLE					(3)
#define RIGHT_PADDLE				(4)
#define PADDLE_NORM_ZONE		(HORZ_PADDLE_WIDTH/6)
typedef struct {
	PT_T loc;
	int8_t v;
} PADDLE_T;
typedef struct {
	PADDLE_T lp;
	PADDLE_T rp;
	PADDLE_T tp;
	PADDLE_T bp;
} PADDLES_T;

// GAME COLORS
typedef struct {
	COLOR_T c_bg;
	COLOR_T c_pad;
	COLOR_T c_ball;
	COLOR_T c_brick_v_weak;
	COLOR_T c_brick_weak;
	COLOR_T c_brick_strong;
	COLOR_T c_brick_v_strong;
	COLOR_T c_brick_invulnerable;
} COLORS_T;

// BALL
#define BALL_SIDE_LENGTH	(HORZ_PADDLE_HEIGHT)
#define BALL_INITIAL_VX 	(-2)
#define BALL_INITIAL_VY 	(-3)
typedef struct {
	PT_T loc;
	int8_t vx;
	int8_t vy;
} BALL_T;

// BRICKS
#define BRICK_SIDE_LENGTH	(10)
#define BRICK_DEAD			(0)
#define BRICK_V_WEAK		(1)
#define BRICK_WEAK			(2)
#define BRICK_STRONG		(3)
#define BRICK_V_STRONG	(4)
#define NUM_BRICK_ROWS	(10)
#define NUM_BRICK_COLS	(6)
#define BRICK_GAP				(10)
#define BRICK_START_X		(((TFT_WIDTH-(NUM_BRICK_COLS)*(BRICK_SIDE_LENGTH)-(NUM_BRICK_COLS-1)*BRICK_GAP))/2)
#define BRICK_START_Y		(((TFT_HEIGHT-INFO_BAR_HEIGHT-(NUM_BRICK_ROWS)*(BRICK_SIDE_LENGTH)-(NUM_BRICK_ROWS-1)*BRICK_GAP))/2+INFO_BAR_HEIGHT)
#define BRICK_PROTECT_STRONG	(25)
#define BRICK_PROTECT_WEAK		(12)
#define BRICK_PROTECT_V_WEAK	(6)
#define BRICK_TOTAL_NUM	(NUM_BRICK_ROWS*NUM_BRICK_COLS)
typedef struct {
	PT_T loc;
	uint8_t health;
	uint8_t protection;
} BRICK_T;

// STATUS button
#define STATUS_BUT_WIDTH	(TFT_WIDTH/2)
#define STATUS_BUT_HEIGHT	(2*INFO_BAR_HEIGHT)

// GAME structure will contain all the appropriate elements of a game in one central location
#define INFO_BAR_HEIGHT		(17)
typedef struct {
	PADDLES_T paddles;
	COLORS_T colors;
	BALL_T ball;
	BRICK_T bricks[NUM_BRICK_ROWS][NUM_BRICK_COLS];
	uint8_t hits;
	uint8_t lives;
} GAME_T;


// ****************
// HELPER FUNCTIONS
// ****************
void Game_Init(GAME_T * g);									// Sets up the initial game state
void Redraw_Vertical_Paddles(GAME_T * g);		// Redraw, including movement, the vertical paddles
void Redraw_Horizontal_Paddles(GAME_T * g);	// Redraw, including movement, the horizontal paddles
void Redraw_Paddles(GAME_T * g);						// Redraws, including movement, of both horizontal and vertical paddles
void Move_Paddles(GAME_T * g);							// Adjust the positions of paddles
void Convert_Tilt(GAME_T * g);							// Convert pitch and roll to velocities of the paddles
void Redraw_Ball(GAME_T * g);								// Redraw, including movement, the ball
void Move_Ball(GAME_T * g);									// Adjust the position of ball
void Detect_Paddle_Collision(GAME_T * g);		// Detect the ball making a collision with a paddle
void Calc_Bounce_Velocities(BALL_T * b, int16_t cb, int16_t cp, uint8_t paddle);		// Calculate the new velocities of a ball after contacting the paddle depending on where it hits
void Draw_Brick(BRICK_T * b, COLOR_T * c);	// Draws a single brick in a certain color, but only if the brick is active
void Detect_Brick_Collision(GAME_T * g);		// Detects the first ball collision with a brick and takes the appropriate action
void Redraw_Bricks(GAME_T * g);							// Redraws all bricks. Necessary to avoid ball erase making it look as though contact happened.

#endif
