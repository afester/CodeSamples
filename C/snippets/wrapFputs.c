#include <stdio.h>


int catched = 0;

int __wrap_fputs ( const char * str, FILE * stream ) {
  if (catched) {
    printf("[CATCHED] %s", str);
  } else {
    __real_fputs(str, stream);
  }
}

