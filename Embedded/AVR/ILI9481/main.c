#include "ILI9481.h"
//#include "small.h"
//#include "lcd.h"
#include "7seg.h"

// NOTE: it currently takes approx. 48 seconds to count from 0 to 999 using three digits!

// NOTE: 5296 bytes RAM!!!!
uint16_t digitImage[42 * 63 + 2];
Bitmap16* bitmap = &digitImage;
static const uint16_t* palette = grayPalette;

void bitBltIdx(const Bitmap8* source, Bitmap16* dest, uint16_t x, uint16_t y) {
    const uint8_t* source8 = source->bitmap;

    for(int i = 0; i < source->height; i++) {

      uint16_t* dst = dest->bitmap + ((dest->width*(y+i))+x);
      for(int m = 0; m < source->width; m++) { // x direction

        if (*source8 != 0) {  // index 0 is transparent pixel
            *dst = palette[*source8];
        }
        dst++;
        source8++;
      }
    }
}


// vertically mirrored
void bitBltIdxVM(const Bitmap8* source, Bitmap16* dest, const uint16_t x, const uint16_t y) {

    for(int i = 0; i < source->height; i++) {    // y direction
      const uint8_t* source8 = source->bitmap + (((i+1) * source->width)-1);    // row end index

      uint16_t* dst = dest->bitmap + ((dest->width*(y+i))+x);
      for(int m = 0; m < source->width; m++) { // x direction

          if (*source8 != 0) {  // index 0 is transparent pixel
            *dst = palette[*source8];
          }
          dst++;
          source8--;
      }
    }
}

// horizontally mirrored
void bitBltIdxHM(const Bitmap8* source, Bitmap16* dest, const uint16_t x, const uint16_t y) {

    for(int i = 0;  i < source->height; i++) {    // y direction
      const uint8_t* source8 = source->bitmap + (source->height - i - 1) * source->width;    // row start index

      uint16_t* dst = dest->bitmap + ((dest->width*(y+i))+x);
      for(int m = 0; m < source->width; m++) { // x direction

          if (*source8 != 0) {  // index 0 is transparent pixel
            *dst = palette[*source8];
          }
          dst++;
          source8++;
      }
    }
}


// horizontally and vertically mirrored
void bitBltIdxHVM(const Bitmap8* source, Bitmap16* dest, const uint16_t x, const uint16_t y) {

    for(int i = 0; i < source->height; i++) {    // y direction
      const uint8_t* source8 = source->bitmap + (((source->height - i) * source->width)-1);    // row end index

      uint16_t* dst = dest->bitmap + ((dest->width*(y+i))+x);
      for(int m = 0; m < source->width; m++) { // x direction

          if (*source8 != 0) {  // index 0 is transparent pixel
            *dst = palette[*source8];
          }
          dst++;
          source8--;
      }
    }
}

/****************************************************************************/

void bitBlt(const Bitmap16* source, Bitmap16* dest, uint16_t x, uint16_t y) {
    const uint16_t* source16 = source->bitmap;

    for(int i = 0; i < source->height; i++) {

      uint16_t* dst = dest->bitmap + ((dest->width*(y+i))+x);
      for(int m = 0; m < source->width; m++) { // x direction

        if (*source16 != BLACK) {  // black pixel in mask gets background color
            *dst = *source16;
        }
        dst++;
        source16++;
      }
    }
}


// vertically mirrored
void bitBltVM(const Bitmap16* source, Bitmap16* dest, const uint16_t x, const uint16_t y) {

    for(int i = 0; i < source->height; i++) {    // y direction
      const uint16_t* source16 = source->bitmap + (((i+1) * source->width)-1);    // row end index

      uint16_t* dst = dest->bitmap + ((dest->width*(y+i))+x);
      for(int m = 0; m < source->width; m++) { // x direction

          if (*source16 != BLACK) {  // white pixel in mask gets background color
              *dst = *source16;
          }
          dst++;
          source16--;
      }
    }
}

// horizontally mirrored
void bitBltHM(const Bitmap16* source, Bitmap16* dest, const uint16_t x, const uint16_t y) {

    for(int i = 0;  i < source->height; i++) {    // y direction
      const uint16_t* source16 = source->bitmap + (source->height - i - 1) * source->width;    // row start index

      uint16_t* dst = dest->bitmap + ((dest->width*(y+i))+x);
      for(int m = 0; m < source->width; m++) { // x direction

          if (*source16 != BLACK) {  // white pixel in mask gets background color
              *dst = *source16;
          }
          dst++;
          source16++;
      }
    }
}


