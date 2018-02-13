/**
 */

#include <stdbool.h>
#include <string.h>

#include <avr/sleep.h>  // !!!
#include "twi.h"
#include "bitsInlineMacro.h"
#include "adc.h"
#include "tools.h"


uint8_t buffer[30];

static unsigned short get_crc(unsigned char count,unsigned char *ptr)
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

enum {
   CMD_PING = 0x00,
   CMD_GETVERSION = 0x01,
   CMD_REBOOT = 0x05,
   CMD_CLEAR = 0x06,
   CMD_SETCONTENTS1 = 0x07,
   CMD_SETCONTENTS2 = 0x08,
   CMD_SETCURSORPOS = 0x0B,
   CMD_SETCURSORSTYLE = 0x0C,
   CMD_SETCONTRAST = 0x0D,
   CMD_SETBRIGHTNESS = 0x0E,
   CMD_READTEMPERATURE = 0x0F,
   CMD_POLLKEYS = 0x18};

enum {
   KP_UP = 0x01,
   KP_ENTER = 0x02,
   KP_CANCEL = 0x04,
   KP_LEFT = 0x08,
   KP_RIGHT = 0x10,
   KP_DOWN = 0x20};


static void lcdSendReceive(CFA533Packet* pkg) {
   unsigned short CRC = get_crc(2 + pkg->length, (unsigned char*) pkg);
   pkg->data[pkg->length] = CRC & 0xFF;
   pkg->data[pkg->length + 1] = CRC >> 8;

   uint8_t result = twi_writeTo(42, (uint8_t*) pkg, pkg->length + 4, false, true);
   uint8_t nbytes = twi_readFrom(42, (uint8_t*) pkg, 20, true);
//   toHex(pkg.command, dest);
//   toHex(pkg.length, dest+2);
//   toHex(pkg.data[0], dest+4);
//   toHex(pkg.data[1], dest+6);
//   toHex(pkg.data[2], dest+8);
//   toHex(pkg.data[3], dest+10);
}


void cfa533Init() {
   twi_init();
}


void cfa533SetContent(int row, const char* text) {
   CFA533Packet pkg;

#if 0
   pkg.command = 0x1f;
   pkg.length = 18; // strlen(text);
   pkg.data[0] = 0;
   pkg.data[1] = row;
   int i = 0;
   for (i = 0;  i < strlen(text); i++) {
      pkg.data[2 + i] = text[i];
   }
   for (  ;  i < 16; i++) {
      pkg.data[2 + i] = ' ';
   }
#endif

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


void cfa533Clear() {
   CFA533Packet pkg;
   pkg.command = CMD_CLEAR;
   pkg.length = 0;

   lcdSendReceive(&pkg);
}

void cfa533GetVersion(char* dest) {
   CFA533Packet pkg;
   pkg.command = CMD_GETVERSION;
   pkg.length = 0;

   lcdSendReceive(&pkg);
   memcpy(dest, pkg.data, pkg.length);
   dest[pkg.length] = 0;
}


uint8_t cfa533ReadKeys() {
   CFA533Packet pkg;
   pkg.command = 0x18;
   pkg.length = 0;

   lcdSendReceive(&pkg);
   return pkg.data[0];
}


void cfa533SetCursor() {
   CFA533Packet pkg;
   pkg.command = 0x0C;
   pkg.length = 1;
   pkg.data[0] = 3; // blinking underscore

   lcdSendReceive(&pkg);
}


void cfa533SetCursorPos(uint8_t column, uint8_t row) {
   CFA533Packet pkg;
   pkg.command = 0x0B;
   pkg.length = 2;
   pkg.data[0] = column;
   pkg.data[1] = row;

   lcdSendReceive(&pkg);
}

// NOTE: Requires externally connected temperatur sensors
void cfa533EnableTempDisplay() {
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

   lcdSendReceive(&pkg);
}
