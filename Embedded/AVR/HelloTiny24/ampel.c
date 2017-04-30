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
volatile uint8_t pcint1 = 0;

ISR(TIM1_COMPA_vect) {
}

ISR(PCINT1_vect) {
   pcint1 = 1;
}


int main() {
   DDRB =  0b00000001;
   PORTB = 0b00000101;	// PB2 pullup enable, enable power (PB0)

   PORTA = 0b10001111;	// all LEDs off
   DDRA =  0b10001111;

   // initialize timer.
   // timer runs at 977 Hz - at 977, an interrupt is generated
   // to wakeup the device
   OCR1A = 98;		      /* Compare match after 98 increments */
   TCCR1A = _BV(COM1A0);      /* Toggle OC1A on compare match */
   TCCR1B = _BV(WGM12) | _BV(CS12) | _BV(CS10);   /* Mode=Clear on Compare Match; clk = Clk_IO/1024 = 977 Hz */
   TCCR1C = 0;
   TIMSK1 = _BV(OCIE1A);

   // initialize pin change interrupt
   // PIN change interrupts are not useful when reading non-debounced keys
//   PCMSK1 = _BV(PCINT10);	// PIN5
 //  GIMSK = _BV(PCIE1);		// enable PC1 interrupts

   // set first state of LEDs
   uint8_t idx = 0;
   PORTA = (PORTA & portMask) | states[idx];
   idx++;

   uint8_t loops = 0;
   sei();
   while(1) {
      set_sleep_mode(0); // IDLE mode
      sleep_mode();

//      if (pcint1) {	// port change interrupt
//         pcint1 = 0;
//         PORTA ^= 0b00000001;
//      } else {		// timer interrupt
         // the following code is executed once a second
         PORTA = (PORTA & portMask) | states[idx];
         idx++;
         if (idx >= sizeof(states)) {
            idx = 0;
            loops++;
         }

         if (loops > 2) {
           PORTB = 0;  // turn off power
         }
//      }
   }
}
