#include <stdio.h>
#include "monitor.h"


void sfrListener(uint8_t* addr, uint8_t value) {
   printf("MODIFY: 0x%016p from 0x%02X to 0x%02X\n", addr, *addr, value);
   *addr = value;
}


void setMemory(uint8_t* mem, int offset, uint8_t value) {
    *(mem + offset) = value;
}

int main() {
    uint8_t* observedMemory = omAllocate(sfrListener);     // only one page supported currently.

    setMemory(observedMemory, 0, 0x42);
    observedMemory[2] = 0x22;

    *(observedMemory + 3) = 0x08;
    *(observedMemory + 3) |= 0x08;
    *(observedMemory + 3) &= 0x0F;

    observedMemory[5] = 0x83;
    observedMemory[5] |= 0x40;
    observedMemory[3] &= 0x0F;

    observedMemory[3] = observedMemory[4] | 0x55;

    printf("Observed Memory: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", 
           observedMemory[0],observedMemory[1],observedMemory[2],observedMemory[3],observedMemory[4],observedMemory[5],observedMemory[6],observedMemory[7],
           observedMemory[8],observedMemory[9],observedMemory[10],observedMemory[11],observedMemory[12],observedMemory[13],observedMemory[14],observedMemory[15]);
}
