#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "bitsInlineMacro.h"

void SPI_MasterInit() {
    /* Set MOSI, SCK and SS output */
    bitSet(PORTB, PB0); // SS=1
    DDRB = _BV(DDB0) | _BV(DDB1) | _BV(DDB2);
//    DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK);
 
    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<DORD);
}



void SPI_MasterTransmit(uint8_t cData) {
    /* Start transmission */
    SPDR = cData;
    /* Wait for transmission complete */
//     bitClear(PORTJ, PJ1);   // red
    while(!(SPSR & (1<<SPIF))) ;
//     bitSet(PORTJ, PJ1);   // red
}


int main() {
SPI_MasterInit();
  bitSet(DDRJ, DDJ1);  /* red - output pin */
   bitSet(DDRJ, DDJ0);  /* yellow - output pin */
   bitSet(DDRH, DDH1);  /* green - output pin */

 bitSet(PORTJ, PJ1);  /* red off */
   bitSet(PORTJ, PJ0);  /* yellow off */


   static const uint8_t reg0 = 0b00000000;
   static const uint8_t reg1 = 0b00011000;

   bitClear(PORTB, PB0); // SS=0
     bitClear(PORTH, PH1);   // green
   SPI_MasterTransmit(reg1);   
     bitClear(PORTJ, PJ0);   // yellow
   SPI_MasterTransmit(reg0);   
     bitClear(PORTJ, PJ1);   // red
   bitSet(PORTB, PB0); // SS=1
while(1);
}
