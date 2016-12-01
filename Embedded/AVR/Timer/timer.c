#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "bitsInlineMacro.h"

volatile uint8_t irqCounter;

// ISR is called at 61 Hz
ISR(TIMER0_OVF_vect) {
   irqCounter++;
   if (irqCounter >= 30) {
      irqCounter = 0;
      bitToggle(PORTL, PL1);
   }
}

int main() {
   bitSet(DDRL, DDL1);  /* output pin */
   bitSet(DDRB, DDB1);  /* output pin */
   bitSet(DDRB, DDB3);  /* output pin */

   bitSet(PORTL, PL1);
   bitSet(PORTB, PB1);
   bitSet(PORTB, PB3);

   TCCR0A = 0;
   TCCR0B = _BV(CS02) | _BV(CS00);	// start timer with clk/1024 (15,625 kHz)
   TIMSK0 = _BV(TOIE0);			// enable timer0 overflow interrupt

   sei();	// enable interrupts
   while(1) {
   }
}
