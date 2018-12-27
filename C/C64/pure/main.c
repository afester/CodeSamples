/**
 * This is a CL65 sample program which tries to implement all required functions on its own instead
 * of using the runtime library.
 */

/*
 * C64 Memory Layout
 *                                                              |Basic ROM  |  I/O      |Kernal ROM |
 *                                                              |$a000 $cfff|$d000 $dfff|$e000 $ffff|
 * ___________ ___________ _____________ _______________ ___________________ _______________________
 *| Zero Page |Stack      |"System vars"|Screen RAM (1k)|"Free" RAM         |Non-acc. RAM           |
 *|$0000 $00FF|$0100 $01ff|$0200   $03FF|$0400 $07ff    |$0800         $cfff|$d000             $ffff|
 *
 * - Screen RAM can be relocated, but not to "high" memory addresses ...
 */
#include <stdint.h>

enum Colors {BLACK, WHITE, RED, CYAN, PURPLE, GREEN, BLUE, YELLOW, ORANGE, BROWN, PINK, DARKGRAY, MIDDLEGRAY, LIGHTGREEN, LIGHTBLUE, LIGHTGRAY};

static char* const SCREEN = (char*) 0x0400;
static uint8_t* const COLORRAM = (uint8_t*) 0xD800;
static uint8_t* const ZP = (uint8_t*) 0x0000;

enum {CURSOR_XPOS=0xD3, CURSOR_YPOS=0xD6, CURSOR_CONTROL=204};

static char* const BORDERCOL = (uint8_t*) 0xD020;
static char* const BGCOL = (uint8_t*) 0xD021;
static const char* const HEX = "0123456789abcdef";

static uint16_t idx = 0;
static uint8_t color = 0;

void clrscr() {
    // Alternatively: call kernal at $e544
    uint16_t cidx = 0;
    for (cidx = 0;  cidx < 40 * 25;  cidx++) {
        SCREEN[cidx] = ' ';
    }
}

void setBorder(uint8_t col) {
    *BORDERCOL = col;
}

void setBackground(uint8_t col) {
    *BGCOL = col;
}

void setColor(uint8_t col) {
    color = col;
}

void gotoxy(uint8_t x, uint8_t y) {
//    ZP[CURSOR_XPOS] = x;
//    ZP[CURSOR_YPOS] = y;
    idx = 40 * y + x;

    __asm__ ("jsr popa");
    __asm__ ("tax");
    __asm__ ("jsr popa");
    __asm__ ("tay");
    __asm__ ("jmp $fff0");      // PLOT
}


uint8_t mapAsciiToChar(uint8_t c) {
    if (c < 32) {
        return '?';
    }
    if (c == 64) {
        return 0;
    }
    if (c < 91) {
        return c;
    }
    if (c < 97) {
        switch(c) {
            case 91 : return 27;    // [
            case 92 : return 28;    // backslash (N/A)
            case 93 : return 29;    // ]
            case 94 : return 30;    // ^
            case 95 : return 100;   // _
            case 96 : return 39;    // `
        }
    }
    if (c < 123) {
        return c - 96;
    }
    
    if (c < 128) {
        switch(c) {
            case 123 : return 40;   // {
            case 124 : return 116;  // |
            case 125 : return 41;   // }
            case 126 : return 102;  // ~
            case 127 : return '?';  // DEL (N/A)
        }
    }
    
    if (c < 0xb0) {
        return '?';
    }

    if (c < 0xb0 + 37) {
        return c - 85;
    }

    return '?';
}


void printChar(char c) {
    if (idx >= 1000) {
        clrscr();
        idx = 0;
    }

    if (c  == '\n') {
        idx = ((idx / 40) + 1 ) * 40;
    } else {
        c = mapAsciiToChar(c);
        SCREEN[idx] = c;
        COLORRAM[idx] = color;
        idx++;
    }
}

void printHex(uint8_t val) {
    printChar(HEX[val >> 4]);
    printChar(HEX[val & 0x0f]);
}
    
void print(const char* str) {
    while(*str) {
        // printChar(*(str++));
        char c = *(str++);
        printHex(c);
        printChar(' ');
    }
}

