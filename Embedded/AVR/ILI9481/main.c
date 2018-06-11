#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <string.h>

#include "ILI9481.h"
// #include "7seg.h"
#include "encoder.h"
#include "mcp4811.h"
#include "adc.h"
#include "../LCDisplay/cfa533.h"
#include <avr/pgmspace.h>

//extern const Bitmap8* DejaVuSans[224];

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
static char convert[10];
static int wakeup = 0;

extern const  Bitmap16 g001 PROGMEM;
extern const  Bitmap8 null PROGMEM;

uint16_t redPalette[]   = {0x0000, 0x1500, 0x1f00, 0x4d00, 0x2600, 0x2a00, 0xc210};
uint16_t greenPalette[] = {0x0000, 0x7606, 0xfb07, 0x3105, 0xc310, 0xea02, 0x8501};

uint16_t palette[] = {0xffff, 0x718c, 0xf37b, 0x3663, 0xf65a, 0xd75a, 0x3563, 0x3284, 0x34a5, 0x0000, 0x0900, 0x1400, 0x1e00, 0x1f00, 0x1b00, 0x1200, 0x0600, 0x8631, 0x0a00, 0x1800, 0x0200, 0x1600, 0x1100, 0x0300, 0x1700, 0x0100, 0x1c00, 0x1900, 0x0e00, 0x0800, 0x1000, 0x1300, 0x0c00, 0x0500, 0x0f00, 0x0d00, 0x0b00, 0x0400, 0x1d00, 0x1500, 0x0700, 0x1a00, 0xdfff};

int main() {
//   CLKPR = 0b10000000; // Enable clock prescaler change
   // CLKPR = 0b00000100; // slow down a bit ....
//   CLKPR = 0b00000001; // slow down a bit ....

   tftInit();
   tftClear(BLACK);

//   cfa533Init();
   encoderInit();
   MCP48xx_Init();
   adcInit();

   tftDrawText("Display controller: ");
   tftDeviceCodeRead();

   tftBlt(&g001, 50, 50);
   tftBltPaletteRle(&null, redPalette, 100, 50);

   while(1);
#if 0

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
#if 0
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

//         displayValue(130, value1, GREEN);
//         itoa(value1 * 10, buffer, 10);
//         cfa533SetContent(0, buffer);
         MCP48xx_SetValue(value1 * 10);
#endif
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
         displayValue(130, value2, GREEN);
         MCP48xx_SetValue(value2 * 10);
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
         itoa(voltage, convert, 10);    // "x", "xx", "xxx"
         int len = strlen(convert);      // 1, 2, 3
         if (len == 1) {
           buffer[0] = ' ';
           buffer[1] = '0';
           buffer[2] = ',';
           buffer[3] = convert[0];
         } else if (len == 2) {
           buffer[0] = ' ';
           buffer[1] = convert[0];
           buffer[2] = ',';
           buffer[3] = convert[1];
         } else if (len == 3) {
           buffer[0] = convert[0];
           buffer[1] = convert[1];
           buffer[2] = ',';
           buffer[3] = convert[2];
         }
         buffer[4] = 0;
         strcat(buffer, " V");
#endif
         displayValue(210, voltage, RED);
         //cfa533SetContent(1, buffer);
      }
#endif
   }
#endif
}
