/**
 * A simple example how to drive a Crystalfontz LC display.
 */


#include <avr/sleep.h>
#include <stdbool.h>
#include "twi.h"
#include <avr/interrupt.h>
#include <string.h>
#include "bitsInlineMacro.h"



void lcdInit() {
   twi_init();
}


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


#define KP_UP 0x01
#define KP_ENTER 0x02
#define KP_CANCEL 0x04
#define KP_LEFT 0x08
#define KP_RIGHT 0x10
#define KP_DOWN 0x20

static void lcdSendReceive(CFA533Packet* pkg) {
   unsigned short CRC = get_crc(2 + pkg->length, (unsigned char*) pkg);
   pkg->data[pkg->length] = CRC & 0xFF;
   pkg->data[pkg->length + 1] = CRC >> 8;

   uint8_t result = twi_writeTo(42, (uint8_t*) pkg, pkg->length + 4, true, true);
   uint8_t nbytes = twi_readFrom(42, buffer, 20, true);
}

void lcdPrint(int row, const char* text) {
   CFA533Packet pkg;

//   pkg.command = 0x1f;
//   pkg.length = 18; // strlen(text);
//   pkg.data[0] = 0;
//   pkg.data[1] = row;
//   int i = 0;
//   for (i = 0;  i < strlen(text); i++) {
//      pkg.data[2 + i] = text[i];
//   }
//   for (  ;  i < 16; i++) {
//      pkg.data[2 + i] = ' ';
//   }

   pkg.command = 0x07 + row;
   pkg.length = 16; // strlen(text);
   int i = 0;
   for (i = 0;  i < strlen(text); i++) {
      pkg.data[i] = text[i];
   }
   for (  ;  i < 16; i++) {
      pkg.data[i] = ' ';
   }

   lcdSendReceive(&pkg);
}

static char byte[] = "xxxxxxxx";

static uint8_t cfa533ReadKeys() {
   CFA533Packet pkg;
   pkg.command = 0x18;
   pkg.length = 0;
   unsigned short CRC = get_crc(2 + pkg.length, (unsigned char*) &pkg);
   pkg.data[0] = CRC & 0xFF;
   pkg.data[0 + 1] = CRC >> 8;

   uint8_t result = twi_writeTo(42, (uint8_t*) &pkg, pkg.length + 4, true, true);
   uint8_t nbytes = twi_readFrom(42, buffer, 20, true);

   return buffer[2];
}


static void lcdSetCursor() {
   CFA533Packet pkg;
   pkg.command = 0x0C;
   pkg.length = 1;
   pkg.data[0] = 3; // blinking underscore
   unsigned short CRC = get_crc(2 + pkg.length, (unsigned char*) &pkg);
   pkg.data[pkg.length] = CRC & 0xFF;
   pkg.data[pkg.length + 1] = CRC >> 8;

   uint8_t result = twi_writeTo(42, (uint8_t*) &pkg, pkg.length + 4, true, true);
   uint8_t nbytes = twi_readFrom(42, buffer, 20, true);

}

static void lcdSetCursorPos(uint8_t column, uint8_t row) {
   CFA533Packet pkg;
   pkg.command = 0x0B;
   pkg.length = 2;
   pkg.data[0] = column;
   pkg.data[1] = row;
   unsigned short CRC = get_crc(2 + pkg.length, (unsigned char*) &pkg);
   pkg.data[pkg.length] = CRC & 0xFF;
   pkg.data[pkg.length + 1] = CRC >> 8;

   uint8_t result = twi_writeTo(42, (uint8_t*) &pkg, pkg.length + 4, true, true);
   uint8_t nbytes = twi_readFrom(42, buffer, 20, true);
}

// NOTE: Requires externally connected temperatur sensors
static void enableTempDisplay() {
   CFA533Packet pkg;
   pkg.command = 0x15;
   pkg.length = 7;
   pkg.data[0] = 0;
   pkg.data[1] = 2;
   pkg.data[2] = 0;
   pkg.data[3] = 3;
   pkg.data[4] = 0;
   pkg.data[5] = 1;
   pkg.data[6] = 0;
   unsigned short CRC = get_crc(2 + pkg.length, (unsigned char*) &pkg);
   pkg.data[pkg.length] = CRC & 0xFF;
   pkg.data[pkg.length + 1] = CRC >> 8;

   uint8_t result = twi_writeTo(42, (uint8_t*) &pkg, pkg.length + 4, true, true);
   uint8_t nbytes = twi_readFrom(42, buffer, 20, true);
}

#define BAUD 9600
#define MYUBRR (F_CPU/16/BAUD-1)

volatile uint8_t irqCounter;
volatile uint8_t sleepCounter;

#if 0
// ISR is called at 61 Hz
ISR(TIMER0_OVF_vect) {
   irqCounter++;
   if (irqCounter >= 30) {
      irqCounter = 0;
      bitToggle(PORTL, PL1);
   }
}

static char line[] = "          ";

int main() {
   bitSet(DDRL, DDL1);  /* output pin */
   bitSet(DDRB, DDB1);  /* output pin */
   bitSet(DDRB, DDB3);  /* output pin */

   TCCR0A = 0;
   TCCR0B = _BV(CS02) | _BV(CS00);      // start timer with clk/1024 (15,625 kHz)
   TIMSK0 = _BV(TOIE0);                 // enable timer0 overflow interrupt

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
   lcdPrint(0, "Hello Arduino!");
   lcdPrint(1, "Hello World!");
   lcdSetCursor();
   lcdSetCursorPos(0, 0);
   uint8_t column = 0;
   while(1) {
      set_sleep_mode(0); // IDLE mode
      sleep_mode();

      bitToggle(PORTB, PB1);
      if (sleepCounter++ > 5) { // 15) {
         sleepCounter = 0;
         bitToggle(PORTB, PB3);

         uint8_t keys = cfa533ReadKeys();

         if (keys & KP_RIGHT) {
            column++;
            lcdSetCursorPos(column, 0);
         } else if (keys & KP_LEFT) {
            column--;
            lcdSetCursorPos(column, 0);
         } else if (keys & KP_UP) {
            line[column]++;
            lcdPrint(0, line);
         } else if (keys & KP_DOWN) {
            line[column]--;
            lcdPrint(0, line);
         }
      }
   }
}
#endif
