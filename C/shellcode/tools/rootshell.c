#include <stdlib.h>
#include <unistd.h>

int main(){
  char *name[2];

  name[0] = "/bin/sh";
  name[1] = 0x0;
  execve(name[0], name, NULL);
  exit(0);
}
