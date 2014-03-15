#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define KB (1024)
#define MB (KB * KB)
#define GB (KB * KB * KB)

int main() {
   char* ptr = malloc(10 * GB);
   printf("%p\n", ptr);
   memset(ptr, 0, 10 * GB);

   return 0;
}

