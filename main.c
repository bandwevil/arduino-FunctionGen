/*
 * CPE 329 - Spring 2013
 * Project 2: Function Generator
 *
 * Tyler Saadus and Jonathan Hernandez
 *
 * Outputs square, sawtooth, and sine waves to an external DAC at varying
 * frequencies and duty cycles. Also supports analog input sampling and mirroring
 * through the onboard ADC.
 *
 */

#include "main.h"

volatile unsigned char interruptCounter, waveState, dutyCycle, button3State;

int main()
{
   int freq, button0State = BUTTON_UP, button1State = BUTTON_UP, button2State = BUTTON_UP;
   DDRB |= (1<<5);

   //Start with a 100 Hz square wave at 50% duty cycle
   waveState = WAVE_SQUARE;
   interruptCounter = 0;
   dutyCycle = 16;
   freq = 100;

   //Initalize other external function stuff
   initTimer2(freq);
   Initialize_SPI_Master();
   Initialize_ADC0();

   /*
    * After init, main will simply poll the buttons and change variables as needed
    *
    * Button 0 (Pin 2): Frequency control
    * Button 1 (Pin 3): Wave shape control
    * Button 2 (Pin 4): Duty cycle control
    * Button 3 (Pin 5): Analog input control (checked in ISR only)
    */
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

/*
 * Generic handling of buttons. Only returns 1 once per button press.
 * args:
 * pin - Which pin on Port D should be checked
 * state - variable that holds the button state between polls
 * returns:
 * 1 if the button was just pressed
 */
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
 * args:
 * pin - Which pin on Port D should be checked
 * returns:
 * 0 if the button is pressed, 1 if the button is up
 */
int buttonPressed(int pin) {
   int previous, next, i = 0;

   DDRD &= ~(1<<pin);
   PORTD |= (1<<pin);

   previous = PIND & (1<<pin);

   //We read the input every 1ms, waiting until we read the same value
   //4 times in a row, which means it's stable.
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

/*
 * Initalizes the analog to digital controller for input on pin A0
 */
void Initialize_ADC0() {
   ADCSRA = 0x87;	//Turn On ADC and set prescaler (CLK/128--62 kHz)
   //MAX A/D conversion rate 5 kHz @ 62 kHz frequency
   ADCSRB = 0x00;	//turn off autotrigger
   ADMUX = 0x00;    	//Set ADC channel ADC0 and AREF input (wire to 5V)
}

/*
 * Interrupt routine handles all the actual outputting of waves. Interrupts
 * are set to run 40 times every period.
 */
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

/*
 * Square wave output with variable duty cycle
 */
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

/*
 * Sawtooth wave
 */
void sawtoothWave() {
   Transmit_SPI_Master(100*interruptCounter);
}

/*
 * Sine wave based off lookup table
 */
void sineWave() {
   Transmit_SPI_Master(sinTable[interruptCounter]);
}

/*
 * Custom wave sampled from A0 when Pin D5's button is pressed.
 */
void customWave() {

   DDRD &= ~(1<<5);
   PORTD |= (1<<5);

   // If pin D5 is grounded then we will read in the current value at A0
   if((PIND & (1<<5)) == 0) {
      int input;
      ADCSRA = 0xC7;        // start conversion
      input = ADC & 0x3FF;  // read voltage from ADC

      // ADC goes from 0 to 0x3FF, multiply by 4 if we have room to get the
      // full 0 to 0xFFF output
      if ((input<<2) <= 0xFFF) {
         customTable[interruptCounter] = input << 2;
      } else {
         customTable[interruptCounter] = 0xFFF;
      }
   }

   Transmit_SPI_Master(customTable[interruptCounter]);
}
