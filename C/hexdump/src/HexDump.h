/*
 ============================================================================
 Name        : hexdump_c.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdint.h>


typedef struct _HexDump HexDump;

enum _BlockMode {BLOCKMODE_LINEAR, BLOCKMODE_GROUPED};

typedef enum _BlockMode BlockMode;

HexDump* new_HexDump(const uint8_t* data, size_t length);

void HexDump_reset(HexDump* this, const uint8_t* data, size_t length);

void HexDump_dumpAscii(HexDump* this, int flag);

void HexDump_setBytesPerLine(HexDump* this, int bytesPerLine);

void HexDump_setBlockSize(HexDump* this, int blockSize);

void HexDump_setBlockMode(HexDump* this, BlockMode mode);

char* HexDump_headerLine(HexDump* this);

int HexDump_hasNext(HexDump* this);

char* HexDump_nextLine(HexDump* this);

void HexDump_dumpAll(HexDump* this);
