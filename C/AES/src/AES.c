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
#include "dump.h"

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




void shiftColumn(extendedKey_t* keyArea, int block) {
	uint8_t temp = keyArea->data[block][0][0];
	keyArea->data[block][0][0] = keyArea->data[block][1][0];
	keyArea->data[block][1][0] = keyArea->data[block][2][0];
	keyArea->data[block][2][0] = keyArea->data[block][3][0];
    keyArea->data[block][3][0] = temp;
}

void substituteColumn(extendedKey_t* keyArea, int block) {
	keyArea->data[block][0][0] = sbox[ keyArea->data[block][0][0] ];
	keyArea->data[block][1][0] = sbox[ keyArea->data[block][1][0] ];
	keyArea->data[block][2][0] = sbox[ keyArea->data[block][2][0] ];
	keyArea->data[block][3][0] = sbox[ keyArea->data[block][3][0] ];
}

void xorRcon(extendedKey_t* keyArea, int block) {
	keyArea->data[block][0][0] = keyArea->data[block][0][0] ^ keyArea->data[block - 1][0][0] ^ rcon[block];
	keyArea->data[block][1][0] = keyArea->data[block][1][0] ^ keyArea->data[block - 1][1][0];
	keyArea->data[block][2][0] = keyArea->data[block][2][0] ^ keyArea->data[block - 1][2][0];
	keyArea->data[block][3][0] = keyArea->data[block][3][0] ^ keyArea->data[block - 1][3][0];
}

void xorColumn(extendedKey_t* keyArea, int block, int col) {
	keyArea->data[block][0][col] = keyArea->data[block][0][col - 1] ^ keyArea->data[block - 1][0][col];
	keyArea->data[block][1][col] = keyArea->data[block][1][col - 1] ^ keyArea->data[block - 1][1][col];
	keyArea->data[block][2][col] = keyArea->data[block][2][col - 1] ^ keyArea->data[block - 1][2][col];
	keyArea->data[block][3][col] = keyArea->data[block][3][col - 1] ^ keyArea->data[block - 1][3][col];
}


void copyBlock(block16_t* dest, const uint8_t* src, int maxBytes) {

    size_t idx;
    for (idx = 0;  idx < 16 && idx < maxBytes;  idx++) {
        dest->data[idx % 4][idx / 4] = src[idx];
    }
}

void AddRoundKey(block16_t* block, const extendedKey_t* extendedKey, int keyRound) {
	int row = 0;
	for (row = 0;  row < 4;  row++) {

		int col = 0;
		for (col = 0;  col < 4;  col++) {
            block->data[row][col] = block->data[row][col] ^ extendedKey->data[keyRound][row][col];
		}
	}
}

void SubBytes(block16_t* block) {
   	int row = 0;
	for (row = 0;  row < 4;  row++) {

		int col = 0;
		for (col = 0;  col < 4;  col++) {
            block->data[row][col] = sbox[ block->data[row][col] ];
		}
	}
}

