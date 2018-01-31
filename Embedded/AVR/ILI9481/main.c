#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "ILI9481.h"
#include "7seg.h"
#include "encoder.h"
#include "mcp4811.h"
#include "adc.h"

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
static uint16_t values[] = {0, 0, 0, 0};
static int valuePtr = 0;

int main() {
   // CLKPR = 0b10000000; // Enable clock prescaler change
   // CLKPR = 0b00000100; // slow down a bit ....

   tftInit();
   tftClear(BLACK);

   encoderInit();
   MCP48xx_Init();
   adcInit();

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
         MCP48xx_SetValue(value1 * 10);
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
      }

      //values[valuePtr++] = adcRead();
      //valuePtr &= 0x03;
      //uint16_t current = (values[0] + values[1] + values[2] + values[3]) >> 2;
      uint16_t current = adcRead();
      displayValue(210, current, RED);
   }
}
