#define SS   2  // PB pin 2
#define MOSI 3  // PB pin 3
#define LDAC 4  // PB pin 4
#define SCK  5  // PB pin 5

#include <avr/io.h>
#include <util/delay.h>

/*
 * Sets up the SPI to write out to the pins defined above, connected to the DAC.
 */
void Initialize_SPI_Master(void);

/*
 * Transmits 12 bits to the DAC with the appropriate header.
 * Takes in a values from 0x000 to 0xFFF
 */
void Transmit_SPI_Master(int Data);

/*
 * Generates a sloped wave corresponding to half of a triabgle wave.
 * Hardcoded at the moment to a specific frequency and amplitude.
 */
void tri(int from, int to);
