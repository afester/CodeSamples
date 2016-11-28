#include <avr/io.h>
#include <util/delay.h>

#include "bitsInlineMacro.h"


int main() {
   bitSet(DDRL, DDL1);  /* output pin */
   bitSet(DDRB, DDB1);  /* output pin */
   bitSet(DDRB, DDB3);  /* output pin */

   bitSet(PORTL, PL1);  /* toggle PORTL.1 */
   bitSet(PORTB, PB1);  /* toggle PORTL.1 */
   bitSet(PORTB, PB3);  /* toggle PORTL.1 */

   while(1) {
      bitSet(PORTB, PB1);
      bitSet(PORTB, PB3);
      bitClear(PORTL, PL1);
      _delay_ms(2000);        /* busy waiting */

      bitSet(PORTL, PL1);
      bitClear(PORTB, PB3);
      _delay_ms(500);        /* busy waiting */

      bitSet(PORTB, PB3);
      bitClear(PORTB, PB1);
      _delay_ms(2000);        /* busy waiting */

      bitClear(PORTB, PB3);
      _delay_ms(500);        /* busy waiting */
   }
}
