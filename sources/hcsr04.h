#ifndef __HCSR04__
#define __HCSR04__


#define TRIG PC0
#define ECHO PC1
#define PORT_HCSR04 PORTC
#define PIN_HCSR04 PINC
#define DDR_HCSR04 DDRC

/**
 * INIT function.
 */
void init_hcsr04(void);

/** 
 * Send trigger.
 */
void send_trig(void);

/**
 * Compute distance away from obstacle.
 */
float wait_for_echo_and_get_distance(void);
#endif
