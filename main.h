
#include <avr/delay.h>
#include <avr/interrupt.h>
#include "interrupt.h"
#include "dac.h"

#define BUTTON_DOWN 0
#define BUTTON_UP 1

#define WAVE_SQUARE 0
#define WAVE_SAWTOOTH 1
#define WAVE_SINE 2
#define WAVE_CUSTOM 3

//Lookup table for the sine wave
int sinTable[41] = {2147, 2367, 2679, 2976, 3250, 3494, 3703, 3870, 3993, 4068, 4094, 4068, 3993, 3870, 3703, 3494, 3250, 2976, 2679, 2367, 2047, 1726, 1414, 1117, 843, 599, 390, 223, 100, 25, 0, 25, 100, 223, 390, 599, 843, 1117, 1414, 1726, 1900};
int customTable[41];

int buttonPressed();
void Initialize_ADC0();
void squareWave();
void sawtoothWave();
void sineWave();
void customWave();
int handleButton(int pin, int* state);
