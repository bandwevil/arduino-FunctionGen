
#include <avr/delay.h>
#include <avr/interrupt.h>
#include "interrupt.h"
#include "dac.h"

#define BUTTON_DOWN 0
#define BUTTON_UP 1

#define WAVE_SQUARE 0
#define WAVE_SAWTOOTH 1
#define WAVE_SINE 2
