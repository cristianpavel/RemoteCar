#ifndef __TIMER1__
#define __TIMER1__

/**
 * INIT timer1.
 */
void init_timer1(void);

/**
 * GET OCR if duty_cycle is wanted.
 *
 */
int compute_OCR_for_duty_cycle(float duty_cycle);

#endif
