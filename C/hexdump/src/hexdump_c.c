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
	BlockMode blockMode;
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
	result->blockMode = BLOCKMODE_LINEAR;   /* no special grouping of memory blocks */

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


void HexDump_setBlockMode(HexDump* this, BlockMode mode) {
    this->blockMode = mode;
}

void HexDump_dumpAscii(HexDump* this, int flag) {
	this->dumpAscii = flag;
}


void HexDump_setBytesPerLine(HexDump* this, int bytesPerLine) {
	this->bytesPerLine = bytesPerLine;
}


int HexDump_hasNext(HexDump* this) {
    if (this->blockMode == BLOCKMODE_LINEAR &&
        this->offset < this->length) {
        return 1;
    }

    const int numRows = this->length / this->bytesPerLine;
    if (this->blockMode == BLOCKMODE_GROUPED &&
        this->offset < this->blockSize * numRows) {
        return 1;
    }
    return 0;
}


char* HexDump_headerLine(HexDump* this) {
    char* result = 0;

    if (this->blockMode == BLOCKMODE_LINEAR) {
        result = calloc(200, 1);
        /* strcpy(result, "00 01 02 03 04"); */
    } else {
        result = calloc(200, 1);
        strcpy(result, this->prefix);
        strcat(result, "Addr:");

        char* tail = result;
        const int numBlocks = this->bytesPerLine / this->blockSize;
        const int numRows = this->length / this->bytesPerLine;
        const int blockLength = numRows * this->blockSize;
        int block = 0;
        int blockAddr = 0;
        for (block = 0;  block < numBlocks;  block++) {
            tail = result + strlen(result);
            sprintf(tail, " %04X        ", blockAddr);
            blockAddr += blockLength;
        }
    }

    return result;
}


void HexDump_nextLineBlockGrouped(HexDump* this, char* line) {
    // address
    char* tail = line + strlen(line);
    sprintf(tail, " +%02X:", this->offset);

    // hex dump
    int hexIdx = 0;
    const int numBlocks = this->bytesPerLine / this->blockSize;
    const int numRows = this->length / this->bytesPerLine;

    int block = 0;
    for (block = 0;  block < numBlocks;  block++) {

        /* one block in the current line */
        hexIdx = this->offset + block * (numRows * this->blockSize);
        int col = 0;
        for (col = 0; col < this->blockSize && hexIdx < this->length; col++, hexIdx++) {
            tail = line + strlen(line);
            sprintf(tail, " %02X", this->data[hexIdx]);
        }

        strcat(tail, " ");
    }

    this->offset += this->blockSize;
}


char* HexDump_nextLine(HexDump* this) {
	/* TODO: properly calculate maximum line length!!!! */
    char* result = calloc(5 + this->bytesPerLine * 3 + this->bytesPerLine + 20, 1);
    char* tail = result;

    // prefix
    strcat(tail, this->prefix);

    if (this->blockMode == BLOCKMODE_GROUPED) {
        HexDump_nextLineBlockGrouped(this, result);
    } else {
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
    }

    return result;
}


void HexDump_dumpAll(HexDump* this) {
	char* line = HexDump_headerLine(this);
	printf("%s\n", line);
	free(line);
	while (HexDump_hasNext(this)) {
		line = HexDump_nextLine(this);
		printf("%s\n", line);
		free(line);
	}
}
