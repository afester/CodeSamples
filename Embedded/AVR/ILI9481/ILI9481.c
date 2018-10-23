#include <stdlib.h>

#include <avr/io.h>
#include <util/delay.h>
#include "ILI9481.h"
#include <avr/pgmspace.h>

/*****************************************************************************/
/* Low level hardware access functions */

/* Pin configuration */
#define CTRL_PORT PORTD
#define CTRL_PORT_DIR DDRD

#define RD_MASK  0b10000000
#define RD_LOW  CTRL_PORT &= ~RD_MASK
#define RD_HIGH CTRL_PORT |=  RD_MASK

#define WR_MASK  0b01000000
#define WR_LOW  CTRL_PORT &= ~WR_MASK
#define WR_HIGH CTRL_PORT |=  WR_MASK

#define RS_MASK  0b00100000
#define RS_LOW  CTRL_PORT &= ~RS_MASK
#define RS_HIGH CTRL_PORT |=  RS_MASK

#define CS_MASK  0b00010000
#define CS_LOW  CTRL_PORT &= ~CS_MASK
#define CS_HIGH CTRL_PORT |=  CS_MASK

#define RST_MASK 0b00001000
#define RST_LOW  CTRL_PORT &= ~RST_MASK
#define RST_HIGH CTRL_PORT |=  RST_MASK

#define DATA_DIR DDRC
#define DATA_OUT PORTC
#define DATA_IN PINC

void configurePorts() {
  DATA_DIR = 0b11111111;
  CTRL_PORT_DIR = 0b11111000;

  RD_HIGH;
  WR_HIGH;
  RS_HIGH;
  CS_HIGH;
  RST_HIGH;
}

/*********************************************************************************/


/**
 * Write one byte into the device.
 *
 * @param data The data byte to write.
 */
void Lcd_Writ_Bus(uint8_t data) {
  DATA_OUT = data;
  WR_LOW;
  WR_HIGH;
}


/**
 * Writes a command byte into the device.
 *
 * @param cmd The command byte to write.
 */
void Lcd_Write_Com(uint8_t cmd) {
  RS_LOW;
  Lcd_Writ_Bus(cmd);
}


/**
 * Writes a data byte into the device.
 *
 * @param data The data byte to write.
 */
void Lcd_Write_Data(uint8_t data) {
  RS_HIGH;
  Lcd_Writ_Bus(data);
}


/**
 * Reads one data byte from the device.
 * (PENDING: NOT TESTED YET)
 *
 * @return The data byte which has been read from the device.
 */
uint8_t Lcd_Read_Data() {
  DATA_DIR = 0b00000000;
  RS_HIGH;
  RD_LOW;
  RD_HIGH;
  uint8_t result = DATA_IN;
  DATA_DIR = 0b11111111;
  return result;
}



/**************************************************************************/
/* Intermediate level ILI9184 specific functions */

