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

uint16_t thePalette[] = {0x0000, 0xf800, 0xf800, 0xf800, 0xf800, 0xf800, 0xf800, 0xf800, 0xf800, 0xf800, 0xf800, 0xf800, 0xf800, 0xf800, 0xf800, 0xf800};

extern const Bitmap8* const charSet[224] PROGMEM;

uint16_t drawChar(uint16_t x, uint16_t y, char c) {
   Bitmap8* glyph = pgm_read_ptr(charSet + (unsigned char) c - 32);
   if (glyph != NULL) {
      return tftBltPaletteRle(glyph, thePalette, x, y);
   }
   return 5;
}

void drawText(uint16_t x, uint16_t y, const char* str) {
   while(*str) {
      x += drawChar(x, y, *str);
      str++;
   }
}

int main() {
//   CLKPR = 0b10000000; // Enable clock prescaler change
   // CLKPR = 0b00000100; // slow down a bit ....
//   CLKPR = 0b00000001; // slow down a bit ....

   tftInit();
   tftClear(BLACK);

//   cfa533Init();
//   encoderInit();
//   MCP48xx_Init();
//   adcInit();

//   tftDrawText("Display controller: ");
//   tftDeviceCodeRead();

   int ypos = 10;
   drawText(5, ypos, "U: 18,5 V");
   ypos += 50;
   drawText(5, ypos, "U5: 4,9 V");
   ypos += 50;
   drawText(5, ypos, "U12: 12,1 V");
   ypos += 50;
   drawText(5, ypos, "I: 0,05 A");
   drawText(260, ypos, "I: 0,05 A");
   ypos += 50;
   drawText(5, ypos, "I5: 0,01 A");
   ypos += 50;
   drawText(5, ypos, "I12: 0,02 A");
   drawText(260, ypos, "T: 42 °C");
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
