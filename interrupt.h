#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

/*
 * Initializes timer 0 for interrupts every X us
 */
void initTimer0();

/*
 * Initializes timer 2 for interrupts every X us
 */
void initTimer2(int freq);

void origInit();
