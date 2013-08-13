#include <stdio.h>


typedef unsigned char BYTE;
typedef unsigned int DWORD;

struct PartitionEntry {
    BYTE status;
    BYTE chsFirst[3];
    BYTE type;
    BYTE chsLast[3];
    DWORD firstLBA;
    DWORD sectors;
};


struct MBR {
    BYTE bootstrap[446];
    struct PartitionEntry partition[4];
    BYTE sig55;
    BYTE sigAA;
} __attribute__((packed));

struct MBR mbr;
FILE* diskImage;
BYTE bootSect[512];

void readBootsect(int part) {
   FILE* result = NULL;
   DWORD offset = 0x2000 + mbr.partition[part].firstLBA * 512;

   fseek(diskImage, offset, SEEK_SET);
   fread(&bootSect, sizeof(bootSect), 1, diskImage);

   result = fopen("result.bin", "wb");
   fwrite(&bootSect, sizeof(bootSect), 1, result);
   fclose(result); 
}

void writeBootsect(int part) {
   FILE* source = NULL;
   DWORD offset = 0x2000 + mbr.partition[part].firstLBA * 512;

   source = fopen("source.bin", "rb");
   fread(bootSect, 1, sizeof(bootSect), source);
   fclose(source); 

   fseek(diskImage, offset, SEEK_SET);
   fwrite(bootSect, sizeof(bootSect), 1, diskImage);
}


int main(int argc, char* argv[]) {
   if (argc > 1) {
       char* fileName = argv[1];

       printf("Partition entry size: %ld\n", sizeof(struct PartitionEntry));
       printf("MBR size: %ld\n", sizeof(struct MBR));

       diskImage = fopen(fileName, "r+b");
       if (diskImage) { 
          int part = 0;

          fseek(diskImage, 0x2000, SEEK_SET);
          fread(&mbr, sizeof(mbr), 1, diskImage);
          printf("MBR contents:\n");
	  for (part = 0;  part < 4;  part++) {
              printf("  Partition %d\n", part);
              printf("    Status: %02x\n", mbr.partition[part].status);
              printf("    Type: %d\n", mbr.partition[part].type);
              printf("    First LBA: %08x\n", mbr.partition[part].firstLBA);
              printf("    Sectors: %d\n", mbr.partition[part].sectors);
              printf("    Size: %d MB\n", mbr.partition[part].sectors / 2048);
          }
          printf("  Signature: %02x%02x\n", mbr.sig55, mbr.sigAA);

          readBootsect(0);
          writeBootsect(0);

          fclose(diskImage);
       }
   } 


   return 0;
}
