#include <ILI9481.h>

Bitmap8 adRedBlack = {34, 8,
  {0x00, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 
   0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 
   0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 
   0x04, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 
   0x00, 0x04, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 0x00, 
   0x00, 0x00, 0x04, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x04, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x04, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00}};

Bitmap8 bcefRedBlack = {8, 29,
  {0x00, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x04, 0x02, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 
   0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 0x00, 0x00, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 0x00, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 
   0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 0x00, 
   0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 0x00, 0x00, 
   0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00}};

Bitmap8 gRedBlack = {30, 8,
  {0x00, 0x00, 0x00, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 0x00, 
   0x00, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 
   0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 
   0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 
   0x00, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 
   0x00, 0x00, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 0x00, 0x00}};

uint16_t redPalette[]   = {0x0000, 0x2900, 0x1f00, 0x1000, 0x0200};
uint16_t grayPalette[]  = {0x0000, 0x8210, 0xa210, 0x6108, 0x0000};
uint16_t greenPalette[] = {0x0000, 0x4702, 0xfb07, 0xed03, 0x0000};