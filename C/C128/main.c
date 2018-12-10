/**
 * C64 Memory Layout
 *                                                              |Basic ROM  |Kernal ROM |
 *                                                              |$a000 $cfff|$d000 $ffff|
 * ___________ ___________ _____________ _______________ ___________________ ______________
 *| Zero Page |Stack      |"System vars"|Screen RAM (1k)|"Free" RAM         |Non-acc. RAM  |
 *|$0000 $00FF|$0100 $01ff|$0200   $03FF|$0400 $07ff    |$0800         $cfff|$d000    $ffff|
 *
 * - Screen RAM can be relocated, but not to "high" memory addresses ...
 */
#include <stdint.h>

static char* const SCREEN = (char*) 0x400;
static uint16_t idx = 0;

void clrscr() {
    uint16_t cidx = 0;
    for (cidx = 0;  cidx < 40 * 25;  cidx++) {
        SCREEN[cidx] = ' ';
    }
}

void gotoxy(uint8_t x, uint8_t y) {
    idx = 40 * y + x;
}

void print(const char* str) {
    while(*str) {
        SCREEN[idx++] = *(str++);
    }
}

int main() {
    clrscr();
    gotoxy(0, 2);
    print("Hello World");

    while(1);
    return 0;
}
