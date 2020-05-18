/**
 * A simple example how to drive a Crystalfontz LC display.
 */


#include <avr/sleep.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>

#include "bitsInlineMacro.h"
#include "adc.h"
#include "cfa533.h"
#include "tools.h"


char buffer[30];

static volatile uint8_t irqCounter = 0;
static volatile uint8_t timerEvent = 0;

// ISR is called at 38 Hz
ISR(TIMER0_OVF_vect) {
   irqCounter++;
   if (irqCounter >= 30) {
      irqCounter = 0;
//      bitToggle(PORTD, PD0);
      timerEvent = 1;
   }
}

static char CLOCK[] = " |\0 /\0 -\0 \244\0";
static int clockIdx = 0;

int main() {
   CLKPR = 0b10000000; // Enable clock prescaler change
   CLKPR = 0b00000001; // slow down a bit ....

   DDRD = 0b00001001;
   PORTD = 0b00000000;

   TCCR0A = 0;
   TCCR0B = _BV(CS02) | _BV(CS00);      // start timer with clk/1024 (9,765 kHz)
   TIMSK0 = _BV(TOIE0);                 // enable timer0 overflow interrupt

   adcInit();
   cfa533Init();
   sei();
   cfa533GetVersion(buffer);
   cfa533SetContent(0, buffer);
   cfa533SetContent(1, "Hello World!");

   while(1) {
      set_sleep_mode(0); // IDLE mode
      sleep_mode();
      if (timerEvent) {
         bitToggle(PORTD, PD3);
         uint16_t value = adcRead();
         utoa(value, buffer, 10);
         strcat(buffer, CLOCK + clockIdx);
         clockIdx += 3;
         if (clockIdx > 9) {
             clockIdx = 0;
         }
         cfa533SetContent(1, buffer);
         timerEvent = 0;
      }
   }
}


#ifdef COMPLEX_SAMPLE

#define BAUD 9600
#define MYUBRR (F_CPU/16/BAUD-1)

volatile uint8_t irqCounter;
volatile uint8_t sleepCounter;

// ISR is called at 61 Hz
ISR(TIMER0_OVF_vect) {
   irqCounter++;
   if (irqCounter >= 30) {
      irqCounter = 0;
      bitToggle(PORTL, PL1);
   }
}

static char line[] = "          ";

int main() {
   bitSet(DDRL, DDL1);  /* output pin */
   bitSet(DDRB, DDB1);  /* output pin */
   bitSet(DDRB, DDB3);  /* output pin */

   TCCR0A = 0;
   TCCR0B = _BV(CS02) | _BV(CS00);      // start timer with clk/1024 (15,625 kHz)
   TIMSK0 = _BV(TOIE0);                 // enable timer0 overflow interrupt

   // Enable USART0
   /* Set baud rate */
   UBRR0H = (unsigned char)(MYUBRR>>8);
   UBRR0L = (unsigned char)MYUBRR;
   UCSR0A = 0;
   /* Set frame format: 8data, 1stop bit */
   UCSR0C = (3<<UCSZ00);
   /* Enable receiver and transmitter */
   UCSR0B = (1<<RXEN0)|(1<<TXEN0);

   while( !(UCSR0A & _BV(UDRE0)));
   UDR0 = 'X';

   lcdInit();
   sei();
   lcdPrint(0, "Hello Arduino!");
   lcdPrint(1, "Hello World!");
   lcdSetCursor();
   lcdSetCursorPos(0, 0);
   uint8_t column = 0;
   while(1) {
      set_sleep_mode(0); // IDLE mode
      sleep_mode();

      bitToggle(PORTB, PB1);
      if (sleepCounter++ > 5) { // 15) {
         sleepCounter = 0;
         bitToggle(PORTB, PB3);

         uint8_t keys = cfa533ReadKeys();

         if (keys & KP_RIGHT) {
            column++;
            lcdSetCursorPos(column, 0);
         } else if (keys & KP_LEFT) {
            column--;
            lcdSetCursorPos(column, 0);
         } else if (keys & KP_UP) {
            line[column]++;
            lcdPrint(0, line);
         } else if (keys & KP_DOWN) {
            line[column]--;
            lcdPrint(0, line);
         }
      }
   }
}
#endif

