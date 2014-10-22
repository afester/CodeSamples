#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Samples.h"

typedef unsigned short Bits16;
enum Exception {NO_EXCEPTION, NUMBER_FORMAT_EXCEPTION};

Bits16 Bits16_fromString(const char* value, Exception* exception) {
    Bits16 result = 0;
    Bits16 mask = 0x1;

    if (exception) {
        *exception = NO_EXCEPTION;
    }

    /* value is right padded, need to start from end */
    int idx = strlen(value);
    if (idx > 16) {
        if (exception) {
            *exception = NUMBER_FORMAT_EXCEPTION;
        }
        return 0;
    }

    while(idx > 0) {
        idx--;
        if (value[idx] == '1') {
            result |= mask;
        } else if (value[idx] != '0') {
            if (exception) {
                *exception = NUMBER_FORMAT_EXCEPTION;
            }
            return 0;
        }
        mask <<= 1;
    }

    return result;
}


char* Bits16_toString(Bits16 value) {
    char* result = (char*) malloc(17 * sizeof(char));
    char* ptr = result;

    Bits16 mask = 0x8000;
    for (int i = 0;  i < 16;  i++) {
        if (value & mask) {
            *ptr = '1';
        } else {
            *ptr = '0';
        }
        ptr++;
        mask >>= 1;
    }
    *ptr = 0;

    return result;
}

size_t Bits16_countOnes(Bits16 value) {
    Bits16 mask = 0x8000;
    size_t result = 0;

    while(mask != 0) {
        if (value & mask) {
            result++;
        }
        mask >>= 1;
    }

    return result;
}



void orSample() {
    Exception ex = NO_EXCEPTION;
    Bits16 first = Bits16_fromString("0001", &ex);
    Bits16 second = Bits16_fromString("0010", &ex);
    Bits16 result = first | second;
    char* val1 = Bits16_toString(first);
    char* val2 = Bits16_toString(second);
    char* val3 = Bits16_toString(result);
    printf("\n    %sb\n", val1);
    printf("OR  %sb\n", val2);
    printf("---------------------\n");
    printf("    %sb\n", val3);
    free(val1);
    free(val2);
    free(val3);

}


void andSample() {
    Exception ex = NO_EXCEPTION;
    Bits16 first = Bits16_fromString("1001", &ex);
    Bits16 second = Bits16_fromString("1010", &ex);
    Bits16 result = first & second;
    char* val1 = Bits16_toString(first);
    char* val2 = Bits16_toString(second);
    char* val3 = Bits16_toString(result);
    printf("\n    %sb\n", val1);
    printf("AND %sb\n", val2);
    printf("---------------------\n");
    printf("    %sb\n", val3);
    free(val1);
    free(val2);
    free(val3);
}


void xorSample() {
    Exception ex = NO_EXCEPTION;
    Bits16 first = Bits16_fromString("1001", &ex);
    Bits16 second = Bits16_fromString("1010", &ex);
    Bits16 result = first ^ second;
    char* val1 = Bits16_toString(first);
    char* val2 = Bits16_toString(second);
    char* val3 = Bits16_toString(result);
    printf("\n    %sb\n", val1);
    printf("XOR %sb\n", val2);
    printf("---------------------\n");
    printf("    %sb\n", val3);
    free(val1);
    free(val2);
    free(val3);
}


void notSample() {
    Exception ex = NO_EXCEPTION;
    Bits16 first = Bits16_fromString("1001", &ex);
    Bits16 result = ~first;

    size_t count = Bits16_countOnes(result);

    char* val1 = Bits16_toString(first);
    char* val3 = Bits16_toString(result);
    printf("\nNOT %sb\n", val1);
    printf("---------------------\n");
    printf("    %sb\n", val3);
    printf("Number of 1 bits in result: %d\n", count);
    free(val1);
    free(val3);
}


void shiftLeftSample() {
    Exception ex = NO_EXCEPTION;
    Bits16 first = Bits16_fromString("1001", &ex);
    Bits16 result = first << 3;
    char* val1 = Bits16_toString(first);
    char* val3 = Bits16_toString(result);
    printf("\nSHL 3 %sb\n", val1);
    printf("---------------------\n");
    printf("      %sb\n", val3);
    free(val1);
    free(val3);
}


BitsSample::BitsSample() : Sample("Bits sample") {
}

void BitsSample::run() {
    Exception ex = NO_EXCEPTION;
    Bits16 inputValue = Bits16_fromString("1001000110100", &ex);
    printf("Input : 0x%04X\n", inputValue);

    char* value = Bits16_toString(0x1234);
    printf("Result: %sb\n", value);
    free(value);

    orSample();
    andSample();
    xorSample();
    notSample();
    shiftLeftSample();
}
