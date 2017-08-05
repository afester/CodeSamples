#include "ILI9481.h"
//#include "small.h"
#include "lcd.h"



uint16_t digitImage[42 * 63 + 2];
Bitmap16* bitmap = &digitImage;

void bitBlt(const Bitmap16* source, Bitmap16* dest, uint16_t x, uint16_t y) {
    uint16_t* source16 = source->bitmap;

    for(int i = 0; i < source->height; i++) {

      uint16_t* dst = dest->bitmap + ((dest->width*(y+i))+x);

      for(int m = 0; m < source->width; m++) { // x direction

        if (*source16 != WHITE) {  // white pixel in mask gets background color
            *dst = *source16;
        }
        dst++;
        source16++;
      }
    }
}


// vertically mirrored
void bltMask2(const Bitmap16* source, Bitmap16* dest, const uint16_t x, const uint16_t y) {

    for(int i = 0; i < source->height; i++) {    // y direction
      const uint16_t* source16 = source->bitmap + (((i+1) * source->width)-1);    // row end index

      uint16_t* dst = dest->bitmap + ((dest->width*(y+i))+x);
      for(int m = 0; m < source->width; m++) { // x direction

          if (*source16 != WHITE) {  // white pixel in mask gets background color
              *dst = *source16;
          }
          dst++;
          source16--;
      }
    }
}

// horizontally mirrored
void bitBlt3(const Bitmap16* source, Bitmap16* dest, const uint16_t x, const uint16_t y) {

    for(int i = 0;  i < source->height; i++) {    // y direction
      const uint16_t* source16 = source->bitmap + (source->height - i - 1) * source->width;    // row start index

      uint16_t* dst = dest->bitmap + ((dest->width*(y+i))+x);
      for(int m = 0; m < source->width; m++) { // x direction

          if (*source16 != WHITE) {  // white pixel in mask gets background color
              *dst = *source16;
          }
          dst++;
          source16++;
      }
    }
}


// horizontally and vertically mirrored
void bitBlt4(const Bitmap16* source, Bitmap16* dest, const uint16_t x, const uint16_t y) {

    for(int i = 0; i < source->height; i++) {    // y direction
      const uint16_t* source16 = source->bitmap + (((source->height - i) * source->width)-1);    // row end index

      uint16_t* dst = dest->bitmap + ((dest->width*(y+i))+x);
      for(int m = 0; m < source->width; m++) { // x direction

          if (*source16 != WHITE) {  // white pixel in mask gets background color
              *dst = *source16;
          }
          dst++;
          source16--;
      }
    }
}


void clearBitmap(Bitmap16* dest, uint16_t color) {
    uint16_t* writer = dest->bitmap;
    for(int i = 0; i < dest->height; i++) {
      for(int m = 0; m < dest->width; m++) {
        *writer++ = color;
      }
    }

}

const uint8_t segments[] = {
    // gfedcba
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b11111111, // 8
    0b01101111, // 9
};

void createSegment(uint8_t value) {
    uint8_t mask = segments[value];
    clearBitmap(bitmap, WHITE);

    if (mask & 0b00000001) {
        bitBlt(&adSegment, bitmap, 4, 0);      // a
    }

    if (mask & 0b00000010) {
        bitBlt(&bcefSegment, bitmap, 34, 2);   // b
    }
    if (mask & 0b00000100) {
        bitBlt3(&bcefSegment, bitmap, 34, 31); // c
    }
    if (mask & 0b00001000) {
        bitBlt3(&adSegment, bitmap, 4, 55);    // d
    }
    if (mask & 0b00010000) {
        bitBlt4(&bcefSegment, bitmap, 0, 31);  // e
    }
    if (mask & 0b00100000) {
        bltMask2(&bcefSegment, bitmap, 0, 2);  // f
    }
    if (mask & 0b01000000) {
        bitBlt(&gSegment, bitmap, 6, 27);      // g
    }
}


int main() {
   tftInit();

   tftClear(WHITE);

//   tftFillRect(0, 0, 479, 319, WHITE);
//   tftFillRect(1, 1, 477, 317, BLACK);
//   tftFillRect(2, 2, 475, 315, BLUE);

//   tftHLine(0, 160, 480, WHITE);
//   tftVLine(240, 0, 320, WHITE);

//   tftRect(220, 140, 40, 40, RED);

   bitmap->width = 42;
   bitmap->height = 63;

   createSegment(0);
   tftBlt2(bitmap, 0, 50);
   createSegment(1);
   tftBlt2(bitmap, 45, 50);
   createSegment(2);
   tftBlt2(bitmap, 90, 50);
   createSegment(3);
   tftBlt2(bitmap, 135, 50);
   createSegment(4);
   tftBlt2(bitmap, 180, 50);
   createSegment(5);
   tftBlt2(bitmap, 225, 50);
   createSegment(6);
   tftBlt2(bitmap, 270, 50);
   createSegment(7);
   tftBlt2(bitmap, 315, 50);
   createSegment(8);
   tftBlt2(bitmap, 360, 50);
   createSegment(9);
   tftBlt2(bitmap, 405, 50);

 //  for (int x = 10;  x < 200; x+=2) {
//      tftDrawPixel(x, 200, RED);
//   }

   tftDrawText("Display controller: ");
   tftDeviceCodeRead();
//   tftDrawChar('\n'); 
//   for (unsigned short c = 0;  c <= 255;  c++) {
//       tftDrawChar(c); 
//   }

   while(1);
}