void tftInit() {
  configurePorts();

  RST_HIGH;
  _delay_ms(5);
  RST_LOW;
  _delay_ms(15);
  RST_HIGH;
  _delay_ms(15);

  CS_HIGH;
  WR_HIGH;

  CS_LOW;

  Lcd_Write_Com(0x11);  // Exit_sleep_mode
  _delay_ms(20);

  Lcd_Write_Com(0xD0);  // Power_Setting
  Lcd_Write_Data(0x07); // 0000 0111 - Vci1 = 1.0 * Vci
  Lcd_Write_Data(0x42); // 0100 0010 - PON (generate VLOUT3), VGL=3*Vci1
  Lcd_Write_Data(0x18); // 0001 1000 - VREG1OUT=Vci*1.95

  Lcd_Write_Com(0xD1);  // VCOM_Control
  Lcd_Write_Data(0x00); // 0000 0000 - select D1h for VCM setting
  Lcd_Write_Data(0x07); // 0000 0111 - VCOMH=0.72*VREG1OUT
  Lcd_Write_Data(0x10); // 0001 0000 - VCOM_ampl=VREGOUT1*1.02

  Lcd_Write_Com(0xD2);  // Power_Setting_for_normal_mode
  Lcd_Write_Data(0x01); // 0000 0001 - Gamma driver ampl. = Source driver ampl. = 1.0
  Lcd_Write_Data(0x02); // 0000 0010 - fDCDC2=Fosc/16 ; fDCDC1=Fosc/4

  Lcd_Write_Com(0xC0);  // Panel_driving_setting
  Lcd_Write_Data(0x10); // 0001 0000 - Grayscale inversion, gate driver pin arrangement
  Lcd_Write_Data(0x3B); // 0011 1011 - (value+1)*8 = number of lines to drive (59+1=60, 60*8=480)
  Lcd_Write_Data(0x00); // 0000 0000 - Scanning start position
                        // Missing parameter????? The following
                        // parameters are not consistent with the manual!
  Lcd_Write_Data(0x02); // 0000 0010 - ??????
  Lcd_Write_Data(0x11); // 0001 0001 - source output level in non-display area;

  Lcd_Write_Com(0xC5);  // Frame_rate_and_inversion_control
  Lcd_Write_Data(0x03); // 0000 0011 - Frame rate 72 Hz

// This is the most important register, since it sets the 
// way how the frame memory is set when it is written,
// and how the frame memory is mapped to the display panel
  Lcd_Write_Com(0x36);  // Set_address_mode
  Lcd_Write_Data(0b00101000);
                // |||||||+-- Vertical flip
                // ||||||+--- Horizontal flip
                // |||||+---- 0
                // ||||+----- Pixels sent in RGB order
                // |||+------ LCD refresh top to bottom
                // ||+------- Page/Column order
                // |+-------- Column address order left to right
                // +--------- Page address order top to bottom

  Lcd_Write_Com(0x3A);  // Set_pixel_format
  Lcd_Write_Data(0x55); // x101 x101 - 16 bit per pixel (65536 colors)

  Lcd_Write_Com(0x2A);  // Set_column_address
  Lcd_Write_Data(0x00); // 0000 0000
  Lcd_Write_Data(0x00); // 0000 0000 - Start column = 0
  Lcd_Write_Data(0x01); // 0000 0001
  Lcd_Write_Data(0x3F); // 0011 1111 - End column = 0x13F = 319

  Lcd_Write_Com(0x2B);  // Set_page_address
  Lcd_Write_Data(0x00); // 0000 0000
  Lcd_Write_Data(0x00); // 0000 0000 - Start page = 0
  Lcd_Write_Data(0x01); // 0000 0001
  Lcd_Write_Data(0xE0); // 1110 0000 - End page = 320 (???) (should be 1DF?)
  _delay_ms(120);

  Lcd_Write_Com(0x29);  // Set_display_on

  Lcd_Write_Com(0x2c);  // Write_memory_start
}



void Address_set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  Lcd_Write_Com(0x2a);      // Set_column_address

  Lcd_Write_Data(x1>>8);    // MSB
  Lcd_Write_Data(x1);       // LSB

  Lcd_Write_Data(x2>>8);    // MSB
  Lcd_Write_Data(x2);       // LSB

  Lcd_Write_Com(0x2b);      // Set_page_address

  Lcd_Write_Data(y1>>8);    // MSB
  Lcd_Write_Data(y1);       // LSB

  Lcd_Write_Data(y2>>8);    // MSB
  Lcd_Write_Data(y2);       // LSB

  Lcd_Write_Com(0x2e);      // Write_memory_start
  Lcd_Write_Com(0x2c);      // Write_memory_start
}

void tftDeviceCodeRead(uint8_t result[4]) {
   CS_LOW;

   Lcd_Write_Com(0xbf);

   Lcd_Read_Data();			// dummy read
   result[0] = Lcd_Read_Data();		// MIPI Alliance code
   result[1] = Lcd_Read_Data();		// MIPI Alliance code
   result[2] = Lcd_Read_Data();		// Device ID code
   result[3] = Lcd_Read_Data();		// Device ID code
   Lcd_Read_Data();			// Exit code (0xff)

   CS_HIGH;
}


void tftFillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t col) {
  CS_LOW;

  Address_set(x, y, x+w-1, y+h-1);
  for(int i = 0; i < h; i++) {
    for(int m = 0; m < w; m++) {
      Lcd_Write_Data(col>>8);
      Lcd_Write_Data(col);
    }
  }

  CS_HIGH;
}


void tftClear(uint16_t col) {
  tftFillRect(0, 0, 480, 320, col);
}


void tftHLine(uint16_t x, uint16_t y, uint16_t l, uint16_t col) {
  CS_LOW;

  Address_set(x, y, x+l-1, y);
  for(int i=0; i < l; i++) {
      Lcd_Write_Data(col>>8);
      Lcd_Write_Data(col);
  }

  CS_HIGH;
}

void tftVLine(uint16_t x, uint16_t y, uint16_t l, uint16_t col) {
  CS_LOW;

  Address_set(x, y, x, y+l-1);
  for(int i = 0; i < l; i++) {
      Lcd_Write_Data(col>>8);
      Lcd_Write_Data(col);
  }

  CS_HIGH;
}


void tftLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t col) {
  CS_LOW;

//  Address_set(x, y, x+l-1, y);
//  for(int i=0; i < l; i++) {
//      Lcd_Write_Data(col>>8);
//      Lcd_Write_Data(col);
//  }

  CS_HIGH;
}


void tftCircle(uint16_t cx, uint16_t cy, uint16_t r) {
}


void tftRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t col) {
  tftHLine(x    , y    , w, col);
  tftHLine(x    , y+h-1, w, col);
  tftVLine(x    , y    , h, col);
  tftVLine(x+w-1, y    , h, col);
}


void tftBltMask(const uint8_t* source, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t col) {
    CS_LOW;

    const uint16_t* source16 = (const uint16_t*) source;

    Address_set(x, y, x+w-1, y+h-1);
    for(int i = 0; i < h; i++) {
      for(int m = 0; m < w; m++) { // x direction
        if (*source16++) {  // white pixel in mask gets background color



            Lcd_Write_Data(BLACK>>8);
            Lcd_Write_Data(BLACK);
        } else {            // non-white pixels in mask get given color
            Lcd_Write_Data(col>>8);
            Lcd_Write_Data(col);
        }
      }
    }

    CS_HIGH;
}


void tftBlt(const Bitmap16* source, uint16_t x, uint16_t y) {
  CS_LOW;

  uint8_t* reader = (uint8_t*) source->bitmap;
  const uint16_t width = pgm_read_byte(&source->width);
  const uint16_t height = pgm_read_byte(&source->height);
  Address_set(x, y, x + width - 1, y + height - 1);

  for(int i = 0; i < height; i++) {
    for(int m = 0; m < width; m++) { // x direction
      uint8_t upper = pgm_read_byte(&(*reader++));
      uint8_t lower = pgm_read_byte(&(*reader++));
      Lcd_Write_Data(upper); // High byte!!
      Lcd_Write_Data(lower); // Low byte!!
    }
  }

  CS_HIGH;
}

#include <avr/pgmspace.h>

void tftBltPalette(const Bitmap8* source, const uint16_t* palette, uint16_t x, uint16_t y) {
  CS_LOW;

  uint8_t* reader = (uint8_t*) source->bitmap;
  const uint16_t width = pgm_read_byte(&source->width);
  const uint16_t height = pgm_read_byte(&source->height);
  Address_set(x, y, x + width - 1, y + height - 1);

  for(int i = 0; i < height; i++) {
    for(int m = 0; m < width; m++) { // x direction
      uint8_t idx = pgm_read_byte(&(*reader++));
      uint16_t data = palette[idx];

      Lcd_Write_Data(data & 0xff); // High byte!!
      Lcd_Write_Data(data >> 8);   // Low byte!!
    }
  }

  CS_HIGH;
}

uint16_t tftBltPaletteRle(const Bitmap8* source, const uint16_t* palette, uint16_t x, uint16_t y) {
  CS_LOW;

  uint8_t* reader = (uint8_t*) source->bitmap;
  const uint16_t width = pgm_read_byte(&source->width);
  const uint16_t height = pgm_read_byte(&source->height);
  Address_set(x, y, x + width - 1, y + height - 1);

  uint8_t idx = 0;
  uint8_t count = 0;
  uint16_t color = 0;
  for(int i = 0; i < height; i++) {
    for(int m = 0; m < width; m++) { // x direction

      if (count == 0) {
        idx = pgm_read_byte(&(*reader++));
        count = idx >> 4;
        uint8_t colorIdx = idx & 0x0f;
        color = pgm_read_word(&(* (palette + colorIdx)));
      }

      Lcd_Write_Data(color >> 8);   // Low byte!!
      Lcd_Write_Data(color & 0xff); // High byte!!
      count--;
    }
  }

  CS_HIGH;
  return width;
}


void tftDrawPixel(uint16_t x, uint16_t y, uint16_t col) {
  CS_LOW;

  Address_set(x, y, x, y);
  Lcd_Write_Data(col>>8);
  Lcd_Write_Data(col);

  CS_HIGH;
}


extern const Bitmap8* const charSet[224] PROGMEM;
extern const uint16_t thePalette[] PROGMEM;

uint16_t tftDrawChar(uint16_t x, uint16_t y, char c) {
   Bitmap8* glyph = pgm_read_ptr(charSet + (unsigned char) c - 32);
   if (glyph != NULL) {
      return tftBltPaletteRle(glyph, thePalette, x, y);
   }
   return 5;
}

void tftDrawText(uint16_t x, uint16_t y, const char* str) {
   while(*str) {
      x += tftDrawChar(x, y, *str);
      str++;
   }
}
