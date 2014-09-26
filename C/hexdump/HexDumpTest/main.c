#include <stdio.h>
#include <stdlib.h>
#include "HexDump.h"

int main(void) {

    printf("\nBlock of 3 bytes with default layout:\n------------------------------------------------------\n");
	uint8_t data[3] = { 1, 2, 3 };
	HexDump* hd1 = new_HexDump(data, 3);
	while (HexDump_hasNext(hd1)) {
		char* nextLine = HexDump_nextLine(hd1);
		printf("%s\n", nextLine);
		free(nextLine);
	}

    printf("\nBlock of 36 bytes with default layout:\n------------------------------------------------------\n");
	uint8_t data2[36] = { 1, 2, 3, 0xff,
				 254, 253, 252,  240, 64, 3, 4, 5, 6,
				34, 66, 23, 7, 5, 1, 121, 66, 77, 88, 99, 2, 4, 8, 16, 32, 64,
				128, 25, 121, 0, 17, 43 };
	HexDump* hd2 = new_HexDump(data2, 36);
	/* hd2.setPrefix("> ");*/
	while (HexDump_hasNext(hd2)) {
		char* nextLine = HexDump_nextLine(hd2);
		printf("%s\n", nextLine);
		free(nextLine);
	}

    printf("\nBlock of 176 bytes with 32 bytes per line:\n------------------------------------------------------\n");
	uint8_t* data3 = calloc(176, 1);
	HexDump* hd3 = new_HexDump(data3, 176);
	HexDump_dumpAscii(hd3, 0);
	HexDump_setBytesPerLine(hd3, 32);
	/* hd2.setPrefix("> ");*/
	while (HexDump_hasNext(hd3)) {
		char* nextLine = HexDump_nextLine(hd3);
		printf("%s\n", nextLine);
		free(nextLine);
	}


    printf("\nSpecial layout with several equal-sized memory blocks:\n------------------------------------------------------\n");
    // we would like to dump 4x4 byte blocks side by side
    uint8_t myBlock[11][4][4];

    // initialize the memory blocks - dump should look like this:
    //         Block 1      Block 2
    // 0000: 00 01 02 03  10 11 12 13
    // 0004: 04 05 06 07  14 15 16 17
    // 0008: 08 09 0a 0b  18 19 1a 1b
    // 000c: 0c 0d 0e 0f  1C 1D 1E 1F

    int col = 0;
    int row = 0;
    int num = 0;
    uint8_t c = 0;
    for (num = 0;  num < 11;  num++) {
        for (row = 0;  row < 4;  row++) {
            for (col = 0;  col < 4;  col++) {
                myBlock[num][row][col] = c++;
            }
        }
    }

    hd3 = new_HexDump((uint8_t*) myBlock, 11 * 16);
	HexDump_dumpAscii(hd3, 0);
	HexDump_setBytesPerLine(hd3, 11 * 4);
	HexDump_setBlockSize(hd3, 4);
	HexDump_setBlockMode(hd3, BLOCKMODE_GROUPED);
    HexDump_dumpAll(hd3);

	return EXIT_SUCCESS;
}
