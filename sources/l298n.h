#ifndef __L298N__
#define __L298N__
#include <avr/io.h>
#include <avr/interrupt.h>

#define SPEED_UNIT 0.15


#define ENABLE_A OCR1A
#define ENABLE_B OCR1B
#define INPUT1 PB7
#define INPUT2 PB6
#define INPUT3 PB5
#define INPUT4 PB4
#define DDR_INPUT DDRB
#define PORT_INPUT PORTB


typedef struct {

	int forward;
	int stopped;
	float speed;

} motor_t;

/**
 * INIT function.
 */
void init_l298n(float speed_as_duty_cycle);

/**
 * MOVE motors forward.
 */
void move_forward(void);

/**
 * STEER left.
 */
void steer_left(void);

/**
 * STEER right.
 */
void steer_right(void);

/**
 * MOVE motors bacwards.
 */
void move_backwards(void);

/**
 * INCREASE speed of motors.
 */
void speed_up(void);

/**
 * DECREASE speed of motors.
 */
void speed_down(void);

/**
 * Stop the motors.
 */ 
void stop(void);


/**
 * Check if motors are moving forward
 */
int is_moving_forward(void);

/**
 * Check if motors are stopped*/
int is_stopped(void);
#endif

