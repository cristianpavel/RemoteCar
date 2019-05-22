#include "timer1.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/**
 * Init timer in PWM phase correct
 * with top at ICR1 and
 *
 * COM1A1:0 = 2
 * ICR1 = 0xFFFF.
 *
 */
void init_timer1(void)
{
	DDRD |= (1 << PD5) | (1 << PD4);



	TCCR1B |= (1 << WGM13) | (1 << CS11);
	TCCR1A |= (1 << COM1A1)  | (1 << COM1B1);

	ICR1 = 0xFFFF;
	
	OCR1A = 0;
	OCR1B = 0;

}

/**
 * Return value for OCR if duty_cycle is
 * wanted.
 *
 */
int compute_OCR_for_duty_cycle(float duty_cycle)
{
	
	return ICR1 * duty_cycle;


}	
