#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "encoder.h"

const uint8_t ACCEL_THRESHOLD = 20;

// channel 1
static volatile uint8_t ticks1 = 0;
volatile bool accel1 = false;
static int8_t lastStep1 = 3;
static volatile int8_t encDelta1 = 0;

// channel 2
static volatile uint8_t ticks2 = 0;
volatile bool accel2 = false;
static int8_t lastStep2 = 3;
static volatile int8_t encDelta2 = 0;

#define ENC1_A_MASK 0x01
#define ENC1_B_MASK 0x02
#define ENC1_PORT PINB

#define ENC2_A_MASK 0x02
#define ENC2_B_MASK 0x01
#define ENC2_PORT PIND


// 1kHz 
ISR(TIMER1_COMPA_vect) {
   ticks1++;
   if (ticks1 > 100) {  // TODO: is this correct?
       ticks1 = 0;
   }
   ticks2++;
   if (ticks2 > 100) {  // TODO: is this correct?
       ticks2 = 0;
   }

   // read current encoder states as gray code
   uint8_t pinValue = ENC1_PORT;

   // convert to binary code (cw sequence: 2, 1, 0, 3)
   // AB    step
   // 11(R) 10 2
   // 01    01 1
   // 00    00 0
   // 10    11 3
   int8_t step = 0;
   if (pinValue & ENC1_A_MASK) {
      step = 3;
   }
   if (pinValue & ENC1_B_MASK) {
      step ^= 0x01;
   }

   int8_t diff = lastStep1 - step;  // only -3, -1, 1 or 3 possible (assumed that no step has been skipped)
                                    // (or 0 if no rotation)

   // -1: 0b11111111 ccw
   //  3: 0b00000011 ccw
   //  1: 0b00000001 cw
   // -3: 0b11111101 cw

   //  0: 0b00000000 none
   // -2: 0b11111110 skipped
   //  2: 0b00000010 skipped

   // only diff & 1 == 1 is a valid rotation
   // diff & 2 == 0 is ccw, diff & 2 == 1 is cw
   if (diff & 1) {
      // consider time it took for the rotation
      if (ticks1 < ACCEL_THRESHOLD) {
          accel1 = true;
      } else {
          accel1 = false;
      }
      ticks1 = 0;      

      lastStep1 = step;
      // diff & 2 => 0 (cw) or 2 (ccw)
      //(diff & 2) - 1 => -1 (cw) or 1 (ccw) (!!!)
      encDelta1 -= (diff & 2) - 1;
   }

//#########################################################

   // Encoder 2
   pinValue = ENC2_PORT;
   step = 0;
   if (pinValue & ENC2_A_MASK) {
      step = 3;
   }
   if (pinValue & ENC2_B_MASK) {
      step ^= 0x01;
   }
   diff = lastStep2 - step;
   if (diff & 1) {
      // consider time it took for the rotation
      if (ticks2 < ACCEL_THRESHOLD) {
          accel2 = true;
      } else {
          accel2 = false;
      }
      ticks2 = 0;      

      lastStep2 = step;
      // diff & 2 => 0 (cw) or 2 (ccw)
      //(diff & 2) - 1 => -1 (cw) or 1 (ccw) (!!!)
      encDelta2 -= (diff & 2) - 1;
   }
}


void encoderInit() {
   // initialize timer.
   // CLK_CPU=20000000 Hz
   // CLK_IO =20000000 Hz
   // T_CLK=19,5kHz
   OCR1A = 20;             /* Compare match after 20 increments (1kHz) (16 bit register!) */
   TCCR1B = _BV(WGM12) | _BV(CS12) | _BV(CS10);   /* Mode=Clear on Compare Match; clk = Clk_IO/1024 = 19530 Hz */
   TCCR1C = 0;
   TIMSK1 = _BV(OCIE1A);

   // enable pull ups
   PORTB |= 0b00000011;
   PORTD |= 0b00000011;
}


int8_t encoderRead1() {
   cli();
   int8_t diff = encDelta1;
   encDelta1 = diff & 3;
   sei();
   diff = diff >> 2;

   if (accel1) {
       diff = diff * 10;
   }
   return diff;
}

int8_t encoderRead2() {
   cli();
   int8_t diff = encDelta2;
   encDelta2 = diff & 3;
   sei();
   diff = diff >> 2;

   if (accel2) {
       diff = diff * 10;
   }
   return diff;
}
