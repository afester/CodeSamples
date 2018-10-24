#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Formats a numeric value into a string using a specified number of decimal places.
 *
 * @param value
 * @param decimals
 * @param result
 */
void strFormat(unsigned int value, int decimals, char* result) {
    // convert value to string
    static char buffer[10];
    itoa(value, buffer, 10);    // n, nn, nnn, ...
    size_t digitCount = strlen(buffer);

    // calculate decimal point position.
    // A value < 1 shows that at least one leading zero needs to be prepended.
    int decPos = digitCount - decimals;

    // calculate number of leading zeroes
    size_t zeroes = 0;
    if (decPos < 1) {
        zeroes = 1 - decPos;
        decPos = 1;
    }

    // digitCount: number of digits in the original number
    // zeroes: number of leading zeroes in result
    // decPos: index of decimal point in result
    // zeroes + digitCount:  // overall number of digits in result (excl. decimal point)

    // create final result - fill with zeroes, add decimal point, add original digits
    int dest = 0;
    int idx = 0;
    while(digitCount > 0) {
        if (zeroes > 0) {
            result[dest++] = '0';
            zeroes--;
        } else{
            result[dest++] = buffer[idx++];
            digitCount--;
        }
        if (dest == decPos && digitCount > 0) {
            result[dest++] = ',';
        }
    }
    result[dest] = 0;
}


void testFormat(unsigned int value, int decimals, const char* const expected) {
    char buffer[20];

    strFormat(value, decimals, buffer);
    printf("%d (%d dec) => \"%s\" %s\n", value, decimals, buffer, strcmp(buffer, expected) ? "[FAIL]" : "[OK]");
}

int main() {
    testFormat(0, 0, "0");
    testFormat(0, 1, "0,0");
    testFormat(0, 2, "0,00");
    testFormat(0, 3, "0,000");

    testFormat(8, 0, "8");
    testFormat(8, 1, "0,8");
    testFormat(8, 2, "0,08");
    testFormat(8, 3, "0,008");

    testFormat(42, 0, "42");
    testFormat(42, 1, "4,2");
    testFormat(42, 2, "0,42");
    testFormat(42, 3, "0,042");

    testFormat(642, 0, "642");
    testFormat(642, 1, "64,2");
    testFormat(642, 2, "6,42");
    testFormat(642, 3, "0,642");

    testFormat(1642, 0, "1642");
    testFormat(1642, 1, "164,2");
    testFormat(1642, 2, "16,42");
    testFormat(1642, 3, "1,642");

    testFormat(0, 1, "0,0");
    testFormat(125, 1, "12,5");
    testFormat(270, 1, "27,0");
    testFormat(350, 1, "35,0");

    testFormat(0, 2, "0,00");
    testFormat(8, 2, "0,08");
    testFormat(45, 2, "0,45");
    testFormat(125, 2, "1,25");
    testFormat(270, 2, "2,70");
    testFormat(300, 2, "3,00");

    testFormat(123456, 5, "1,23456");
    testFormat(65432, 5, "0,65432");
    testFormat(12345678, 3, "12345,678");
    testFormat(12345678, 6, "12,345678");

    testFormat(300, 5, "0,00300");

    return 0;
}
