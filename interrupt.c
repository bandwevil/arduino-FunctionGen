/*
 * CPE 329 - Interrupt tutorial
 */

#include "interrupt.h"

/*
 *int main() {
 *   myCounter = 1;
 *   DDRB |= (1<<5);
 *   DDRD |= (1<<6);
 *
 *   initTimer2();
 *   [>origInit();<]
 *
 *
 *   while(1) { //infinite blank loop, interrupt does actual work
 *   }
 *   return 1;
 *}
 */

void origInit() {
   TCCR0A = 0x01; // timer overflow mode
   TCCR0B = 0x05; // timer clk = system clk / 1024
   TIFR0 = 0x01;     // clear previous timer overflow
   TIMSK0 = 0x01; // timer compare interrupt enabled
}

void initTimer2(int freq) {
   cli(); //Make sure we don't hit an interrupt while changing settings

   TCCR2A = 0;// set entire TCCR2A register to 0
   TCCR2B = 0;// same for TCCR2B
   TCNT2  = 0;//initialize counter value to 0
   // turn on CTC mode
   TCCR2A |= (1 << WGM21);
   // enable timer compare interrupt
   TIMSK2 |= (1 << OCIE2A);

   //Set fequency of interrupts, default to 100 Hz
   //Interrupts occur 40x every period
   if (freq == 500) {
      OCR2A = 100;
      TCCR2B |= (1 << CS21);
   } else if (freq == 400) {
      OCR2A = 124;
      TCCR2B |= (1 << CS21);
   }
   else if (freq == 300) {
      OCR2A = 166;
      TCCR2B |= (1 << CS21);
   } else if (freq == 200) {
      OCR2A = 250;
      TCCR2B |= (1 << CS21);
   } else {
      OCR2A = 124;
      TCCR2B |= (1 << CS21) | (1 << CS20);
   }

   sei(); //re-enable interrupts
}

void initTimer0() {
   //set timer0 interrupt at 2kHz
   TCCR0A = 0;// set entire TCCR0A register to 0
   TCCR0B = 0;// same for TCCR0B
   TCNT0  = 0;//initialize counter value to 0
   // set compare match register for 2khz increments
   OCR0A = 124;// = (16*10^6) / (2000*64) - 1 (must be <256)
   // turn on CTC mode
   TCCR0A = 0x01;
   /*TCCR0A |= (1 << WGM01);*/
   // Set CS01 and CS00 bits for 64 prescaler
   TCCR0B |= (1 << CS01) | (1 << CS00);
   // enable timer compare interrupt
   TIMSK0 = 0x01; // timer compare interrupt enabled
}
