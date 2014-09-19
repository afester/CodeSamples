/*
 ============================================================================
 Name        : hexdump_c.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HexDump.h"


struct _HexDump {
	const uint8_t* data;
	size_t length;

	int bytesPerLine;
	int blockSize;
	int dumpAscii;
	size_t offset;
	char* prefix;
};



HexDump* new_HexDump(const uint8_t* data, size_t length) {
	HexDump* result = calloc(sizeof(HexDump), 1);

	result->data = data;
	result->length = length;
	result->offset = 0;

	result->prefix = "    ";
	result->bytesPerLine = 16;
	result->dumpAscii = 1;
	result->blockSize = 8;	/* larger spacer after each 8 bytes */

	return result;
}


void HexDump_reset(HexDump* this, const uint8_t* data, size_t length) {
	this->data = data;
	this->length = length;
	this->offset = 0;
}


void HexDump_setBlockSize(HexDump* this, int blockSize) {
	this->blockSize = blockSize;
}

void HexDump_dumpAscii(HexDump* this, int flag) {
	this->dumpAscii = flag;
}


void HexDump_setBytesPerLine(HexDump* this, int bytesPerLine) {
	this->bytesPerLine = bytesPerLine;
}


int HexDump_hasNext(HexDump* this) {
    if (this->offset < this->length) {
        return 1;
    }
    return 0;
}


char* HexDump_nextLine(HexDump* this) {
	/* TODO: properly calculate maximum line length!!!! */
    char* result = calloc(5 + this->bytesPerLine * 3 + this->bytesPerLine + 20, 1);
    char* tail = result;

    // prefix
    strcat(tail, this->prefix);

    // address
    tail = result + strlen(result);
    sprintf(tail, "%04X:", this->offset);

    // hex dump
    int hexIdx = 0;
    for (hexIdx = this->offset; hexIdx < this->offset + this->bytesPerLine
            && hexIdx < this->length; hexIdx++) {
        tail = result + strlen(result);
        sprintf(tail, " %02X", this->data[hexIdx]);
        if (((hexIdx - this->offset + 1) % this->blockSize) == 0) {
        	strcat(tail, " ");
        }
    }

    if (this->dumpAscii) {
		// spacer
		for (; hexIdx < this->offset + this->bytesPerLine; hexIdx++) {
			strcat(result, "   ");
		}

		// ascii dump
		strcat(result, "  ");
		//result.append(" |");
		int idx = 0;
		for (idx = this->offset; idx < this->offset + this->bytesPerLine && idx < this->length; idx++) {
			int c = this->data[idx];
			if (c < 32) { // c == '\n' || c == '\t') {
				c = '.';
			}
			tail = result + strlen(result);
			sprintf(tail, "%c", c);
		}
		// spacer
		//for (; idx < offset + bytesPerLine; idx++) {
		//    result.append(" ");
		//}
		//result.append("|");
    }

    this->offset += this->bytesPerLine;
    return result;
}


void HexDump_dumpAll(HexDump* this) {
	while (HexDump_hasNext(this)) {
		char* nextLine = HexDump_nextLine(this);
		printf("%s\n", nextLine);
		free(nextLine);
	}
}