// horizontally and vertically mirrored
void bitBltHVM(const Bitmap16* source, Bitmap16* dest, const uint16_t x, const uint16_t y) {

    for(int i = 0; i < source->height; i++) {    // y direction
      const uint16_t* source16 = source->bitmap + (((source->height - i) * source->width)-1);    // row end index

      uint16_t* dst = dest->bitmap + ((dest->width*(y+i))+x);
      for(int m = 0; m < source->width; m++) { // x direction

          if (*source16 != BLACK) {  // white pixel in mask gets background color
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
    clearBitmap(bitmap, BLACK);

    palette = grayPalette;
    if (mask & 0b00000001) {
       palette = greenPalette;
    }
        bitBltIdx(&adRedBlack, bitmap, 4, 0);      // a
        //bitBlt(&adSegment, bitmap, 4, 0);      // a
        //tftBlt(&adSegment, 4, 0);      // a
    //}

    palette = grayPalette;
    if (mask & 0b00000010) {
       palette = greenPalette;
    }
        bitBltIdxVM(&bcefRedBlack, bitmap, 34, 2);   // b
        //bitBltVM(&bcefSegment, bitmap, 34, 2);   // b
        //tftBltVM(&bcefSegment, 34, 2);   // b
    //}
    palette = grayPalette;
    if (mask & 0b00000100) {
       palette = greenPalette;
    }
        bitBltIdxHVM(&bcefRedBlack, bitmap, 34, 31); // c
        //bitBltHVM(&bcefSegment, bitmap, 34, 31); // c
        //tftBltHVM(&bcefSegment, 34, 31); // c
    //}
    palette = grayPalette;
    if (mask & 0b00001000) {
       palette = greenPalette;
    }
        bitBltIdxHM(&adRedBlack, bitmap, 4, 55);    // d
        //bitBltHM(&adSegment, bitmap, 4, 55);    // d
        //tftBltHM(&adSegment, 4, 55);    // d
    //}
    palette = grayPalette;
    if (mask & 0b00010000) {
       palette = greenPalette;
    }
        bitBltIdxHM(&bcefRedBlack, bitmap, 0, 31);  // e
        //bitBltHM(&bcefSegment, bitmap, 0, 31);  // e
        //tftBltHM(&bcefSegment, 0, 31);  // e
    //}
    palette = grayPalette;
    if (mask & 0b00100000) {
       palette = greenPalette;
    }
        bitBltIdx(&bcefRedBlack, bitmap, 0, 2);  // f
        //bitBlt(&bcefSegment, bitmap, 0, 2);  // f
        //tftBlt(&bcefSegment, 0, 2);  // f
    //}
    palette = grayPalette;
    if (mask & 0b01000000) {
       palette = greenPalette;
    }
        bitBltIdx(&gRedBlack, bitmap, 6, 27);      // g
        //bitBlt(&gSegment, bitmap, 6, 27);      // g
        //tftBlt(&gSegment, 6, 27);      // g
    //}
}

void displayValue(int y, int value) {
   createSegment(value % 10);
   tftBlt(bitmap, 110, y);
   value = value / 10;

   createSegment(value % 10);
   tftBlt(bitmap, 60, y);
   value = value / 10;

   createSegment(value % 10);
   tftBlt(bitmap, 10, y);
}


int main() {
   tftInit();
   tftClear(BLACK);

//   tftFillRect(0, 0, 479, 319, WHITE);
//   tftFillRect(1, 1, 477, 317, BLACK);
//   tftFillRect(2, 2, 475, 315, BLUE);

//   tftHLine(0, 160, 480, WHITE);
//   tftVLine(240, 0, 320, WHITE);

//   tftRect(220, 140, 40, 40, RED);

   tftDrawText("Display controller: ");
   tftDeviceCodeRead();

   bitmap->width = 42;
   bitmap->height = 63;

   displayValue(50, 125);
   displayValue(210, 6);
   int value = 0;
   while(1) {
      displayValue(130, value);
      value++;
   }

 //  for (int x = 10;  x < 200; x+=2) {
//      tftDrawPixel(x, 200, RED);
//   }

//   tftDrawChar('\n'); 
//   for (unsigned short c = 0;  c <= 255;  c++) {
//       tftDrawChar(c); 
//   }

   while(1);
}
