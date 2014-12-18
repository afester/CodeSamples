#include <stdio.h>

void addTwice(int* a, int* b) {
//        pushq   %rbp
//        movq    %rsp, %rbp

//        movq    %rdi, -8(%rbp)	; a
//        movq    %rsi, -16(%rbp)	; b

   *a += *b;
//        movq    -8(%rbp), %rax	
//        movl    (%rax), %edx		; *a => %edx
//        movq    -16(%rbp), %rax
//        movl    (%rax), %eax		; *a => %eax
//        addl    %eax, %edx
//        movq    -8(%rbp), %rax
//        movl    %edx, (%rax)		; %edx => *a

   *a += *b;
//        movq    -8(%rbp), %rax
//        movl    (%rax), %edx		; *a => %edx
//        movq    -16(%rbp), %rax
//        movl    (%rax), %eax		; *b => %eax
//        addl    %eax, %edx
//        movq    -8(%rbp), %rax
//        movl    %edx, (%rax)		; %edx => *a
}
//        popq    %rbp 
//        ret


void addTwice2(int*restrict a, int*restrict b) {
   *a += *b;
   *a += *b;
}

int main() {
   int a = 1;
   int b = 2;

   addTwice(&a, &b);
   printf("a=%d, b=%d\n", a, b);

   addTwice2(&a, &b);
   printf("a=%d, b=%d\n", a, b);

   int c = 2;
   addTwice(&c, &c);
   printf("c=%d\n", c);

   int d = 2;
   addTwice2(&d, &d);
   printf("d=%d\n", d);

   return 0;
}

