
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "bitsInlineMacro.h"
#include "mcp4811.h"

void MCP48xx_Init() {
    /* Set MOSI, SCK and CS output */
    bitSet(PORTB, PB4); // CS=1
    DDRB = _BV(DDB4) | _BV(DDB5) | _BV(DDB7);
 
    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}


void MCP48xx_SetValue(uint16_t value) {
   // CS=0
   bitClear(PORTB, PB4);

   // high byte
   SPDR = (value >> 8) | 0b00010000;
   while(!(SPSR & (1<<SPIF))) ;

   // low byte
   SPDR = value & 0xFF;
   while(!(SPSR & (1<<SPIF))) ;

   // SS=1
   bitSet(PORTB, PB4);
}
