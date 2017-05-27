#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdlib.h>

#include "bitsInlineMacro.h"
#include "lcdisplay.h"
#include "mcp4811.h"


volatile uint8_t encoderVal;
uint8_t prevEncoder = 3;

// ISR is called at 61 Hz
ISR(TIMER0_OVF_vect) {
   encoderVal = PINL & 0b00000011;
}
   

static char line[] = "                ";

int main() {
   bitSet(DDRJ, DDJ1);  /* red - output pin */
   bitSet(DDRJ, DDJ0);  /* yellow - output pin */
   bitSet(DDRH, DDH1);  /* green - output pin */

   bitSet(PORTJ, PJ1);  /* red off */
   bitSet(PORTJ, PJ0);  /* yellow off */
   bitSet(PORTH, PH1);  /* green off */

   DDRL =  0b00000000;  /* PL0 .. PL7 input */
   PORTL = 0b11111111;  /* PL0 .. PL7 pullup enable */

   TCCR0A = 0;
   TCCR0B = _BV(CS02) | _BV(CS00);      // start timer with clk/1024 (15,625 kHz)
   TIMSK0 = _BV(TOIE0);                 // enable timer0 overflow interrupt

SPI_MasterInit();
   int value = 0;
   int prevValue = 0;
   lcdInit();
   sei();
   lcdPrint(0, "Hello Arduino!");
   while(1) {
      set_sleep_mode(0); // IDLE mode
      sleep_mode();

      if (prevEncoder == 2 && encoderVal == 0) {
         value+=10;
         if (value > 1000) {
            value = 1000;
         }
         bitClear(PORTJ, PJ1);  /* red on */
      } else {
         bitSet(PORTJ, PJ1);  /* red off */
      }

      if (prevEncoder == 1 && encoderVal == 0) {
         value-=10;
         if (value < 0) {
            value = 0;
         }
         bitClear(PORTJ, PJ0);  /*yellow on */
      } else {
         bitSet(PORTJ, PJ0);  /* yellow off */
      }

      if (prevValue != value) {
         itoa(value, line, 10);
         lcdPrint(0, line);
         MCP48xx_SetValue(value);
      }
      prevValue = value;
   
#if 0
      if ( (prevEncoder == 3 && encoderVal == 2) ||
           (prevEncoder == 2 && encoderVal == 0) ||
           (prevEncoder == 0 && encoderVal == 1) ||
           (prevEncoder == 1 && encoderVal == 3)) {
         bitClear(PORTJ, PJ1);  /* red on */
      } else {
         bitSet(PORTJ, PJ1);  /* red off */
      }

      if ( (prevEncoder == 3 && encoderVal == 1) ||
           (prevEncoder == 1 && encoderVal == 0) ||
           (prevEncoder == 0 && encoderVal == 2) ||
           (prevEncoder == 2 && encoderVal == 3)) {
         bitClear(PORTJ, PJ0);  /*yellow on */
      } else {
         bitSet(PORTJ, PJ0);  /* yellow off */
      }
#endif 
      prevEncoder = encoderVal;
   }
}
