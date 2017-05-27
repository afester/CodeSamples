#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "bitsInlineMacro.h"

void SPI_MasterInit() {
    /* Set MOSI, SCK and SS output */
    bitSet(PORTB, PB0); // SS=1
    DDRB = _BV(DDB0) | _BV(DDB1) | _BV(DDB2);
 
    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}


void MCP48xx_SetValue(uint16_t value) {
   // SS=0
   bitClear(PORTB, PB0);

   // high byte
   SPDR = (value >> 8) | 0b00010000;
   while(!(SPSR & (1<<SPIF))) ;

   // low byte
   SPDR = value & 0xFF;
   while(!(SPSR & (1<<SPIF))) ;

   // SS=1
   bitSet(PORTB, PB0);
}

