#include "main.h"

volatile unsigned char interruptCounter, waveState;

int buttonPressed();
void squareWave();
void sawtoothWave();
void sineWave();
int handleButton(int pin, int* state);

int main()
{
   int freq = 100, button0State = 0;
   DDRB |= (1<<5);
   DDRD &= ~(1<<2);
   PORTD |= (1<<2);

   waveState = WAVE_SQUARE;

   initTimer2(freq);
   Initialize_SPI_Master();

   while (1) {
      if (handleButton(2, &button0State)) {
         freq = (freq%500) + 100;
         initTimer2(freq);
      }
   }
}

int handleButton(int pin, int* state) {
   int in;

   in = buttonPressed(pin);
   if (in == 0 && *state == BUTTON_UP) {
      *state = BUTTON_DOWN;
      return 1;
   } else if (button == 1 && *state == BUTTON_DOWN) {
      *state = BUTTON_UP;
   }

   return 0;
}

/*
 * Checks if the button input on Pin B3 is pressed or not.
 * Polls the button state every 1ms, 4 consecutive reads means that it's stable.
 * Returns 1 if the button is up, 0 if it is pressed.
 */
int buttonPressed(int pin) {
   int previous, next, i = 0;
   previous = PIND & (1<<pin);

   while (i < 4) {
      _delay_ms(1);
      next = PIND & (1<<pin);
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
   //InterruptCounter gives us the current 'position' we are in the wave's period
   //We have 40 divs / period, so we reset it after 40 ticks
   if (interruptCounter == 40) {
      interruptCounter = 0;
   } else {
      interruptCounter++;
   }

   switch(waveState) {
      case WAVE_SQUARE:
         squareWave();
         break;
      case WAVE_SAWTOOTH:
         sawtoothWave();
         break;
      case WAVE_SINE:
         sineWave();
         break;
   }
}

void squareWave() {
   if (interruptCounter >= 20) {
      Transmit_SPI_Master(0x000);
   } else {
      Transmit_SPI_Master(0xFFF);
   }
}

void sawtoothWave() {
   Transmit_SPI_Master(105*interruptCounter);
}

void sineWave() {
}
