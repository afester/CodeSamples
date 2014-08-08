#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

void dumpLine(unsigned int addr, const char* hexDump, const char* asciiDump) {
    printf("%08X: %48s %s\n", addr, hexDump, asciiDump);
}

void dumpBuffer(const char* buffer, ssize_t size) {
   int addr = 0;
   char hexDumpStr[100];
   char* hexDump = hexDumpStr;
   char asciiDumpStr[100];
   char* asciiDump = asciiDumpStr;
   int column = 0;
   int index = 0;

   while(index < size) {
     char c = buffer[index];

     sprintf(hexDump, "%02X ", c);
     hexDump += 3;

     if (c > 31 && c < 128) {
        *asciiDump = c;
     } else {
        *asciiDump = '.';
     }
     asciiDump++;
     *asciiDump = 0;

     column++;
     if ( (column % 16) == 0) {
        dumpLine(addr, hexDumpStr, asciiDumpStr);

        asciiDump = asciiDumpStr;
        hexDump = hexDumpStr;
        column = 0;
        addr += 16;
     }
   }

   if (column > 0) {
      dumpLine(addr, hexDumpStr, asciiDumpStr);
   }
}


int main() {
   ssize_t siz = 0;
   char buf[2000];

   int fd = open("/tmp/x", O_RDONLY);
   if (fd < 0) {
      perror("open");
   }
   printf("fd=%d\n", fd);
   siz = read(fd, buf, 1024);
   printf("size=%d\n", siz);

   dumpBuffer(buf, siz);

   close(fd);
}
