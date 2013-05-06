#include "main.h"

volatile unsigned char interruptCounter, waveState, dutyCycle, button3State;

int main()
{
   int freq = 100, button0State = BUTTON_UP, button1State = BUTTON_UP, button2State = BUTTON_UP;
   DDRB |= (1<<5);

   waveState = WAVE_SQUARE;
   interruptCounter = 0;
   dutyCycle = 16;

   initTimer2(freq);
   Initialize_SPI_Master();
   Initialize_ADC0();

   while (1) {
      if (handleButton(2, &button0State)) {
         freq = (freq%500) + 100;
         initTimer2(freq);
      }
      if (handleButton(3, &button1State)) {
         switch (waveState) {
            case WAVE_SQUARE:
               waveState = WAVE_SAWTOOTH;
               break;
            case WAVE_SAWTOOTH:
               waveState = WAVE_SINE;
               break;
            case WAVE_SINE:
               waveState = WAVE_CUSTOM;
               break;
            case WAVE_CUSTOM:
               waveState = WAVE_SQUARE;
               break;
            default:
               waveState = WAVE_SQUARE;
         }
      }
      if (handleButton(4, &button2State)) {
         if (dutyCycle < 40) {
            dutyCycle += 4;
         } else {
            dutyCycle = 0;
         }
      }
   }
}

int handleButton(int pin, int* state) {
   int in;

   in = buttonPressed(pin);
   if (in == 0 && *state == BUTTON_UP) {
      *state = BUTTON_DOWN;
      return 1;
   } else if (in == 1 && *state == BUTTON_DOWN) {
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

   DDRD &= ~(1<<pin);
   PORTD |= (1<<pin);

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

void Initialize_ADC0() {
   ADCSRA = 0x87;	//Turn On ADC and set prescaler (CLK/128--62 kHz)
   //MAX A/D conversion rate 5 kHz @ 62 kHz frequency
   ADCSRB = 0x00;	//turn off autotrigger
   ADMUX = 0x00;    	//Set ADC channel ADC0 and AREF input (wire to 5V)
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
      case WAVE_CUSTOM:
         customWave();
         break;
      default:
         squareWave();
   }
}

void squareWave() {
   if (dutyCycle != 0) {
      if (interruptCounter > dutyCycle) {
         Transmit_SPI_Master(0x000);
      } else {
         Transmit_SPI_Master(0xFFF);
      }
   } else {
      Transmit_SPI_Master(0x000);
   }
}

void sawtoothWave() {
   Transmit_SPI_Master(100*interruptCounter);
}

void sineWave() {
   Transmit_SPI_Master(sinTable[interruptCounter]);
}

void customWave() {

   DDRD &= ~(1<<5);
   PORTD |= (1<<5);

   if((PIND & (1<<5)) == 0) {
      int input;
      ADCSRA = 0xC7;        // start conversion
      input = ADC & 0x3FF;  // read voltage from ADC
      if ((input<<2) < 0xFFF) {
         customTable[interruptCounter] = input << 2;
      } else {
         customTable[interruptCounter] = 0xFFF;
      }
   }

   Transmit_SPI_Master(customTable[interruptCounter]);
}
