#include <avr/io.h>
#include <util/delay.h>

// Clock source: internal 8MHz RC oscillator
// Clk_IO: 1 MHz (CKDIV8 fuse)

// OC1A: Pin 7 (PA6) 
// (ATTENTION! PA6 is the MOSI pin!)

//red             yel grn red grn
//PA7 PA6 PA5 PA4 PA3 PA2 PA1 PA0
// 1   x   x   x   0   0   1   0	// 0x82

// 1   x   x   x   1   0   1   0	// 0x8a

// 0   x   x   x   0   1   1   0	// 0x06
// 0   x   x   x   0   1   1   0
// 0   x   x   x   0   1   1   0

// 0   x   x   x   1   0   1   0	// 0x0a

// 1   x   x   x   0   0   1   0	// 0x82

// 1   x   x   x   0   0   0   1	// 0x81
// 1   x   x   x   0   0   0   1
// 1   x   x   x   0   0   0   1

// uint8_t states[] = {0x82, 0x8a, 0x06, 0x06, 0x06, 0x0a, 0x82, 0x81, 0x81, 0x81};

int main() {
   DDRA |= (1 << DDA1);       /* output pin */
   DDRA |= (1 << DDA6);       /* PA6 (OC1A) = output pin */

   TCNT1 = 977;		      /* Compare match after 977 increments */
   TCCR1A = _BV(COM1A0);      /* Toggle OC1A on compare match */
   TCCR1B = _BV(WGM12) | _BV(CS12) | _BV(CS10);   /* Mode=Clear on Compare Match; clk = Clk_IO/1024 = 977 Hz */
   TCCR1C = 0;

//    uint8_t idx = 0;
   while(1) {
//       PORTA = states[idx];
//       idx = (idx + 1) % sizeof(states);
 
      PORTA &= ~(1 << PA1);   /* PORTL.1 ON */
      _delay_ms(100);        /* busy waiting */
      PORTA |= (1 << PA1);    /* PORTL.1 OFF */
      _delay_ms(100);        /* busy waiting */
   }
}
