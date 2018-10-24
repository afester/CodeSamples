#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <string.h>
#include <stdlib.h>

#include "ILI9481.h"
// #include "7seg.h"
#include "encoder.h"
#include "mcp4811.h"
#include "adc.h"
#include "../LCDisplay/cfa533.h"
#include <avr/pgmspace.h>

#if 0
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
#endif

extern volatile int8_t globalStep;
// static uint16_t values[] = {0, 0, 0, 0};
// static int valuePtr = 0;
static char buffer[30];
// static int wakeup = 0;

/**
 * Format a value from 0 .. 999 into a string in the format
 * "nn,n"
 */
void format(int value, char* result) { // , int decimals) {
    static char convert[10];
    itoa(value, convert, 10);    // "x", "xx", "xxx"
    int len = strlen(convert);      // 1, 2, 3

    int idx = 0;
    if (len == 1) {
        result[idx++] = ' ';
        result[idx++] = '0';
        result[idx++] = ',';
        result[idx++] = convert[0];
    } else if (len == 2) {
        result[idx++] = ' ';
        result[idx++] = convert[0];
        result[idx++] = ',';
        result[idx++] = convert[1];
    } else if (len == 3) {
        result[idx++] = convert[0];
        result[idx++] = convert[1];
        result[idx++] = ',';
        result[idx++] = convert[2];
    }
    buffer[4] = 0;
}


int main() {
//   CLKPR = 0b10000000; // Enable clock prescaler change
   // CLKPR = 0b00000100; // slow down a bit ....
//   CLKPR = 0b00000001; // slow down a bit ....

   tftInit();
   tftClear(BLACK);

//   cfa533Init();
   encoderInit();
//   MCP48xx_Init();
//   adcInit();

//   tftDrawText("Display controller: ");
//   tftDeviceCodeRead();

   int ypos = 10;
   tftDrawText(5, ypos, "U");
   tftDrawText(80, ypos, ":"); //  18,5 V");
   ypos += 50;
   tftDrawText(5, ypos, "U5");
   tftDrawText(80, ypos, ": 4,9 V");
   ypos += 50;
   tftDrawText(5, ypos, "U12");
   tftDrawText(80, ypos, ": 12,1 V");
   ypos += 50;
   tftDrawText(5, ypos, "I");
   tftDrawText(80, ypos, ": 0,05 A");
   tftDrawText(260, ypos, "I: ");
   ypos += 50;
   tftDrawText(5, ypos, "I5");
   tftDrawText(80, ypos, ": 0,01 A");
   ypos += 50;
   tftDrawText(5, ypos, "I12");
   tftDrawText(80, ypos, ": 0,02 A");
   tftDrawText(260, ypos, "T: 42 °C");

   sei();
//   displayValue(50, 0, RED);
//   displayValue(130, 0, GREEN);
//   displayValue(210, 0, RED);

   int value1 = 0;
   int value2 = 0;
   while(1) {
      set_sleep_mode(0); // IDLE mode
      sleep_mode();

      // voltage
      int8_t diff = encoderRead1();
      if (diff != 0) {
#if 0
         // If the diff is bigger than 1, then accelerate
         if (diff > 1) {
            diff = 10;
         }
         if (diff < -1) {
            diff = -10;
         }
#endif
         value1 += diff;
         if (value1 < 0) {
            value1 = 0;
         } else if (value1 > 350) {
            value1 = 350;
         }

         format(value1, buffer);
         strcat(buffer, " V    ");
         uint16_t xEnd = tftDrawText(98, 10, buffer);
         tftFillRect(xEnd, 10, 300-xEnd, 45, RED);
//         MCP48xx_SetValue(value1 * 10);
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

         format(value2, buffer);
         strcat(buffer, " A    ");
         tftDrawText(290, 160, buffer);
//         MCP48xx_SetValue(value2 * 10);
      }

#if 0
      wakeup++;
      if (wakeup > 100) {
         wakeup = 0;
         //values[valuePtr++] = adcRead();
         //valuePtr &= 0x03;
         //uint16_t current = (values[0] + values[1] + values[2] + values[3]) >> 2;
         uint16_t voltage = adcRead();

#if 0
         voltage = (voltage * 11) >> 5; // / 2,9 => 0 .. 352
         format(voltage, buffer);
         strcat(buffer, " V");
#endif
         displayValue(210, voltage, RED);
      }
#endif
   }
}
