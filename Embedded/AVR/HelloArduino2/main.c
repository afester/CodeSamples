#include <avr/io.h>
#include <util/delay.h>

#include "bitsInlineMacro.h"


int main() {
   bitSet(DDRL, DDA1);  /* output pin */

   while(1) {
      bitToggle(PORTL, PA1);  /* toggle PORTL.1 */
      _delay_ms(1000);        /* busy waiting */
   }
}
