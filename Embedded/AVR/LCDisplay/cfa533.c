#include <stdio.h>
#include <string.h>


typedef struct {
   unsigned char command;
   unsigned char length;
   char data[20];  // data + CRC
}CFA533Packet;


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

int main() {
   const char* text = "Hello Moon      ";

   CFA533Packet pkg;
   pkg.command = 0x08;
   pkg.length = strlen(text);
   int i = 0;
   for (i = 0;  i < pkg.length; i++) {
      pkg.data[i] = text[i];
   }  
   unsigned short CRC = get_crc(2 + pkg.length, (unsigned char*) &pkg);
   pkg.data[i] = CRC & 0xFF;
   pkg.data[i + 1] = CRC >> 8;

   printf("uint8_t pgk[] = {");
   const unsigned char* buf = (const unsigned char*) &pkg;
   for (int idx = 0;  idx < 4 + pkg.length;  idx++) {
      if (idx > 0) {
          printf(", ");
      }
      printf("0x%02X", buf[idx]);
   }
   printf("};\n");
   return 0;
}