void ShiftRows(block16_t* block) {
	int row = 0;
	for (row = 1;  row < 4;  row++) {
		uint8_t temp = block->data[row][0];
		block->data[row][0] = block->data[row][1];
		block->data[row][1] = block->data[row][2];
		block->data[row][2] = block->data[row][3];
		block->data[row][3] = temp;
	}
	for (row = 2;  row < 4;  row++) {
		uint8_t temp = block->data[row][0];
		block->data[row][0] = block->data[row][1];
		block->data[row][1] = block->data[row][2];
		block->data[row][2] = block->data[row][3];
		block->data[row][3] = temp;
	}
	for (row = 3;  row < 4;  row++) {
		uint8_t temp = block->data[row][0];
		block->data[row][0] = block->data[row][1];
		block->data[row][1] = block->data[row][2];
		block->data[row][2] = block->data[row][3];
		block->data[row][3] = temp;
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


void MixColumns(block16_t* block) {
	int col = 0;
	for (col = 0;  col < 4;  col++) {
		uint8_t b0 = gmul( block->data[0][col], 2) ^ gmul( block->data[1][col], 3) ^ gmul( block->data[2][col], 1) ^ gmul( block->data[3][col], 1);
		uint8_t b1 = gmul( block->data[0][col], 1) ^ gmul( block->data[1][col], 2) ^ gmul( block->data[2][col], 3) ^ gmul( block->data[3][col], 1);
		uint8_t b2 = gmul( block->data[0][col], 1) ^ gmul( block->data[1][col], 1) ^ gmul( block->data[2][col], 2) ^ gmul( block->data[3][col], 3);
		uint8_t b3 = gmul( block->data[0][col], 3) ^ gmul( block->data[1][col], 1) ^ gmul( block->data[2][col], 1) ^ gmul( block->data[3][col], 2);

        block->data[0][col] = b0;
        block->data[1][col] = b1;
        block->data[2][col] = b2;
        block->data[3][col] = b3;
	}
}


static extendedKey_t*  extendedKey = NULL;

void createExtendedKey(extendedKey_t* extendedKey, const uint8_t* key, size_t extendedKeySize) {
	int round = 0;

    /* Copy first block */
    size_t idx;
    for (idx = 0;  idx < 16;  idx++) {
        extendedKey->data[0][idx % 4][idx / 4] = key[idx];
    }
    dumpExtendedKey("Initial state", extendedKey, extendedKeySize);

/* Round key generation ***********************************************/
	for (round = 1;  round <= 10;  round++) {
		printf("========================================================================\n" \
		       "Generate round key: %d\n", round);

        /* Step 1: Copy Column */
		int row = 0;
		for (row = 0;  row < 4;  row++) {
            extendedKey->data[round][row][0] = extendedKey->data[round-1][row][3];
		}
        dumpExtendedKey("Copy", extendedKey, extendedKeySize);

        /* Step 2: Shift Column */
		shiftColumn(extendedKey, round);
        dumpExtendedKey("Shift", extendedKey, extendedKeySize);

        /* Step 3: Substitute */
		substituteColumn(extendedKey, round);
        dumpExtendedKey("Substitute", extendedKey, extendedKeySize);

        /* Step 4: XOR column 0, including rcon */
		xorRcon(extendedKey, round);
        dumpExtendedKey("XOR 1 (incl. Rcon)", extendedKey, extendedKeySize);

        /* Step 5: XOR column 1 */
		xorColumn(extendedKey, round, 1);
        dumpExtendedKey("XOR 2", extendedKey, extendedKeySize);

        /* Step 6: XOR column 2 */
		xorColumn(extendedKey, round, 2);
        dumpExtendedKey("XOR 3", extendedKey, extendedKeySize);

        /* Step 7: XOR column 3 */
		xorColumn(extendedKey, round, 3);
        dumpExtendedKey("XOR 4", extendedKey, extendedKeySize);
	}
}


void encryptBlock(block16_t* block) {
	int round = 0;

//    HexDump_dumpAscii(blockDump, 1);
    dumpBlock("Plaintext", block);
//    HexDump_dumpAscii(blockDump, 0);

	printf("Pre Round:\n");
	AddRoundKey(block, extendedKey, 0);
    dumpBlock("AddRoundKey", block);

	for (round = 1;  round <= 9;  round++) {
		printf("Round %d:\n", round);

        /* Step 1: SubBytes */
		SubBytes(block);
        dumpBlock("SubBytes", block);

        /* Step 2: ShiftRows */
		ShiftRows(block);
        dumpBlock("ShiftRows", block);

        /* Step 3: MixColumns */
		MixColumns(block);
        dumpBlock("MixColumns", block);

        /* Step 4: AddRoundKey*/
		AddRoundKey(block, extendedKey, round);
        dumpBlock("AddRoundKey", block);
	}

	printf("Final Round:\n");

	SubBytes(block);
    dumpBlock("SubBytes", block);

	ShiftRows(block);
    dumpBlock("ShiftRows", block);

	AddRoundKey(block, extendedKey, 10);
    dumpBlock("AddRoundKey", block);
}


void padBlock2(uint8_t* block, size_t remaining) {
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


void padBlock(block16_t* block, size_t remaining) {
   int padding = 16 - remaining;
   if ( padding > 0) {
        size_t row = remaining % 4;
        size_t col = remaining / 4;

        for (  ;  col < 4 ; col++) {
            for (  ; row < 4;  row++) {
                block->data[row][col] = padding;
            }
            row = 0;
        }
    }
}


int main(void) {
    dumpInit();

/** Key generation ******************************************************/

	const uint8_t* key = (uint8_t*) "Bar12345Bar12345";
    dumpData("Key", key, 16);

	size_t extendedKeySize = sizeof(extendedKey_t);
    extendedKey = calloc(extendedKeySize, 1);
	createExtendedKey(extendedKey, key, extendedKeySize);

/** Encryption **********************************************************/

	const uint8_t* plaintext = (uint8_t*)"Sample String which we want to encrypt";
	const size_t plaintextLen = strlen((char*) plaintext);
    dumpData("Plaintext", plaintext, plaintextLen);

	/**
	 * The simplest of the encryption modes is the electronic codebook (ECB) mode.
     * The message is divided into blocks, and each block is encrypted separately.
     */
	block16_t* block = calloc(sizeof(block16_t), 1);
	int offset = 0;
	while(offset < plaintextLen) {
        size_t remaining = plaintextLen - offset;

		copyBlock(block, plaintext + offset, remaining > 16 ? 16 : remaining);

        padBlock(block, remaining);

		encryptBlock(block);

		offset += 16;
	}
	if (offset == plaintextLen) {
        padBlock(block, 16);
		encryptBlock(block);
	}

    free(block);
    free(extendedKey);

	return EXIT_SUCCESS;
}
