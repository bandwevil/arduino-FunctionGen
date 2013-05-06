#include<avr/io.h>
#include<util/delay.h>

void Initialize_ADC0(void);

int main(void)
{
   int i, voltage = 0;
   char toggle = 0;

   DDRD = 0xFF;           //jgh set port d to output
   UCSR0B = 0;            //jgh set usart off      
   DDRB = 1 << 5;              	// jgh Set PB5 output for led

   Initialize_ADC0();          	// Initialize ADC0 for manual trigger

   while(1)
   {
      ADCSRA = 0xC7;        		// start conversion
      _delay_us(260);			// ensure max sampling rate not exceeded
      voltage = ADC & 0x3FF;     		// jgh read voltage from PC0/AD0 (A5 for UNO rev 2)

      PORTD = (voltage >> 2) & 0xFF;    //jgh 8 msb to port d

      if (voltage < 205) {
         _delay_ms(500);
      } else if (voltage < 410) {
         _delay_ms(250);
      } else if (voltage < 614) {
         _delay_ms(125);
      } else if (voltage < 819) {
         _delay_ms(62);
      } else {
         _delay_ms(31);
      }
      PORTB ^= (1<<5);
   }

   return 0;
}

void Initialize_ADC0(void)
{
   ADCSRA = 0x87;	//Turn On ADC and set prescaler (CLK/128--62 kHz)
   //MAX A/D conversion rate 5 kHz @ 62 kHz frequency    
   ADCSRB = 0x00;	//turn off autotrigger
   ADMUX = 0x01;    	//Set ADC channel ADC0 and AREF input (wire to 5V)
}
