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

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct _HexDump HexDump;

enum _BlockMode {BLOCKMODE_LINEAR, BLOCKMODE_GROUPED};

typedef enum _BlockMode BlockMode;

HexDump* new_HexDump(const uint8_t* data, size_t length);

void HexDump_reset(HexDump* _this, const uint8_t* data, size_t length);

void HexDump_dumpAscii(HexDump* _this, int flag);

void HexDump_setBytesPerLine(HexDump* _this, int bytesPerLine);

void HexDump_setBlockSize(HexDump* _this, int blockSize);

void HexDump_setBlockMode(HexDump* _this, BlockMode mode);

char* HexDump_headerLine(HexDump* _this);

int HexDump_hasNext(HexDump* _this);

char* HexDump_nextLine(HexDump* _this);

void HexDump_dumpAll(HexDump* _this);

#ifdef __cplusplus
}
#endif // __cplusplus
