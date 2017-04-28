#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#include "bitsInlineMacro.h"


int main() {
   bitSet(DDRJ, DDJ1);  /* red - output pin */
   bitSet(DDRJ, DDJ0);  /* yellow - output pin */
   bitSet(DDRH, DDH1);  /* green - output pin */

   bitSet(PORTJ, PJ1);  /* red off */
   bitSet(PORTJ, PJ0);  /* yellow off */
   bitSet(PORTH, PH1);  /* green off */

   while(1) {
      bitSet(PORTJ, PJ1);     
      bitSet(PORTJ, PJ0);
      bitClear(PORTH, PH1);   // green
      _delay_ms(2000);        /* busy waiting */

      bitSet(PORTH, PH1);
      bitClear(PORTJ, PJ0);  // yellow 
      _delay_ms(500);        /* busy waiting */

      bitSet(PORTJ, PJ0);    
      bitClear(PORTJ, PJ1);  // red
      _delay_ms(2000);        /* busy waiting */

      bitClear(PORTJ, PJ0);  // yellow
      _delay_ms(500);        /* busy waiting */
   }
}
