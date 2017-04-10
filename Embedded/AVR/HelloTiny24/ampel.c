#include <avr/io.h>
#include <util/delay.h>

int main() {
   DDRA |= (1 << DDA1);       /* output pin */

   while(1) {
      PORTA &= ~(1 << PA1);   /* PORTL.1 ON */
      _delay_ms(100);        /* busy waiting */
      PORTA |= (1 << PA1);    /* PORTL.1 OFF */
      _delay_ms(100);        /* busy waiting */
   }
}
