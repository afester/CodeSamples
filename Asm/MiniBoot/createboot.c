#include <stdio.h>
#include <stdlib.h>


typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned long long QWORD;

struct _VDIHeader {
   char desc[0x40];
   DWORD signature;	/* 0x0040 */
   WORD versionMinor;
   WORD versionMajor;
   DWORD headerSize;
   DWORD imageType;
   DWORD imageFlags;	/* 0x0050 */
   char desc2[252];
   DWORD unknown;
   DWORD offsetBlocks;
   DWORD offsetData;
   DWORD cylinders;
   DWORD heads;
   DWORD sectors;
   DWORD sectorSize;
   DWORD unused;
   QWORD diskSize;
   DWORD blockSize;



/*
0170      00 00 00 78 00 00 00 00                            DiskSize (Bytes)
                                  00 00 10 00                BlockSize
                                              00 00 00 00    Block Extra Data (0)
0180      80 07 00 00                                        #BlocksInHDD
                      0B 02 00 00                            #BlocksAllocated
                                  5A 08 62 27 A8 B6 69 44    UUID of this VDI
0190      A1 57 E2 B2 43 A5 8F CB
                                  0C 5C B1 E3 C5 73 ED 40    UUID of last SNAP
01A0      AE F7 06 D6 20 69 0C 96
                                  00 00 00 00 00 00 00 00    UUID link
01B0      00 00 00 00 00 00 00 00
                                  00 00 00 00 00 00 00 00    UUID Parent
01C0      00 00 00 00 00 00 00 00
                                  CF 03 00 00 00 00 00 00    -- garbage / unused --
01D0      3F 00 00 00 00 02 00 00 00 00 00 00 00 00 00 00    -- garbage / unused --
01E0      00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    -- unused --
01F0      00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    -- unused 
*/
} __attribute__((packed));
typedef struct _VDIHeader VDIHeader;

struct _PartitionEntry {
    BYTE status;
    DWORD chsFirst : 24;
    BYTE type;
    DWORD chsLast : 24;
    DWORD firstLBA;
    DWORD sectors;
} __attribute__((packed));
typedef struct _PartitionEntry PartitionEntry;

const PART_ACTIVE = 0x80;
const FAT16B = 0x06;
const FAT32_CHS = 0x0B;
const FAT32_LBA = 0x0C;

struct _MBR {
    BYTE bootstrap[446];
    PartitionEntry partition[4];
    BYTE sig55;
    BYTE sigAA;
} __attribute__((packed));
typedef struct _MBR MBR;


MBR mbr;
FILE* diskImage;
BYTE bootSect[512];

void readBootsect(int part) {
   FILE* result = NULL;
   DWORD offset = 0x2000 + mbr.partition[part].firstLBA * 512;

   /* read the current boot sector from the vdi image */
   fseek(diskImage, offset, SEEK_SET);
   fread(&bootSect, sizeof(bootSect), 1, diskImage);

   /* backup the current boot sector into result.bin */
   result = fopen("result.bin", "wb");
   fwrite(&bootSect, sizeof(bootSect), 1, result);
   fclose(result); 
}

void writeBootsect(int part) {
   FILE* source = NULL;
   DWORD offset = 0x2000 + mbr.partition[part].firstLBA * 512;

   /* read the machine code from source.bin into the bootSect buffer */
   source = fopen("source.bin", "rb");
   fread(bootSect, 1, sizeof(bootSect), source);
   fclose(source); 

   /* write the complete boot sector with the bootloader instructions into the vdi image */
   fseek(diskImage, offset, SEEK_SET);
   fwrite(bootSect, sizeof(bootSect), 1, diskImage);
}


VDIHeader* readVDIHeader() {
   VDIHeader* result = malloc(sizeof(VDIHeader));
   fseek(diskImage, 0, SEEK_SET);
   fread(result, 1, sizeof(VDIHeader), diskImage);
   return result;
}

