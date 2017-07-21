#include "ILI9481.h"
#include "small.h"


int main() {
   tftInit();

   tftFillRect(0, 0, 479, 319, WHITE);
   tftFillRect(1, 1, 477, 317, BLACK);
   tftFillRect(2, 2, 475, 315, BLUE);

   tftHLine(0, 160, 480, WHITE);
   tftVLine(240, 0, 320, WHITE);

   tftRect(220, 140, 40, 40, RED);

   tftBlt(small_gray, 50, 50, 32, 32);

   while(1);
}

