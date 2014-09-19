/*
 ============================================================================
 Name        : AES.c
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



uint8_t sbox[256] = {
0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16};

uint8_t rcon[256] = {
0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d};


void copyBlock(uint8_t* dest, const uint8_t* src, int bytesPerLine, int maxBytes) {

	int sidx = 0;
	int col = 0;
	int row = 0;

	for (col = 0;  col < 4 && sidx < maxBytes;  col++) {
		int didx = col;
		for (row = 0;  row < 4 && sidx < maxBytes;  row++, didx += bytesPerLine) {
			dest[didx] = src[sidx];
			sidx++;
		}
	}
}


void copyColumn(uint8_t* keyArea, int to) {
	int sidx = to - 1;
	int didx = to;
	int row = 0;

	for (row = 0;  row < 4;  row++, didx += 44, sidx += 44) {
		keyArea[didx] = keyArea[sidx];
	}
}


void shiftColumn(uint8_t* keyArea, int col) {
	uint8_t temp = keyArea[col];
	keyArea[col + 0*44] = keyArea[col + 1*44];
	keyArea[col + 1*44] = keyArea[col + 2*44];
	keyArea[col + 2*44] = keyArea[col + 3*44];
	keyArea[col + 3*44] = temp;
}


void substituteColumn(uint8_t* keyArea, int col) {
	keyArea[col + 0*44] = sbox[keyArea[col + 0*44]];
	keyArea[col + 1*44] = sbox[keyArea[col + 1*44]];
	keyArea[col + 2*44] = sbox[keyArea[col + 2*44]];
	keyArea[col + 3*44] = sbox[keyArea[col + 3*44]];
}


void xorRcon(uint8_t* keyArea, int col, int round) {
	keyArea[col + 0*44] = keyArea[col + 0*44] ^ keyArea[(col-4) + 0*44] ^ rcon[round];
	keyArea[col + 1*44] = keyArea[col + 1*44] ^ keyArea[(col-4) + 1*44];
	keyArea[col + 2*44] = keyArea[col + 2*44] ^ keyArea[(col-4) + 2*44];
	keyArea[col + 3*44] = keyArea[col + 3*44] ^ keyArea[(col-4) + 3*44];
}


void xorColumn(uint8_t* keyArea, int col) {
	keyArea[col + 0*44] = keyArea[(col-1) + 0*44] ^ keyArea[(col-4) + 0*44];
	keyArea[col + 1*44] = keyArea[(col-1) + 1*44] ^ keyArea[(col-4) + 1*44];
	keyArea[col + 2*44] = keyArea[(col-1) + 2*44] ^ keyArea[(col-4) + 2*44];
	keyArea[col + 3*44] = keyArea[(col-1) + 3*44] ^ keyArea[(col-4) + 3*44];
}


void AddRoundKey(uint8_t* block, const uint8_t* extendedKey, int keyRound) {
	int didx = 0;	/* destination index */

	int row = 0;
	for (row = 0;  row < 4;  row++) {
		int col = 0;
		int sidx = (44 * row) + (4 * keyRound); /* source index of first key byte */

		/* calculate one row */
		for (col = 0;  col < 4;  col++) {
			block[didx] = block[didx] ^ extendedKey[sidx];
			didx++;
			sidx++;
		}
	}
}


void SubBytes(uint8_t* block) {
	int idx = 0;
	for (idx = 0;  idx < 16;  idx++) {
		block[idx] = sbox[block[idx]];
	}
}


void ShiftRows(uint8_t* block) {
	int row = 0;
	for (row = 1;  row < 4;  row++) {
		int idx = 4 * row;
		uint8_t temp = block[idx];
		block[idx] = block[idx+1];
		block[idx+1] = block[idx+2];
		block[idx+2] = block[idx+3];
		block[idx+3] = temp;
	}
	for (row = 2;  row < 4;  row++) {
		int idx = 4 * row;
		uint8_t temp = block[idx];
		block[idx] = block[idx+1];
		block[idx+1] = block[idx+2];
		block[idx+2] = block[idx+3];
		block[idx+3] = temp;
	}
	for (row = 3;  row < 4;  row++) {
		int idx = 4 * row;
		uint8_t temp = block[idx];
		block[idx] = block[idx+1];
		block[idx+1] = block[idx+2];
		block[idx+2] = block[idx+3];
		block[idx+3] = temp;
	}
}

