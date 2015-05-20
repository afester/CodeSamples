#include "add.h"

static const char* value = "Hello World!";

int main();

void _start() {
   main();
}


int main() {
  int result = add(1, 3);
  int* res = &result;
/*  char* p = value;*/
/*  while(*p) p++;*/
  return 0;
}
