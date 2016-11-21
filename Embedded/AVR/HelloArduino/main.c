#include <avr/io.h>
#include <util/delay.h>

int main() {
   DDRL |= (1 << DDA1);       /* output pin */

   while(1) {
      PORTL &= ~(1 << PA1);   /* PORTL.1 ON */
      _delay_ms(1000);        /* busy waiting */
      PORTL |= (1 << PA1);    /* PORTL.1 OFF */
      _delay_ms(1000);        /* busy waiting */
   }
}
