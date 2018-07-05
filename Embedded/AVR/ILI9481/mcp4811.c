
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "bitsInlineMacro.h"
#include "mcp4811.h"

void MCP48xx_Init() {
    /* Set MOSI, SCK and CS output */
    /* NOTE: SS (PB4) MUST be output to properly enable master mode */
    bitSet(PORTB, PB3); // CS=1
    DDRB |= _BV(DDB3) | _BV(DDB4) | _BV(DDB5) | _BV(DDB7);
 
    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}


void MCP48xx_SetValue(uint16_t value) {
   value = value & 0b0000111111111111; // always use 12 bit - the lowest 2 or 4 bits are ignored for 8 and 10 bit devices

   // CS=0
   bitClear(PORTB, PB3);

   // high byte
   SPDR = (value >> 8) | 0b00010000; // 0 .. 4.096V
   while(!(SPSR & (1<<SPIF))) ;

   // low byte
   SPDR = value & 0xFF;
   while(!(SPSR & (1<<SPIF))) ;

   // SS=1
   bitSet(PORTB, PB3);
}
