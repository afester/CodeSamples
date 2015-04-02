#include <stdio.h>

int main(int argc, char* argv[]) {
    FILE* f = fopen("/tmp/crash", "w");
    if (f) {
       for (int i = 0;  i < argc;  i++) {
          fprintf(f, "%s\n", argv[i]);
       }
       fclose(f);
    }

    return 0;
}
