#include <stdio.h>

/* brute force attack for Euler#1 */
int main() {
    char numbers[10]; /* 0 ... 999 */
    int result = 0;
    int i = 0;

    /* initialize array */
    for (i = 0; i < sizeof(numbers); i++) {
        numbers[i] = 0;
    }

    /* tag multples of 3 */
    for (i = 3; i < sizeof(numbers); i += 3) {
        numbers[i] = 1;
    }

    /* tag multiples of 5 */
    for (i = 5; i < sizeof(numbers); i+= 5) {
        numbers[i] = 1;
    }

    /* sum all tagged numbers */
    result = 0;
    for (i = 0; i < sizeof(numbers); i++) {
        if (numbers[i]) {
            result += i;
        }
    }
    printf("Result for Euler Problem #1: %d\n", result);

    return 0;
}
