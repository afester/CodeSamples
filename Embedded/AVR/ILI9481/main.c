#include "ILI9481.h"
//#include "small.h"
#include "lcd.h"


int main() {
   tftInit();

   tftClear(BLACK);

//   tftFillRect(0, 0, 479, 319, WHITE);
//   tftFillRect(1, 1, 477, 317, BLACK);
//   tftFillRect(2, 2, 475, 315, BLUE);

//   tftHLine(0, 160, 480, WHITE);
//   tftVLine(240, 0, 320, WHITE);

//   tftRect(220, 140, 40, 40, RED);

   tftBlt(small_gray, 50, 50, 53, 64);
   tftBlt(small_gray, 105, 50, 53, 64);

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
