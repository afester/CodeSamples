
#include <stdio.h>
#include "HexDump.h"
#include "types.h"

static HexDump* stdDump = NULL;
static HexDump* extKeyDump = NULL;
static HexDump* blockDump = NULL;

void dumpInit() {
    stdDump = new_HexDump(NULL, 0);

	extKeyDump = new_HexDump(NULL, 0);
	HexDump_dumpAscii(extKeyDump, 0);
	HexDump_setBytesPerLine(extKeyDump, 44);
	HexDump_setBlockSize(extKeyDump, 4);
	HexDump_setBlockMode(extKeyDump, BLOCKMODE_GROUPED);

    blockDump = new_HexDump(NULL, 0);
	HexDump_dumpAscii(blockDump, 0);
	HexDump_setBytesPerLine(blockDump, 4);
	HexDump_setBlockSize(blockDump, 4);
}

void dumpExtendedKey(const char* title, const extendedKey_t* extendedKey, size_t extendedKeySize) {
    printf("  %s:\n", title);
    HexDump_reset(extKeyDump, (uint8_t*) extendedKey, extendedKeySize);
    HexDump_dumpAll(extKeyDump);
    printf("\n");
}


void dumpBlock(const char* title, const block16_t* block, bool dumpAscii) {
    printf("  %s:\n", title);
    HexDump_dumpAscii(blockDump, dumpAscii);
    HexDump_reset(blockDump, (uint8_t*) block, 16);
    HexDump_dumpAll(blockDump);
    printf("\n");
}


void dumpData(const char* title, const uint8_t* data, size_t length) {
    printf("  %s:\n", title);
    HexDump_reset(stdDump, data, length);
    HexDump_dumpAll(stdDump);
    printf("\n");
}



