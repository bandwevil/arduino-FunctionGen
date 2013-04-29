#include "dac.h"

void tri(int from, int to) {
   int i, increment;
   if (from < to) {
      increment = 55;
   } else {
      increment = -55;
   }

   for (i = 0; i < 20; i++) {
      Transmit_SPI_Master(from);
      from += increment;
      /*_delay_us(delay);*/
   }
}

void Initialize_SPI_Master(void)
{
   DDRB = 1<<MOSI | 1<<SCK | 1<<SS | 1<<LDAC;	// make MOSI, SCK, LDAC, and SS outputs

   SPCR = (0<<SPIE) | 		//No interrupts
      (1<<SPE) | 			//SPI enabled
      (0<<DORD) | 			//shifted out LSB
      (1<<MSTR) | 			//master
      (1<<CPOL) | 			//rising leading edge   
      (1<<CPHA) | 			//sample leading edge   
      (0<<SPR1) | (0<<SPR0) ; 	//clock speed   
   SPSR = (0<<SPIF) | 		//SPI interrupt flag
      (0<<WCOL) | 			//Write collision flag
      (0<<SPI2X) ; 			//Doubles SPI clock
   PORTB |= (1 << SS);  		// make sure SS is high
   PORTB &= ~(1 << LDAC); //Set LDAC to low, transfer on SS
}

void Transmit_SPI_Master(int Data) {
   PORTB &= ~(1 << SS); 		//Assert slave select 
   SPDR = ((Data >> 8) & 0x0F) | 0x10; 	//Attach configuration Bits onto MSB 
   while (!(SPSR & (1<<SPIF)));
   SPDR = 0xFF & Data;
   while (!(SPSR & (1<<SPIF)));
   PORTB |= (1 << SS);
}
