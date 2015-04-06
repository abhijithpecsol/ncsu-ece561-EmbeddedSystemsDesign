// period/delay definitions
#define PERIOD_READ_ACCEL 	(1)			// 100 ms / 100 ms = 1
#define PERIOD_UPDATE_LEDS 	(5)			// 500 ms / 100 ms = 5

// LPTMR period definitioms
#define LED_ON_TIME					(5)			// LEDs must be on for 10 ms
#define ACCEL_CHECK_FREQ		(50)		// check accelerometer every 100 ms

// misc
#define DEG_TO_RAD(X)				(((X)*(180/3.14159265f)))

// debug definitions
#define ALWAYS_RED					1
#define DEBUG_SIGNALS				0

// incremental changes definitions
#define USE_SLEEP_MODES			1
#define RUN_I2C_FAST				1
#define USE_RADIANS					1
#define USE_PWM							1
#define LOW_POWER_ACCEL			1
#define PWM_SLEEP						1
