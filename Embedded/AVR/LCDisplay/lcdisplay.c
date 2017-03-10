/**
 * A simple example how to drive a Crystalfontz LC display.
 */


#include <avr/sleep.h>
#include <stdbool.h>
#include "twi.h"
#include <avr/interrupt.h>
#include <string.h>


static void lcdInit() {
   twi_init();
}

uint8_t testPkg[] = {0x07, 0x10, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x20, 0x20, 0x20, 0x20, 0x20, 0x6E, 0x86};
// uint8_t pgk[] = {0x08, 0x10, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x4D, 0x6F, 0x6F, 0x6E, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xE0, 0xA4};


uint8_t buffer[30];

unsigned short get_crc(unsigned char count,unsigned char *ptr)
  {
  unsigned short
    crc;   //Calculated CRC
  unsigned char
    i;     //Loop count, bits in byte
  unsigned char
    data;  //Current byte being shifted
  crc = 0xFFFF; // Preset to all 1's, prevent loss of leading zeros
  while(count--)
    {
    data = *ptr++;
    i = 8;
    do
      {
      if((crc ^ data) & 0x01)
        {
        crc >>= 1;
        crc ^= 0x8408;
        }
      else
        crc >>= 1;
      data >>= 1;
      } while(--i != 0);
    }
  return (~crc);
  }


typedef struct {
   unsigned char command;
   unsigned char length;
   char data[20];  // data + CRC
}CFA533Packet;


static void lcdPrint(int line, const char* text) {
  CFA533Packet pkg;
   pkg.command = 0x07 + line;
   pkg.length = 16; // strlen(text);
   int i = 0;
   for (i = 0;  i < strlen(text); i++) {
      pkg.data[i] = text[i];
   }
   for (  ;  i < 16; i++) {
      pkg.data[i] = ' ';
   }
   unsigned short CRC = get_crc(2 + pkg.length, (unsigned char*) &pkg);
   pkg.data[i] = CRC & 0xFF;
   pkg.data[i + 1] = CRC >> 8;

   uint8_t result = twi_writeTo(42, (uint8_t*) &pkg, pkg.length + 4, true, true);
//   uint8_t nbytes = twi_readFrom(42, buffer, 20, true);
}


#define BAUD 9600
#define MYUBRR (F_CPU/16/BAUD-1)



int main() {
   // Enable USART0
   /* Set baud rate */
   UBRR0H = (unsigned char)(MYUBRR>>8);
   UBRR0L = (unsigned char)MYUBRR;
   UCSR0A = 0;
   /* Set frame format: 8data, 1stop bit */
   UCSR0C = (3<<UCSZ00);
   /* Enable receiver and transmitter */
   UCSR0B = (1<<RXEN0)|(1<<TXEN0);

   while( !(UCSR0A & _BV(UDRE0)));
   UDR0 = 'X';

   lcdInit();
   sei();
   lcdPrint(0, "Hello 423!");
   lcdPrint(1, "Hello Moon!");

   while(1) {
      set_sleep_mode(0); // IDLE mode
      sleep_mode();
   }

#if 0
   bitSet(DDRL, DDL1);  // output pin
   bitSet(PORTL, PL1);  // green LED

   TCCR0A = 0;
   TCCR0B = _BV(CS02) | _BV(CS00);      // start timer with clk/1024 (15,625 kHz)
   TIMSK0 = _BV(TOIE0);                 // enable timer0 overflow interrupt

   while(1) {
      set_sleep_mode(0); // IDLE mode
      sleep_mode();

      if (doWork) {
         doWork = 0;

         // called every two seconds
         bitToggle(PORTL, PL1);		// green LED blink

         if (toggle) {
            lcdPrint("Hello"); 
         } else {
            lcdPrint("World"); 
         }
         toggle = !toggle;
      }
   }
#endif
}
