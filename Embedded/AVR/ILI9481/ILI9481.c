#include <avr/io.h>
#include <util/delay.h>
#include "ILI9481.h"

/*****************************************************************************/
/* Low level hardware access functions */

#define RD_MASK  0b00001000
#define RD_LOW  PORTB &= ~RD_MASK
#define RD_HIGH PORTB |=  RD_MASK

#define WR_MASK  0b00000100
#define WR_LOW  PORTB &= ~WR_MASK
#define WR_HIGH PORTB |=  WR_MASK

#define RS_MASK  0b00000010
#define RS_LOW  PORTB &= ~RS_MASK
#define RS_HIGH PORTB |=  RS_MASK

#define CS_MASK  0b00000001
#define CS_LOW  PORTB &= ~CS_MASK
#define CS_HIGH PORTB |=  CS_MASK

#define RST_MASK 0b00000001
#define RST_LOW  PORTF &= ~RST_MASK
#define RST_HIGH PORTF |=  RST_MASK


void configurePorts() {
  DDRL = 0b11111111;
  DDRB = 0b00001111;
  DDRF = 0b00000001;

  RD_HIGH;
  WR_HIGH;
  RS_HIGH;
  CS_HIGH;
  RST_HIGH;
}


/**
 * Write one byte into the device.
 *
 * @param data The data byte to write.
 */
void Lcd_Writ_Bus(uint8_t data) {
  PORTL = data;
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
  DDRL = 0b00000000;
  RS_HIGH;
  RD_LOW;
  RD_HIGH;
  uint8_t result = PINL;
  DDRL = 0b11111111;
  return result;
}


//void Lcd_Write_Com_Data(unsigned char com,unsigned char dat) {
//  Lcd_Write_Com(com);
//  Lcd_Write_Data(dat);
//}


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
  Lcd_Write_Data(0b00100000);
                // |||||||+-- Vertical flip
                // ||||||+--- Horizontal flip
                // |||||+---- 0
                // ||||+----- Pixels sent in BGR order
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

  Lcd_Write_Com(0x2c);      // Write_memory_start
}

static const char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

void tftDeviceCodeRead() {
   CS_LOW;

   Lcd_Write_Com(0xbf);

   Lcd_Read_Data();
   uint8_t mpi1 = Lcd_Read_Data();
   uint8_t mpi2 = Lcd_Read_Data();
   uint8_t v1 = Lcd_Read_Data();
   uint8_t v2 = Lcd_Read_Data();
   Lcd_Read_Data();

   CS_HIGH;

   tftDrawChar(hex[mpi1 >> 4]);   
   tftDrawChar(hex[mpi1 & 0x0f]);   
   tftDrawChar(hex[mpi2 >> 4]);   
   tftDrawChar(hex[mpi2 & 0x0f]);   
   tftDrawChar(hex[v1 >> 4]);   
   tftDrawChar(hex[v1 & 0x0f]);   
   tftDrawChar(hex[v2 >> 4]);   
   tftDrawChar(hex[v2 & 0x0f]);   
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


void tftClear(uint8_t col) {
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


void tftRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t col) {
  tftHLine(x    , y    , w, col);
  tftHLine(x    , y+h-1, w, col);
  tftVLine(x    , y    , h, col);
  tftVLine(x+w-1, y    , h, col);
}


void tftBlt(const uint8_t* source, uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  CS_LOW;

  Address_set(x, y, x+w-1, y+h-1);
  for(int i = 0; i < h; i++) {
    for(int m = 0; m < w; m++) { // x direction
      Lcd_Write_Data(*source++);
      Lcd_Write_Data(*source++);
    }
  }

  CS_HIGH;
}


void tftDrawPixel(uint16_t x, uint16_t y, uint16_t col) {
  CS_LOW;

  Address_set(x, y, x, y);
  Lcd_Write_Data(col>>8);
  Lcd_Write_Data(col);

  CS_HIGH;
}

#include <avr/pgmspace.h>

// extern const unsigned char font[] PROGMEM;
// static const unsigned char font[] PROGMEM = {0x7C, 0x12, 0x11, 0x12, 0x7C};
extern const unsigned char font[];
//static const unsigned char font[] = {0x7C, 0x12, 0x11, 0x12, 0x7C};
//01111100
//00010010
//00010001
//00010010
//01111100

static uint16_t xpos=0;
static uint16_t ypos=0;

void tftDrawChar(char c) {
   if (c == '\n') {
     xpos = 0;
     ypos += 9;
     return;
   }

   const uint8_t *glyph = &font[(int) ((unsigned char) c) * 5];

  CS_LOW;

  Address_set(xpos, ypos, xpos+5, ypos+6);

  uint8_t mask = 0x01;

  for(int i = 0; i < 7; i++) {

    for(int m = 0; m < 5; m++) { // x direction
      if (glyph[m] & mask) {
         Lcd_Write_Data(WHITE>>8);
         Lcd_Write_Data(WHITE);
      } else {
         Lcd_Write_Data(BLACK>>8);
         Lcd_Write_Data(BLACK);
      }
    }
    Lcd_Write_Data(BLACK>>8);
    Lcd_Write_Data(BLACK);

    mask <<= 1;
  }

  CS_HIGH;

  xpos += 6;
  if (xpos > 474) {
     xpos = 0;
     ypos += 9;
  }
}

void tftDrawText(const char* str) {
   while(*str) {
      tftDrawChar(*str);
      str++;
   }
}

