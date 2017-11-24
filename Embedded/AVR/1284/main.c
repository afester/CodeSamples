#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>


int main() {
   DDRD = 0b00000001;
   while(1) {
      PORTD = 0b00000001;
      _delay_ms(50);
      PORTD = 0b00000000;
      _delay_ms(50);
   }
}
