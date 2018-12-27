#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "ILI9481.h"
// #include "7seg.h"
#include "encoder.h"
#include "mcp4811.h"
#include "adc.h"
#include "../LCDisplay/cfa533.h"
#include <avr/pgmspace.h>

//extern volatile int16_t timeItTook;
extern volatile bool accel;

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
static uint16_t values[] = {0, 0, 0, 0};
static int valuePtr = 0;
static char buffer[30];
static int wakeup = 0;

/**
 * Formats a numeric value into a string using a specified number of decimal places.
 *
 * @param value
 * @param decimals
 * @param result
 */
void strFormat(unsigned int value, int decimals, char* result) {
    // convert value to string
    static char buffer[10];
    itoa(value, buffer, 10);    // n, nn, nnn, ...
    size_t digitCount = strlen(buffer);

    // calculate decimal point position.
    // A value < 1 shows that at least one leading zero needs to be prepended.
    int decPos = digitCount - decimals;

    // calculate number of leading zeroes
    size_t zeroes = 0;
    if (decPos < 1) {
        zeroes = 1 - decPos;
        decPos = 1;
    }

    // digitCount: number of digits in the original number
    // zeroes: number of leading zeroes in result
    // decPos: index of decimal point in result
    // zeroes + digitCount:  // overall number of digits in result (excl. decimal point)

    // create final result - fill with zeroes, add decimal point, add original digits
    int dest = 0;
    int idx = 0;
    while(digitCount > 0) {
        if (zeroes > 0) {
            result[dest++] = '0';
            zeroes--;
        } else{
            result[dest++] = buffer[idx++];
            digitCount--;
        }
        if (dest == decPos && digitCount > 0) {
            result[dest++] = ',';
        }
    }
    result[dest] = 0;
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
   adcInit();

//   tftDrawText("Display controller: ");
//   tftDeviceCodeRead();
//
//    tftSetNoStroke();
//    tftSetFill(BLUE);
//    tftRect(50, 50, 300, 200);
//
//    tftSetStroke(BLACK);
//    tftSetFill(GREEN);
//    tftRect(60, 60, 280, 180);
//    while(1);
//
//    tftSetNoStroke();
//    uint16_t red = 0b11111;
//    for (int r = 150; r > 10; r -= 5, red--) {
//       tftSetFill(red << 11);
//       tftCircle(240, 160, r);
//    }
//    while(1);
//
//   for (int x = 10; x < 479; x += 10) {
//      tftLine(240, 310, x, 10);
//   }
//
//   while(1);


   int ypos = 10;
   tftDrawText(5, ypos, "U");
   tftDrawText(80, ypos, ":"); //  18,5 V");
   tftDrawText(290, ypos, "1,25 A");
   ypos += 60;
   tftDrawText(200, ypos, "Max: ");

   ypos += 60;
   tftDrawText(5, ypos, "U5");
   tftDrawText(80, ypos, ": 4,9 V");
   tftDrawText(290, ypos, "0,01 A");

   ypos += 60;
   tftDrawText(5, ypos, "U12");
   tftDrawText(80, ypos, ": 12,1 V");
   tftDrawText(290, ypos, "0,02 A");

   ypos += 60;
   tftDrawText(260, ypos, "T: 42 °C");

   sei();
//   displayValue(50, 0, RED);
//   displayValue(130, 0, GREEN);
//   displayValue(210, 0, RED);

   int voltage = 0;
   int current = 0;
   while(1) {
      set_sleep_mode(0); // IDLE mode
      sleep_mode();

      // voltage
      int8_t diff = encoderRead1();
      if (diff != 0) {
         voltage += diff;
         if (voltage < 0) {
            voltage = 0;
         } else if (voltage > 350) {
            voltage = 350;
         }

         //strFormat(voltage, 1, buffer);
         //strcat(buffer, " V    ");
         //tftDrawText(98, 10, buffer);
//         MCP48xx_SetValue(voltage * 10);
      }

      // maximum current
      diff = encoderRead2();
      if (diff != 0) {
         current += diff;
         if (current < 0) {
            current = 0;
         } else if (current > 300) {
            current = 300;
         }

         strFormat(current, 2, buffer);
         strcat(buffer, " A    ");
         tftDrawText(290, 70, buffer);
//         MCP48xx_SetValue(current * 10);
      }

      wakeup++;
      if (wakeup > 100) {
         wakeup = 0;
         values[valuePtr++] = adcReadChannel(3);
         if (valuePtr == 4) {
            valuePtr = 0;
            uint16_t value = (values[0] + values[1] + values[2] + values[3]) >> 2;
            value = (value * 11) >> 5;          // / 2,9 => 0 .. 352 => 0..35,2 V or 0..3,52 A

            // strFormat(value, 1, buffer);
            // strcat(buffer, " V    ");

            strFormat(value, 2, buffer);
            strcat(buffer, " A    ");

            tftDrawText(98, 10, buffer);
         }
      }
   }
}
