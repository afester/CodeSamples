#ifndef __ILI9481_H__
#define __ILI9481_H__

#include <stdint.h>

#define WRITE_MEMORY_START 0x2C		// resets CR/PR to SC/SP
#define READ_MEMORY_START 0x2E		// resets CR/PR to SC/SP
#define WRITE_MEMORY_CONTINUE 0x3C	// does NOT reset CR/PR to SC/SP
#define READ_MEMORY_CONTINUE 0x3E	// does NOT reset CR/PR to SC/SP

// 16 bpp (red/blue=5 bit, green=6 bit)
#define WHITE 0xffff
#define RED   0xf800
#define GREEN 0x07e0
#define BLUE  0x001f
#define BLACK 0x0000


typedef struct _Bitmap16 {
    uint16_t width;
    uint16_t height;
    uint16_t bitmap[];
}Bitmap16;

typedef struct _Bitmap8 {
    uint16_t width;
    uint16_t height;
    uint8_t bitmap[];
}Bitmap8;


/**
 * Initializes the tft display.
 */
void tftInit();


void tftDrawPixel(uint16_t x, uint16_t y, uint16_t col);

void tftDrawChar(char c);

void tftDrawText(const char* str);

void tftDeviceCodeRead();


/**
 * Draws a horizontal line.
 *
 * @param x The x coordinate of the starting point.
 * @param y The y coordinate of the starting point.
 * @param l The length of the line
 * @param col The color of the line
 */
void tftHLine(uint16_t x, uint16_t y, uint16_t l, uint16_t col);


/**
 * Draws a vertical line.
 *
 * @param x The x coordinate of the starting point.
 * @param y The y coordinate of the starting point.
 * @param l The length of the line
 * @param col The color of the line
 */
void tftVLine(uint16_t x, uint16_t y, uint16_t l, uint16_t col);


/**
 */
void tftRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t col);


/**
 * Draws a filled rectangle.
 *
 * @param x The x coordinate of the upper left corner
 * @param y The y coordinate of the upper left corner
 * @param w The width of the rectangle
 * @param h The height of the rectangle
 * @param col The color of the rectangle
 */
void tftFillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t col);


/**
 * Clears the display.
 *
 * @param col The color to use for clearing the display.
 */
void tftClear(uint16_t col);


/**
 * Copies image data into the tft display.
 *
 * @param source A pointer to the source bitmap.
 */
void tftBlt(const Bitmap16* source, uint16_t x, uint16_t y);


//void tftBltVM(const Bitmap16* source, uint16_t x, uint16_t y);
//void tftBltHM(const Bitmap16* source, uint16_t x, uint16_t y);
//void tftBltHVM(const Bitmap16* source, uint16_t x, uint16_t y);

void tftBltPalette(const Bitmap8* source, const uint16_t* palette, uint16_t x, uint16_t y);

uint16_t tftBltPaletteRle(const Bitmap8* source, const uint16_t* palette, uint16_t x, uint16_t y);

void tftBltMask(const uint8_t* source, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t col);

#endif
