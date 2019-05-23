#include "l298n.h"
#include "timer1.h"


static motor_t motor;

/**
 * Initialization of IO ports.
 *
 */
static void IO_init(void)
{

	/* INPUTS are set as outputs */
	DDR_INPUT |= (1 << INPUT1) | (1 << INPUT2)
	       	| (1 << INPUT3) | (1 << INPUT4);	

	/* Set ENABLES as outputs */
	DDRD |= (1 << PD5) | (1 << PD4);

	PORTD &= ~(1 << PD4);
	PORTD &= ~(1 << PD5);

	/* INPUTUTS are set to LOW*/
	PORT_INPUT &= ~(1 << INPUT1);
	PORT_INPUT &= ~(1 << INPUT2);
	PORT_INPUT &= ~(1 << INPUT3);
	PORT_INPUT &= ~(1 << INPUT4);
	

}

/**
 * Return 1 if motor is moving forward
 */
int is_moving_forward(void)
{

	return (int)motor.forward; 

}


/**
 * Return 1 if motor is stopped
 */
int is_stopped(void)
{

	return (int)motor.stopped; 

}
/**
 * Set speed.
 *
 */
static void set_speed(float speed_as_duty_cycle)
{

	if (speed_as_duty_cycle > 1.0
			|| speed_as_duty_cycle < 0.3)
		return;

	motor.speed = speed_as_duty_cycle;

	ENABLE_A = compute_OCR_for_duty_cycle(motor.speed);
	ENABLE_B = compute_OCR_for_duty_cycle(motor.speed);

}


/**
 * Initialize l298n driver.
 */
void init_l298n(float speed_as_duty_cycle)
{
	
	IO_init();
	init_timer1();
	set_speed(speed_as_duty_cycle);
	motor.stopped = 1;
}

/**
 * Make motors move at same speed.
 */
void equalize_speed(void)
{
	set_speed(motor.speed);

}

/**
 * Stop the motors
 *
 */
void stop()
{
	motor.forward = 0;
	motor.stopped = 1;
	/* INPUTUTS are set to LOW*/
	PORT_INPUT &= ~(1 << INPUT1);
	PORT_INPUT &= ~(1 << INPUT2);
	PORT_INPUT &= ~(1 << INPUT3);
	PORT_INPUT &= ~(1 << INPUT4);
	
}

/**
 * Move motors forward.
 *
 */
void move_forward(void)
{
	motor.forward = 1;
	motor.stopped = 0;
	equalize_speed();
	PORT_INPUT |= (1 << INPUT1);
	PORT_INPUT &= ~(1 << INPUT2);
	PORT_INPUT |= (1 << INPUT3);	
	PORT_INPUT &= ~(1 << INPUT4);
}


/**
 * Move motors backwards.
 *
 */
void move_backwards(void)
{
	motor.forward = 0;
	motor.stopped = 0;
	equalize_speed();
	PORT_INPUT &= ~(1 << INPUT1);
	PORT_INPUT |= (1 << INPUT2);
	PORT_INPUT &= ~(1 << INPUT3);
	PORT_INPUT |= (1 << INPUT4);

}


/**
 * Steer left, meaning move
 * right side forward and left side
 * bacwards.
 *
 */
void steer_left(void)
{
	motor.forward = 1;
	motor.stopped = 0;

	equalize_speed();
	ENABLE_A = 0;
	ENABLE_B = ICR1;
	PORT_INPUT &= ~(1 << INPUT1);
	PORT_INPUT |= (1 << INPUT2);
	
	PORT_INPUT |= (1 << INPUT3);
	PORT_INPUT &= ~(1 << INPUT4);

	
}


/**
 * Steer right, meaning move
 * left side forward and right
 * side bacwards.
 *
 */
void steer_right(void)
{
	motor.forward = 1;
	motor.stopped = 0;

	equalize_speed();
	ENABLE_B = 0;
	ENABLE_A = ICR1;
	PORT_INPUT |= (1 << INPUT1);
	PORT_INPUT &= ~(1 << INPUT2);
	
	PORT_INPUT &= ~(1 << INPUT3);
	PORT_INPUT |= (1 << INPUT4);

}

void speed_up(void)
{
	set_speed(motor.speed + SPEED_UNIT);

}

void speed_down(void)
{

	set_speed(motor.speed - SPEED_UNIT);
}

