#include <avr/io.h>
#include <avr/interrupt.h>

#include "encoder.h"

ISR(TIMER1_COMPA_vect) {
    PORTD ^= 0b00000001;  // toggle PD0
}


void encoderInit() {
   // initialize timer.
   // CLK_CPU=20000000 Hz
   // CLK_IO =20000000 Hz
   // T_CLK=19,5kHz
   OCR1A = 1953;             /* Compare match after 1953 increments (10 Hz) (16 bit register!) */
//   TCCR1A = _BV(COM1A0);      /* Toggle OC1A on compare match */
   TCCR1B = _BV(WGM12) | _BV(CS12) | _BV(CS10);   /* Mode=Clear on Compare Match; clk = Clk_IO/1024 = 19530 Hz */
   TCCR1C = 0;
   TIMSK1 = _BV(OCIE1A);

   DDRD |= 1;	// PD0 output
}


#ifdef COMPILE

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdlib.h>

#include "bitsInlineMacro.h"
#include "lcdisplay.h"
#include "mcp4811.h"


volatile int8_t encDelta;
int8_t lastStep = 3;

// ISR is called at 61 Hz
//ISR(TIMER0_OVF_vect) {
//   encoderVal = PINL & 0b00000011;
//}
   
ISR(TIMER1_COMPA_vect) {
   // read current state as gray code
   //                                BA
   uint8_t pinValue = PINL & 0b00000011;
// cw value sequence (gray code)
// BA            step
// 11(R) 00 11  10  2 => -1
// 10    00 00  01  1 => -1
// 00    00 00  00  0 => -1
// 01    00 11  11  3 => 3

   // convert to binary code
   int8_t step = 0;
   if (pinValue & 0x01) {
      step = 3;
   }
   if (pinValue & 0x02) {
      step ^= 0x01;
   }

   diff = lastStep - step;  // only -3, -1, 1 or 3 possible (assumed that no step has been skipped)
                            // (or 0 if no rotation)
   // -1: 0b11111111	cw
   //  3: 0b00000011	cw
   //  1: 0b00000001	ccw
   // -3: 0b11111101	ccw

   //  0: 0b00000000	none
   // -2: 0b11111110    skipped
   //  2: 0b00000010    skipped

   // only diff & 1 == 1 is a valid rotation
   // diff & 2 == 0 is ccw, diff & 2 == 1 is cw
   if (diff & 1) {
      lastStep = step;
      //diff & 2 => 0 (ccw) or 2 (cw)
      //(diff & 2) - 1 => -1 (ccw) or 1 (cw)
      encDelta += (diff & 2) - 1;
   }
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

   // initialize timer.
   OCR1A = 2;                /* Compare match after 2 increments (976 Hz) */
   TCCR1A = _BV(COM1A0);      /* Toggle OC1A on compare match */
   TCCR1B = _BV(WGM12) | _BV(CS12) | _BV(CS10);   /* Mode=Clear on Compare Match; clk = Clk_IO/1024 = 1953 Hz */
   TCCR1C = 0;
   TIMSK1 = _BV(OCIE1A);

   //TCCR0A = 0;
   //TCCR0B = _BV(CS02) | _BV(CS00);      // start timer with clk/1024 (15,625 kHz)
   //TIMSK0 = _BV(TOIE0);                 // enable timer0 overflow interrupt

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
#endif