/**
 * Note: The multiplication is not an arithmetic multiplication, but a
 * multiplication over a Galois field!
 * e.g. 0xAF * 0x08 = 0x578 (0x78 if we consider the low byte only)
 * BUT: 0xAF * 0x08 = 0x0F  when multplying over a Galois Field!
 *
 * The algorithm is explained in detail at http://www.samiam.org/galois.html
 * Since we dont care about side attacks here and since the second multiplicator is
 * always 1, 2 or 3, we can use a simpler approach as described in
 * "c't Programmieren 2014, p. 158":
 */
uint8_t gmul(uint8_t a, uint8_t b) {
	if (b == 1) {
		return a;
	}

	if (b == 2) {
		uint16_t c = ((uint16_t) a) << 1;
		if (a & 0x80) {
			c ^= 0x1b;
		}
		return c & 0xff;
	}

	if (b == 3) {
		return gmul(a, 2) ^ a;
	}

	return 0;
}


void MixColumns(uint8_t* block) {
	int col = 0;
	for (col = 0;  col < 4;  col++) {
		uint8_t b0 = gmul(block[col], 2) ^ gmul(block[col + 1*4], 3) ^ gmul(block[col + 2*4], 1) ^ gmul(block[col + 3*4], 1);
		uint8_t b1 = gmul(block[col], 1) ^ gmul(block[col + 1*4], 2) ^ gmul(block[col + 2*4], 3) ^ gmul(block[col + 3*4], 1);
		uint8_t b2 = gmul(block[col], 1) ^ gmul(block[col + 1*4], 1) ^ gmul(block[col + 2*4], 2) ^ gmul(block[col + 3*4], 3);
		uint8_t b3 = gmul(block[col], 3) ^ gmul(block[col + 1*4], 1) ^ gmul(block[col + 2*4], 1) ^ gmul(block[col + 3*4], 2);

		block[col + 0*4] = b0;
		block[col + 1*4] = b1;
		block[col + 2*4] = b2;
		block[col + 3*4] = b3;
	}
}


static HexDump* hd2 = NULL;
static HexDump* hd3 = NULL;
uint8_t* extendedKey = NULL;


void createExtendedKey(uint8_t* extendedKey, const uint8_t* key, size_t extendedKeySize) {
	int round = 0;

	copyBlock(extendedKey, key, 44, 16);

	hd3 = new_HexDump(extendedKey, extendedKeySize);
	HexDump_dumpAscii(hd3, 0);
	HexDump_setBytesPerLine(hd3, 44);
	HexDump_setBlockSize(hd3, 4);

	HexDump_dumpAll(hd3);
	printf("\n");

/* Round key generation ***********************************************/
	for (round = 1;  round <= 10;  round++) {
		int col = 4 * round;

		printf("========================================================================\n" \
		       "Generate round key: %d\n", round);

		printf("  COPY:\n");
		copyColumn(extendedKey, col);
		HexDump_reset(hd3, extendedKey, extendedKeySize);
		HexDump_dumpAll(hd3);
		printf("\n");

		printf("  SHIFT:\n");
		shiftColumn(extendedKey, col);
		HexDump_reset(hd3, extendedKey, extendedKeySize);
		HexDump_dumpAll(hd3);
		printf("\n");

		printf("  SUBSTITUTE:\n");
		substituteColumn(extendedKey, col);
		HexDump_reset(hd3, extendedKey, extendedKeySize);
		HexDump_dumpAll(hd3);
		printf("\n");

		printf("  XOR 1 (incl. Rcon):\n");
		xorRcon(extendedKey, col, round);
		HexDump_reset(hd3, extendedKey, extendedKeySize);
		HexDump_dumpAll(hd3);
		printf("\n");

		printf("  XOR 2:\n");
		xorColumn(extendedKey, col+1);
		HexDump_reset(hd3, extendedKey, extendedKeySize);
		HexDump_dumpAll(hd3);
		printf("\n");

		printf("  XOR 3:\n");
		xorColumn(extendedKey, col+2);
		HexDump_reset(hd3, extendedKey, extendedKeySize);
		HexDump_dumpAll(hd3);
		printf("\n");

		printf("  XOR 4:\n");
		xorColumn(extendedKey, col+3);
		HexDump_reset(hd3, extendedKey, extendedKeySize);
		HexDump_dumpAll(hd3);
		printf("\n");
	}
/************************************************/
}


