#include <util/delay.h>
#include <avr/io.h>
#include "l298n.h"
#include "hcsr04.h"
#include "usart.h"

#define MIN_DISTANCE 30
#define OK '1'
#define MOVE_FORWARD '1'
#define MOVE_BACKWARDS '2'
#define STEER_LEFT_FORWARD '4'
#define STEER_RIGHT_FORWARD '3'
#define SPEED_UP '5'
#define SPEED_DOWN '6'
#define TURN_LEDS_ON '7'
#define TURN_LEDS_OFF '8'
#define BEEP_BEEP '9'
#define HORN_DURATION 1000
#define TRIG_DELAY 100
#define HORN_DELAY 2
#define STOP '0'
#define MAX_HITS 5

typedef struct {
	int hit;	/* count if HCSR04 sensor 
			 * sees obstacle too close
			 */

	int horn_activated; /* 1 if horn is activated */
	int leds_on;	    /* 1 if car lights are on */

} car_t;


static car_t car;
static int timer0_count;

/* Turn car leds off */
static void turn_leds_off(void)
{
	car.leds_on = 0;
	PORTA &= ~(1 << PA7) & ~(1 << PA6)
		& ~(1 << PA5) & ~(1 << PA4);

}

/* Turn car leds on */
static void turn_leds_on(void)
{

	car.leds_on = 1;
	PORTA |= (1 << PA7) | (1 << PA6)
		| (1 << PA5) | (1 << PA4);

}

/* Car horn */
static void beep_beep(void)
{
	int i;

	for (i = 0; i < HORN_DURATION / HORN_DELAY; i++) {
		PORTB ^= (1 << PB3);
		_delay_ms(HORN_DELAY);
	}	


}


ISR(TIMER0_COMPA_vect)
{

	if (timer0_count >= 50) {
		/* toggle leds */
		PORTA ^= (1 << PA4);
		PORTA ^= (1 << PA5);
		timer0_count = 0;

	}

	timer0_count++;

}


/** Blink back lights */
static void signal_backwards(void)
{
	/* init timer 0 for blinking*/
	/* CTC mode with top OCR0A,
	 * prescaler 1024
	 */
	
	if (!is_moving_forward() &&
		!is_stopped())
		return;

	timer0_count = 0;
	TCCR0A |= (1 << WGM01);

	TCCR0B |= (1 << CS02);
	TIMSK0 |= (1 << OCIE0A);

	OCR0A = 0xFF; 

}

/** Stop blinking */
static void stop_signal_backwards(void)
{
	TCCR0B &= ~(7 << CS00);

	if (car.leds_on) {
		turn_leds_on();
	} else {
		turn_leds_off();
	}
}

/* Init buzzer and leds */
static void init(void)
{
	/* leds as outputs*/
	DDRD |= (1 << PD7);
	DDRA |= (1 << PA7) | (1 << PA6)
		| (1 << PA5) | (1 << PA4);

	/* leds off */
	PORTD |= (1 << PD7);
	PORTA &= ~(1 << PA7) & ~(1 << PA6)
		& ~(1 << PA5) & ~(1 << PA4);


	/* buzzer as output */
	DDRB |= (1 << PB3);
	
	/* buzzer on low */
	PORTB &= ~(1 << PB3);


}



/**
 * Intrerupere la receive
 * pentru USART0.
 *
 */
ISR(USART0_RX_vect)
{

	char data = UDR0;
	switch (data) {

	case MOVE_FORWARD:
		if (car.hit < MAX_HITS) {
			move_forward();
		}
		break;
	case MOVE_BACKWARDS:
		signal_backwards();
		move_backwards();
		break;
	case STEER_LEFT_FORWARD:
		if (car.hit < MAX_HITS) {
			steer_left();
		}
		break;
	case STEER_RIGHT_FORWARD:
		if (car.hit < MAX_HITS) {
			steer_right();
		}
		break;
	case SPEED_UP:
		speed_up();
		break;
	case SPEED_DOWN:
		speed_down();
		break;
	case TURN_LEDS_ON:
		turn_leds_on();
		break;
	case TURN_LEDS_OFF:
		turn_leds_off();
		break;
	case BEEP_BEEP:
		car.horn_activated = 1;
		break;
	case STOP:
		stop();
	default:
		break;
	}




}



int main()
{
	
	sei();
	init();
	USART0_init();
	init_l298n(0.5);
	init_hcsr04();
	
	while(1) {

				
		send_trig();
		float measure_distance = wait_for_echo_and_get_distance();
                if (measure_distance < MIN_DISTANCE) {
			PORTD ^= (1 << PD7);
			car.hit++;
		} else {
			car.hit = 0;
		}	

		if (car.hit > MAX_HITS &&
			is_moving_forward()) {
			USART0_transmit(STOP);
			stop();
		} else {

			USART0_transmit(OK);
		}
	
		if (is_moving_forward()
				|| is_stopped()) {
			stop_signal_backwards();
		}

		if (car.horn_activated == 1) {
			beep_beep();
			car.horn_activated = 0;
		} else {
			_delay_ms(TRIG_DELAY);
		}
		

        }




	

	return 0;

}
