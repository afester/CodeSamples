#include <tools.h>


static char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
void toHex(uint8_t value, char* result) {
   result[0] = hex[value >> 4];
   result[1] = hex[value & 0x0F];
   result[2] = 0;
}