void encryptBlock(uint8_t* block, const uint8_t* plaintext) {
	int round = 0;


	printf("  Plaintext:\n");
	HexDump_dumpAscii(hd3, 0);
	HexDump_setBytesPerLine(hd3, 4);
	HexDump_reset(hd3, block, 16);
	HexDump_dumpAll(hd3);
	printf("\n");

	printf("Pre Round:\n");
	AddRoundKey(block, extendedKey, 0);
	HexDump_reset(hd3, block, 16);
	HexDump_dumpAll(hd3);
	printf("\n");

/*******************/
	for (round = 1;  round <= 9;  round++) {
		printf("Round %d:\n", round);

		printf("  SubBytes:\n");
		SubBytes(block);
		HexDump_reset(hd3, block, 16);
		HexDump_dumpAll(hd3);

		printf("  ShiftRows:\n");
		ShiftRows(block);
		HexDump_reset(hd3, block, 16);
		HexDump_dumpAll(hd3);

		printf("  MixColumns:\n");
		MixColumns(block);
		HexDump_reset(hd3, block, 16);
		HexDump_dumpAll(hd3);

		printf("  AddRoundKey:\n");
		AddRoundKey(block, extendedKey, round);
		HexDump_reset(hd3, block, 16);
		HexDump_dumpAll(hd3);
		printf("\n");
	}
/*******************/

	printf("Final Round:\n");
	printf("  SubBytes:\n");
	SubBytes(block);
	HexDump_reset(hd3, block, 16);
	HexDump_dumpAll(hd3);

	printf("  ShiftRows:\n");
	ShiftRows(block);
	HexDump_reset(hd3, block, 16);
	HexDump_dumpAll(hd3);

	printf("  AddRoundKey:\n");
	AddRoundKey(block, extendedKey, 10);

}


void padBlock(uint8_t* block, size_t remaining) {
    /* Padding - PENDING: Improve and add another block if plaintext is multiple of 16 */
    int padding = 16 - remaining;
    if ( padding > 0) {
        size_t row = remaining % 4;
        size_t col = remaining / 4;

        for (  ;  col < 4 ; col++) {
            for (  ; row < 4;  row++) {
                block[4 * row + col] = padding;
            }
            row = 0;
        }
    }
}


int main(void) {
	const uint8_t* key = (uint8_t*) "Bar12345Bar12345";
	size_t extendedKeySize = 0;

	hd2 = new_HexDump((uint8_t*) key, 16);
	HexDump_dumpAll(hd2);
	printf("\n");

	extendedKeySize = 11 * 4 * 4;
	extendedKey = calloc(extendedKeySize, 1);

	createExtendedKey(extendedKey, key, extendedKeySize);


	/* Encryption (one block only!) ***********************************************/
	const uint8_t* plaintext = (uint8_t*)"Sample String which we want to encrypt";
	const size_t plaintextLen = strlen((char*) plaintext);

	HexDump_reset(hd2, plaintext, plaintextLen);
	HexDump_dumpAll(hd2);
	printf("\n");


	/**
	 * The simplest of the encryption modes is the electronic codebook (ECB) mode.
     * The message is divided into blocks, and each block is encrypted separately.
     */
	uint8_t* block = calloc(16, 1);
	int offset = 0;
	while(offset < plaintextLen) {
        size_t remaining = plaintextLen - offset;
		copyBlock(block, plaintext + offset, 4, remaining > 16 ? 16 : remaining);
        padBlock(block, remaining);

		printf("  NEXT BLOCK:\n");
		HexDump_reset(hd3, block, 16);
		HexDump_dumpAll(hd3);
		printf("\n");

		encryptBlock(block, plaintext + offset);
		HexDump_reset(hd3, block, 16);
		HexDump_dumpAll(hd3);
		printf("\n");
		offset += 16;
	}


	return EXIT_SUCCESS;
}
