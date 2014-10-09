
#ifndef __DUMP_H__
#define __DUMP_H__

#include "types.h"
#include "HexDump.h"

void dumpInit();
void dumpExtendedKey(const char* title, const extendedKey_t* extendedKey, size_t extendedKeySize);
void dumpBlock(const char* title, const block16_t* block);
void dumpData(const char* title, const uint8_t* data, size_t length);

#endif
