#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "ILI9481.h"
#include "7seg.h"
#include "encoder.h"

void displayValue(int y, int value, int color) {
   if (value < 0) {
      tftFillRect(200, y, 20, 10, RED);
      value = -value;
   } else {
      tftFillRect(200, y, 20, 10, BLACK);
   }

   renderDigit(130, y, value % 10, color);
   value = value / 10;

   renderDigit(70, y, value % 10, color);
   value = value / 10;

   renderDigit(10, y, value % 10, color);
}

extern volatile int8_t globalStep;

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
   displayValue(50, 0, RED);
   displayValue(130, 0, GREEN);
   displayValue(210, 0, RED);

   int value1 = 0;
   int value2 = 0;
   while(1) {
      set_sleep_mode(0); // IDLE mode
      sleep_mode();

      // voltage
      int8_t diff = encoderRead1();
      if (diff != 0) {

         // If the diff is bigger than 1, then accelerate
         if (diff > 1) {
            diff = 10;
         }
         if (diff < -1) {
            diff = -10;
         }

         value1 += diff;
         if (value1 < 0) {
            value1 = 0;
         } else if (value1 > 350) {
            value1 = 350;
         }
         displayValue(130, value1, GREEN);
      }

      // maximum current
      diff = encoderRead2();
      if (diff != 0) {
         value2 += diff;
         if (value2 < 0) {
            value2 = 0;
         } else if (value2 > 300) {
            value2 = 300;
         }
         displayValue(210, value2, RED);
      }
   }
}
