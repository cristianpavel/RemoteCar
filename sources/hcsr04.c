#include "hcsr04.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define TOP 0xFF
#define SPEED_OF_SOUND_CM_S 34300.0
#define CLOCK_FREQ_MHz 16.0

static long long timer_ovf;


/**
 *
 * Init timer2. No prescaler.
 * Interrupt enabled on OVF.
 *
 */
static void init_timer2(void)
{
	timer_ovf = 0;
	TCNT2 = 0;
	TCCR2B |= (1 << CS20);
	TCNT2 = 0;

	TIMSK2 |= (1 << TOIE2);

}

/**
 * Stop timer 2.
 *
 */
static void stop_timer2(void)
{
	TCNT2 = 0;
	TCCR2B = 0;
	OCR2A = 0;
}


/**
 * Send 10 us trigger.
 *
 */
void send_trig(void)
{

	PORT_HCSR04 |= (1 << TRIG);
	_delay_us(10);
	PORT_HCSR04 &= ~(1 << TRIG);
	

}


/**
 * Increase timer_ovf variable
 * on overflow.
 *
 */
ISR(TIMER2_OVF_vect) {
	
	timer_ovf++;

}



/**
 * Wait for echo response and calculate
 * distance according to the duration
 * of high signal. 
 *
 * Factor is for convertig timer counter time
 * in real-time, then using the speed of sound
 * get the distance.
 *
 */
float wait_for_echo_and_get_distance(void)
{
	float seconds;
	float measure_distance = 0;
	while ((PIN_HCSR04 & (1 << ECHO)) == 0);
	init_timer2();
	while ((PIN_HCSR04 & (1 << ECHO)) != 0);
	
	float factor = (CLOCK_FREQ_MHz * 1000000);

	seconds= (float)(TCNT2 + TOP * timer_ovf) / factor;
	measure_distance = SPEED_OF_SOUND_CM_S * seconds / 2;
	stop_timer2();

	return measure_distance;	

}

/**
 * TRIG is OUTPUT.
 * ECHO is INPUT.
 *
 */
static void IO_init(void)
{
	DDR_HCSR04 |= (1 << TRIG)
		      &~(1 << ECHO); 
	PORT_HCSR04 &= ~(1 << TRIG);

	PORT_HCSR04 &= ~(1 << ECHO);

}


void init_hcsr04(void)
{

	IO_init();
	_delay_us(2);

}

