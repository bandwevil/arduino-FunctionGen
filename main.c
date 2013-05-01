
#include "main.h"

#define BUTTON_DOWN 0
#define BUTTON_UP 1

volatile unsigned char interruptCounter;

int buttonPressed();
void squareWave();

int main()
{
   int freq = 100, buttonState = 0, button;
   DDRB |= (1<<5);
   DDRD &= ~(1<<2);
   PORTD |= (1<<2);

   initTimer2(freq);

   while (1) {
      button = buttonPressed();
      if (button == 0 && buttonState == BUTTON_UP) {
         if (freq != 500) {
            freq += 100;
         } else {
            freq = 100;
         }
         buttonState = BUTTON_DOWN;
         initTimer2(freq);
      } else if (button == 1 && buttonState == BUTTON_DOWN) {
         buttonState = BUTTON_UP;
      }
   }
}

/*
 * Checks if the button input on Pin B3 is pressed or not.
 * Polls the button state every 1ms, 4 consecutive reads means that it's stable.
 * Returns 1 if the button is up, 0 if it is pressed.
 */
int buttonPressed() {
   int previous, next, i = 0;
   previous = PIND & (1<<2);

   while (i < 4) {
      _delay_ms(1);
      next = PIND & (1<<2);
      if (previous == next) {
         i++;
      } else {
         i = 0;
      }
      previous = next;
   }

   if (next >= 1) {
      return 1;
   } else {
      return 0;
   }
}


ISR(TIMER2_COMPA_vect) {
   if (interruptCounter == 40) {
      interruptCounter = 0;
   } else {
      interruptCounter++;
   }
   squareWave();
}

void squareWave() {
   if (interruptCounter >= 20) {
      PORTB |= (1<<5);
   } else {
      PORTB &= ~(1<<5);
   }
}
