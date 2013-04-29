#define SS   2  // PB pin 2
#define MOSI 3  // PB pin 3
#define LDAC 4  // PB pin 4
#define SCK  5  // PB pin 5

#include <avr/io.h>
#include <util/delay.h>

void Initialize_SPI_Master(void);
void Transmit_SPI_Master(int Data);
void tri(int from, int to);