void dumpVDIHeader(VDIHeader* vdi) {
   printf("VDI header:\n");
   printf("   Vendor       : \"%s\"\n", vdi->desc);
   printf("   Signature    : 0x%08x\n", vdi->signature);
   printf("   Version      : %d.%d\n", vdi->versionMajor, vdi->versionMinor);
   printf("   Header size  : %d (0x%02x)\n", vdi->headerSize, vdi->headerSize);
   printf("   Image type   : %08x\n", vdi->imageType);
   printf("   Image flags  : 0x%08x\n", vdi->imageFlags);
   printf("   Description  : \"%s\"\n", vdi->desc2);
   printf("   Offset blocks: 0x%08x\n", vdi->offsetBlocks);
   printf("   Offset data  : 0x%08x\n", vdi->offsetData);
   printf("   Cylinders    : %d\n", vdi->cylinders);
   printf("   Heads        : %d\n", vdi->heads);
   printf("   Sectors      : %d\n", vdi->sectors);
   printf("   Sector size  : %d\n", vdi->sectorSize);
   printf("   Disk size    : %ld bytes (%d KiB / %d MiB)\n", vdi->diskSize,vdi->diskSize / 1024, vdi->diskSize / 1024 / 1024);
   printf("   Block size   : %d bytes (%d KiB / %d MiB)\n", vdi->blockSize,vdi->blockSize / 1024, vdi->blockSize / 1024 / 1024);
   printf("\n");
}


void setupMBR(MBR* mbr) {
   FILE* source = NULL;
   PartitionEntry* p0 = NULL;

   /* read the machine code from source.bin into the first part of the MBR */
   source = fopen("source.bin", "rb");
   fread(mbr->bootstrap, 1, sizeof(mbr->bootstrap), source);
   fclose(source); 

   /* setup the partition table */
   p0 = &mbr->partition[0];
   p0->status = PART_ACTIVE;
   p0->type = FAT32_LBA;
   p0->chsFirst = 0;
   p0->chsLast = 0;
   p0->firstLBA = 1;
   p0->sectors = 32*2048;	/* 32 MiB */
   mbr->sig55 = 0x55;
   mbr->sigAA = 0xAA;
}


void writeMBR(MBR* mbr, VDIHeader* vdi) {
   /* write the complete boot sector with the bootloader instructions into the vdi image */
   fseek(diskImage, vdi->offsetData, SEEK_SET);
   fwrite((void*) mbr, sizeof(MBR), 1, diskImage);
}

void dumpMBR(MBR* mbr) {
   int part = 0;

   /* Dump the MBR */
   printf("MBR contents:\n");
   for (part = 0;  part < 4;  part++) {
      printf("  Partition %d\n", part);
      printf("    Status: %02x\n", mbr->partition[part].status);
      printf("    Type: %02x\n", mbr->partition[part].type);
      printf("    First LBA: %08x\n", mbr->partition[part].firstLBA);
      printf("    Sectors: %d\n", mbr->partition[part].sectors);
      printf("    Size: %d MB\n", mbr->partition[part].sectors / 2048);
   }
   printf("  Signature: %02x%02x\n", mbr->sig55, mbr->sigAA);
}


int main(int argc, char* argv[]) {
   if (argc > 1) {
       char* fileName = argv[1];

       printf("Partition entry size: %ld\n", sizeof(PartitionEntry));
       printf("MBR size: %ld\n", sizeof( MBR));

       diskImage = fopen(fileName, "r+b");
       if (diskImage) { 
          VDIHeader* h = readVDIHeader();
          dumpVDIHeader(h);

          /* skip over virtualbox dvi header */
          fseek(diskImage, 0x2000, SEEK_SET);

          /* read MBR */
          fread(&mbr, sizeof(mbr), 1, diskImage);
          dumpMBR(&mbr);

          /* check signature */
          if (mbr.sig55 != 0x55 || mbr.sigAA != 0xAA) {
             printf("WARNING: NO VALID MBR FOUND!");
          }

          setupMBR(&mbr);
          writeMBR(&mbr, h);

          free(h);
/*          readBootsect(0);*/
/*          writeBootsect(0);*/

          fclose(diskImage);
       }
   } 


   return 0;
}
