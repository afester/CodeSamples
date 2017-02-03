#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "bitsInlineMacro.h"

volatile uint8_t irqCounter;
volatile uint8_t sleepCounter;

#define BAUD 9600
#define MYUBRR (F_CPU/16/BAUD-1)

// ISR is called at 61 Hz
ISR(TIMER0_OVF_vect) {
   irqCounter++;
   if (irqCounter >= 30) {
      irqCounter = 0;
      bitToggle(PORTL, PL1);
   }
}

// RXD0
// TXD0

int main() {
   bitSet(DDRL, DDL1);  /* output pin */
   bitSet(DDRB, DDB1);  /* output pin */
   bitSet(DDRB, DDB3);  /* output pin */

   bitSet(PORTL, PL1);	// green LED
   bitSet(PORTB, PB1);  // red LED
   bitSet(PORTB, PB3);  // yellow LED

   TCCR0A = 0;
   TCCR0B = _BV(CS02) | _BV(CS00);	// start timer with clk/1024 (15,625 kHz)
   TIMSK0 = _BV(TOIE0);			// enable timer0 overflow interrupt

   // Enable USART0
   /* Set baud rate */
   UBRR0H = (unsigned char)(MYUBRR>>8);
   UBRR0L = (unsigned char)MYUBRR;
   UCSR0A = 0;
   /* Set frame format: 8data, 1stop bit */
   UCSR0C = (3<<UCSZ00);
   /* Enable receiver and transmitter */
   UCSR0B = (1<<RXEN0)|(1<<TXEN0);

   sei();	// enable interrupts
   while(1) {
      while( !(UCSR0A & _BV(RXC0)));
      unsigned char data = UDR0;

      // echo
      while( !(UCSR0A & _BV(UDRE0)));
      UDR0 = data;

      if (data == '1') {
        bitToggle(PORTB, PB1);
      } else if (data == '2') {
        bitToggle(PORTB, PB3);
      }

//      // echo
//      while( !(UCSR0A & _BV(UDRE0)));
//      UDR0 = '\r';
//
//      while( !(UCSR0A & _BV(UDRE0)));
//      UDR0 = '\n';
   }

//   while(1) {
//      set_sleep_mode(0); // IDLE mode
//      sleep_mode();
//      bitToggle(PORTB, PB1);
//      if (sleepCounter++ > 15) {
//         sleepCounter = 0;
//         bitToggle(PORTB, PB3);
//      }
//   }
}
