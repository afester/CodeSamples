#include <stdio.h>

static const unsigned int MAXFIB = 4000000;

/* By considering the terms in the Fibonacci sequence whose values do not
   exceed four million, find the sum of the even-valued terms. */
int main2() {
    unsigned int fib = 0;
    unsigned int n1 = 0;
    unsigned int n2 = 1;
    unsigned int sum = 0;

    do {
        fib = n1 + n2;
        if (fib < MAXFIB) {
            if ( (fib % 2) == 0) {
                sum = sum + fib;
            }
            n1 = n2;
            n2 = fib;
        }
    }while(fib < MAXFIB);
    printf("%d\n", sum);
}


unsigned int sum = 0;

unsigned int fib(unsigned int n1, unsigned int n2) {
    unsigned int current = n1 + n2;
    if (current < MAXFIB) {
        printf("%d\n", current);
        if ( (current % 2) == 0) {
            sum = sum + current;
        }
        fib(n2, current);
    }
}

int main() {
    fib(0, 1);
    printf("%d\n", sum);
}
