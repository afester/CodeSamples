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

   // set first state of LEDs
   uint8_t idx = 0;
   PORTA = (PORTA & portMask) | states[idx];
   idx++;

   uint8_t loops = 0;
   uint8_t wakeup = 0;
   uint8_t mode = 0;
   int keyOld = 1;  // assume pressed - from the "power on" step

   sei();
   while(1) {
      set_sleep_mode(0); // IDLE mode
      sleep_mode();

      // called every 100ms

      // read key and check if it has been pressed since the last scan
      int key = !(PINB & _BV(PORTB2));
      if (key && !keyOld) {
         mode = mode + 1;
         if (mode > 2) {
            mode = 0;
        }
      }
      keyOld = key;

      wakeup++;
      switch(mode) {
         case 0 :
            if (wakeup > 10) {
               wakeup = 0;

               // called every 1s
               PORTA = (PORTA & portMask) | states[idx];
               idx++;
               if (idx >= sizeof(states)) {
                  idx = 0;
               }
               loops++;
            }
            break;

         case 1 : 
            if (wakeup > 5) {
               wakeup = 0;

               // called every 500ms
	       PORTA = (PORTA ^ 0b00001000) | 0b10000111;
               loops++;
            }
            break;

         case 2 :
            PORTB = 0b00000100;	// keep PB2 pullup enable, disable power (PB0)
            break;
      }

      if (loops > 20) {
        PORTB = 0;  // turn off power
      }
   }
}
