/**
 * Compile with:
 * $ gcc -Wno-unused-variable  -Wall -pedantic -c const.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char arr[11];

void f(const arr* val) {
}

void f1(arr* val) {
}

void f2(char* val) {
}

void f3(char** val) {
}

void f4(const char** val) {
}

typedef struct _type11 { char arr[11]; } type11;

void f5(type11* param) {
   (*param).arr[3] = 'A';
}

void f6(const type11* param) {
   /* error: assignment of read-only location 'param->arr[3]' */
   /* param->arr[3] = 'A';*/
}

int main() {
   arr* data = calloc(sizeof(arr), 1);
   type11* data2 = calloc(sizeof(type11), 1);
   char* str = calloc(100, 1);
   char** strPtr = &str;
   strcpy(str, "Hello World");

   /* warning: passing argument 1 of 'f' from incompatible pointer type [enabled by default] */
   /* note: expected 'const char (*)[11]' but argument is of type 'char (*)[11]'             */
   f(data);

   f1(data);

   f2(str);

   f3(strPtr);

   /* warning: passing argument 1 of 'f4' from incompatible pointer type [enabled by default] */
   /* note: expected 'const char **' but argument is of type 'char **'                        */
   f4(strPtr);

   f5(data2);

   f6(data2);

   return 0;
}