void hexDump(uint8_t* addr) {
    uint16_t addr2 = addr;
    int idx = 0;
    printChar(HEX[addr2 >> 12]);
    printChar(HEX[(addr2 >> 8) & 0x0f]);
    printChar(HEX[(addr2 >> 4) & 0x0f]);
    printChar(HEX[addr2 & 0x0f]);
    printChar(':');
    printChar(' ');

    for (idx = 0;  idx < 8;  idx++) {
        printHex(addr[idx]);
        printChar(' ');
    }
    for (idx = 0;  idx < 8;  idx++) {
        printChar(addr[idx]);
    }
}

void dumpMem(uint8_t* addr, count) {
    int line = 0;
    for (line = 0;  line < count;  line++) {
        hexDump(addr);
        printChar('\n');
        addr += 8;
    }
}

void printNumber(uint8_t val) {
    uint8_t fill = ' ';

    uint8_t digit = (val / 100) % 10;
    if (digit == 0) {
        printChar(fill);
    } else {
        printChar('0' + digit);
        fill = '0';
    }

    digit = (val / 10) % 10;
    if (digit == 0) {
        printChar(fill);
    } else {
        printChar('0' + digit);
        fill = '0';
    }

    digit = val % 10;
    printChar('0' + digit);
}


void printCharset(uint8_t offset) {

    // lower 125 characters

    // 6 columns, 22 rows
    // 1234567890123456789012345678901234567890
    // nnn C  nnn C  nnn C  nnn C  nnn C  nnn C
    // 25 rows require 11 chars per line -> only 4 chars per glyph ...
    uint8_t row = 0;
    uint8_t column = 0;
    uint8_t columns = 6;

//    clrscr();
    gotoxy(0, 1);

    for (row = 0;  row < 22;  row++) {
        uint8_t c = offset + row;
        if (row > 17) {
            columns = 5;
        }
        for (column = 0;  column < columns;  column++) {
            uint8_t chr = 0;

            printNumber(c);
            printChar(':');

            // avoid to interpret the characters
            chr = c; //  mapAsciiToChar(c);
            SCREEN[idx] = chr;
            COLORRAM[idx] = color;
            idx++;

            if (column < 5) {
                printChar(' ');
                printChar(' ');
            }
            c += 22;
        }

        if (columns == 5) {
            printChar('\n');
        }
    }
}


enum KEYS {LEFT=0x9d, RIGHT=0x1d, DOWN=0x11, UP=0x91, BS=0x14, ENTER=0x0D, F1=0x85, F2=0x89, F3=0x86};

uint8_t getKey() {
//    __asm__ ("jsr $FF9F");    // SCNKEY - only required when interrupts are disabled
    __asm__ ("jmp $FFE4");      // GETIN
}

uint8_t waitKey() {
    uint8_t key = 0;
    while( (key = getKey()) == 0) {}
    return key;
}

void crsrOn() {
    ZP[204] = 0;
}

void crsrOff() {
    while(ZP[207] != 0) {}
    ZP[204] = 1;
}

int main() {
    clrscr();
    setBorder(DARKGRAY);
    setBackground(BLACK);
    setColor(GREEN);
    gotoxy(0, 2);
    
    // Note: Character literals are encoded in PETSCII!
    // This is different from the charset !!
    // C8 45 4C 4C 4F 20 D7 4F 52 4C 44
    // H  e  l  l  o     W  o  r  l  d
    print("Hello World");

    printChar('\n');
    //dumpMem(ZP+0xc0, 16);
    dumpMem(ZP+0xd0, 1);

    while(1) {
        char c = 0;
        crsrOn();
        c = waitKey();
        crsrOff();

        printHex(c);
        //printChar(' ');
        if (c == RIGHT) {
            gotoxy(ZP[CURSOR_XPOS]+1, ZP[CURSOR_YPOS]);
        } else if (c == LEFT) {
            gotoxy(ZP[CURSOR_XPOS]-1, ZP[CURSOR_YPOS]);
        } else if (c == UP) {
            gotoxy(ZP[CURSOR_XPOS], ZP[CURSOR_YPOS]-1);
        } else if (c == DOWN) {
            gotoxy(ZP[CURSOR_XPOS], ZP[CURSOR_YPOS]+1);
        } else if (c == F1) {
            clrscr();
            gotoxy(0, 0);
        } else if (c == F2) {
            printCharset(0);
        } else if (c == F3) {
            printCharset(128);
        } else {
            printChar(c);
        }

//        dumpMem(ZP+0xd0, 1);
//        printChar('\n');
    }
    return 0;
}
