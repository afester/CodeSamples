#include <stdio.h>
#include <stdlib.h>
#include "HexDump.h"

int main(void) {

	uint8_t data[3] = { 1, 2, 3 };
	HexDump* hd1 = new_HexDump(data, 3);
	while (HexDump_hasNext(hd1)) {
		char* nextLine = HexDump_nextLine(hd1);
		printf("%s\n", nextLine);
		free(nextLine);
	}

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

	return EXIT_SUCCESS;
}


