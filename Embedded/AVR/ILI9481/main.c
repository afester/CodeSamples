#include <avr/interrupt.h>

#include "ILI9481.h"
#include "7seg.h"
#include "encoder.h"

void displayValue(int y, int value, int color) {
   renderDigit(130, y, value % 10, color);
   value = value / 10;

   renderDigit(70, y, value % 10, color);
   value = value / 10;

   renderDigit(10, y, value % 10, color);
}


int main() {
   tftInit();
   tftClear(BLACK);

   encoderInit();

//   tftFillRect(0, 0, 479, 319, WHITE);
//   tftFillRect(1, 1, 477, 317, BLACK);
//   tftFillRect(2, 2, 475, 315, BLUE);

//   tftHLine(0, 160, 480, WHITE);
//   tftVLine(240, 0, 320, WHITE);

//   tftRect(220, 140, 40, 40, RED);

   tftDrawText("Display controller: ");
   tftDeviceCodeRead();

   sei();
   displayValue(50, 125, RED);
   displayValue(210, 6, RED);
   int value = 0;
   while(1) {
      displayValue(130, value, GREEN);
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
