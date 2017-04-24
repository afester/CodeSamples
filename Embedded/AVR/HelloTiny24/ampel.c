#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

// Clock source: internal 8MHz RC oscillator
// Clk_IO: 1 MHz (CKDIV8 fuse)

// OC1A: Pin 7 (PA6) 
// (ATTENTION! PA6 is the MOSI pin!)

const uint8_t states[] = {
//   r---ygrg (red, yellow, green ; red, green)
// PA76543210
   0b00001101,	// red/red
   0b00000101,	// red-yellow/red
   0b10001001,	// green/red
   0b10001001,
   0b10001001,
   0b10000101,	// yellow/red
   0b00001101,  // red/red
   0b00001110,  // red/green
   0b00001110,
   0b00001110
};

const uint8_t portMask = 0b01110000;

ISR(TIM1_COMPA_vect) {
}

int main() {
   DDRA = 0b10001111;

   OCR1A = 977;		      /* Compare match after 977 increments */
   TCCR1A = _BV(COM1A0);      /* Toggle OC1A on compare match */
   TCCR1B = _BV(WGM12) | _BV(CS12) | _BV(CS10);   /* Mode=Clear on Compare Match; clk = Clk_IO/1024 = 977 Hz */
   TCCR1C = 0;
   TIMSK1 = _BV(OCIE1A);

   uint8_t idx = 0;
   sei();
   while(1) {
      set_sleep_mode(0); // IDLE mode
      sleep_mode();
 
      // the following code is executed once a second
      PORTA = (PORTA & portMask) | states[idx];
      idx = (idx + 1) % sizeof(states);
   }
}
