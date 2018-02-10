#ifndef __CFA533_H__
#define __CFA533_H__

/**
 * Initializes the LC display.
 */
void cfa533Init();

void cfa533GetVersion(char* dest);

void cfa533SetContent(int row, const char* text);

uint8_t cfa533ReadKeys();

void cfa533SetCursor();

void cfa533SetCursorPos(uint8_t column, uint8_t row);

void cfa533EnableTempDisplay();

#endif
