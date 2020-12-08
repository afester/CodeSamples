#include <stdio.h>

int main() {
    int result = 0;
    int number = 0;
    for (number = 3; number < 1000;  number++) {
        if ( !(number % 3) || !(number % 5) ) {
            result += number;
        }
    }

    printf("Result for Euler Problem #1: %d\n", result);

    return 0;
}
