#include "add.h"

static const char* value = "Hello World!";

void put(char c) {
    char* vid = (char*) 0xb8000;
    *vid = c;
}

int main() {
  put('A');

  int result = add(1, 3);
  int* res = &result;
/*  char* p = value;*/
/*  while(*p) p++;*/
  return 0;
}
