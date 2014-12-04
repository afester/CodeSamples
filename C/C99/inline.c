/**
 * gcc -pedantic -Wall -O -std=c99 -Winline -S  inline.c -o inline.S
 *
 * The C99 inline keyword is treated as hint only - the compiler
 * can still decide NOT to inline the function.
 * Use -Winline to check which functions are NOT inlined, and why.
 */

#include <stdio.h>
#include <stdlib.h>

void printHello() {
    printf("Hello World!\n");
}

static inline void printHelloI() {
    printf("Hello World!\n");
}


void inlineTest() {

    for (int i = 0;  i < 10;  i++) {
        printHello();
        printHelloI();
    }

/**********************
        movl    $10, %ebx
L4:
        call    _printHello

        movl    $LC0, (%esp)
        call    _printf

        decl    %ebx
        jne     L4
**********************/
}
