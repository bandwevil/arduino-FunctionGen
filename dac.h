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

#include <avr/io.h>
#include <util/delay.h>

#define SS   2  // Pin B2
#define MOSI 3  // Pin B3
#define LDAC 4  // Pin B4
#define SCK  5  // Pin B5

/*
 * Sets up the SPI to write out to the pins defined above, connected to the DAC.
 */
void Initialize_SPI_Master(void);

/*
 * Transmits 12 bits to the DAC with the appropriate header.
 * args:
 * Data - A 12 bit number to be output via SPI, top 4 bits are used as config
 */
void Transmit_SPI_Master(int Data);
