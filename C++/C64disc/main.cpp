#include <iostream>
#include <iomanip>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include "HexDump.h"

using namespace std;


typedef struct Sector {
    uint8_t nextTrack;
    uint8_t nextSector;
    uint8_t data[254];
}_Sector;

typedef struct AllocationMap {
    uint8_t freeSectors;
    uint8_t allocMap[3];
}__attribute__((packed)) _AllocationMap;

typedef struct Bam {
    uint8_t nextTrack;
    uint8_t nextSector;
    uint8_t formatId;
    uint8_t doubleSide;
    AllocationMap allocationMap[35];
    uint8_t discName[16];
    uint8_t fill1[2];
    char discId[2];
    uint8_t fill2;
    uint8_t dosVersion;
    uint8_t formatIdCopy;
    uint8_t fill3[4];
    uint8_t driveMode1[4];
    uint8_t driveMode2[4];
    uint8_t driveMode3;
    uint8_t unused[76];
}__attribute__((packed)) _Bam2;

typedef struct FileEntry {
    uint8_t fileType;
    uint8_t firstTrack;
    uint8_t firstSector;
    uint8_t fileName[16];
    uint8_t reserved[9];
    uint16_t blockCount;
}_FileEntry;

typedef struct DirEntry {
    uint8_t nextTrack;  // only used in first entry!
    uint8_t nextSector; // only used in first entry!
    FileEntry file;
}_DirEntry;

typedef struct Dir {
    DirEntry entry[8];
}_Dir;

FILE* imageFile;


const char* getType(uint8_t typeId) {
    switch(typeId) {
        case 0x80 : return "DEL";
        case 0x81 : return "SEQ";
        case 0x82 : return "PRG";
        case 0x83 : return "USR";
        case 0x84 : return "REL";
    }

    return "???";
}

// NOTE: NOT REENTRANT!
const char* getName(uint8_t* name) {
    static uint8_t result[17];
    memcpy(result, name, 16);
    result[16] = 0;
    for (int idx = 0;  idx < 16;  idx++) {
        if (result[idx] == 0xa0) {
            result[idx] = ' ';
        }
    }
    return (char*) result;
}


static inline string rtrim(std::string &s) {
   size_t beg = s.find_first_not_of(" \r\n");
   return (beg == string::npos) ? "" : s.substr(beg, s.find_last_not_of(" \r\n") - beg + 1);
}

string toBinary(uint8_t value) {
    static char result[9];
    uint8_t mask = 0x01;

    for (int idx = 0;  idx < 8;  idx++) {
        if (value & mask) {
            result[idx] = '1';
        } else {
            result[idx] = '0';
        }
        mask = mask << 1;
    }
    result[8] = 0;
    return result;
}

//  1..17 => 17 tracks * 21 sectors = 357 blocks
// 18..24 =>  7 tracks * 19 sectors = 133 blocks (Track 18 is directory/bam)
// 25..30 =>  6 tracks * 18 sectors = 108 blocks
// 31..25 =>  5 tracks * 17 sectors = 85 blocks
//           ==========           ======
//           35 tracks               683 blocks (-19 = 664 data blocks)
// BAM: 4 bytes per track ; 4 * 35 = 140 bytes

Sector* readSector(int track, int sector) {

    // calculate the sector number
    int absoluteSector = 0;

    if (track < 18) {
        // zone 1
        absoluteSector = (track - 1) * 21 + sector;
    } else if (track < 25) {
        // zone 2
        absoluteSector = (17 * 21) + (track - 18) * 19 + sector;
    } else if (track < 31) {
        // zone 3
        absoluteSector = (17 * 21) + (7 * 19) + (track - 25) * 18 + sector;
    } else {
        // zone 4
        absoluteSector = (17 * 21) + (7 * 19) + (6 * 18) + (track - 31) * 17 + sector;
    }
//    printf("Absolute sector: %d\n", absoluteSector);
    int sectorOffset = 256 * absoluteSector;
//    printf("Sector offset  : %d (%x)\n", sectorOffset, sectorOffset);

    cout << "R " << track<<","<<sector<<endl;
    Sector* result = (Sector*) calloc(1, sizeof(Sector));
    fseek(imageFile, sectorOffset, SEEK_SET);
    fread(result, sizeof(Sector), 1, imageFile);
    return result;
}

void writeSector(Sector* sect, int track, int sector) {
    // calculate the sector number
    int absoluteSector = 0;

    if (track < 18) {
        // zone 1
        absoluteSector = (track - 1) * 21 + sector;
    } else if (track < 25) {
        // zone 2
        absoluteSector = (17 * 21) + (track - 18) * 19 + sector;
    } else if (track < 31) {
        // zone 3
        absoluteSector = (17 * 21) + (7 * 19) + (track - 25) * 18 + sector;
    } else {
        // zone 4
        absoluteSector = (17 * 21) + (7 * 19) + (6 * 18) + (track - 31) * 17 + sector;
    }
//    printf("Absolute sector: %d\n", absoluteSector);
    int sectorOffset = 256 * absoluteSector;
//    printf("Sector offset  : %d (%x)\n", sectorOffset, sectorOffset);

//    cerr << "W:" << sectorOffset << endl;
    cout << "W " << track<<","<<sector<<endl;
    fseek(imageFile, sectorOffset, SEEK_SET);
    fwrite(sect, sizeof(Sector), 1, imageFile);
    fflush(imageFile);
}

void dumpSector(Sector* sector) {
    HexDump* dump = new_HexDump((uint8_t*)sector, sizeof(Sector));
    HexDump_dumpAll(dump);
    free(dump);
}

void dumpBam(Bam* bam) {
    printf("Next Track/Sector: %d/%d\n", bam->nextTrack, bam->nextSector);
    char discName[17];
    memcpy(discName, bam->discName, 16);
    discName[16] = 0;
    printf("Disk name        : %s\n", discName);
    printf("Format id        : %d\n", bam->formatId);
    printf("Double sided     : %d\n", bam->doubleSide);
    printf("Disc id          : %c%c\n", bam->discId[0], bam->discId[1]);
    printf("DOS version      : %c\n", bam->dosVersion);
    printf("Format id copy   : %d\n", bam->formatIdCopy);
    printf("Drive mode       : %d/%d/%d\n", bam->driveMode1[0], bam->driveMode2[0], bam->driveMode3);

    for (int idx = 0;  idx < 35;  idx++) {
        printf("%2d.  %d  %s %s %s\n", idx + 1, bam->allocationMap[idx].freeSectors,
               toBinary(bam->allocationMap[idx].allocMap[0]).c_str(),
               toBinary(bam->allocationMap[idx].allocMap[1]).c_str(),
               toBinary(bam->allocationMap[idx].allocMap[2]).c_str());
    }
}

uint16_t getFreeBlocks(Bam* bam) {
    uint16_t result = 0;
    for (int idx = 0;  idx < 35;  idx++) {
        if (idx != 17) {    // Track 18 is BAM/Directory
            result += bam->allocationMap[idx].freeSectors;
        }
    }
    return result;
}

void dumpDir(Dir* dir) {
    printf("Next Track/Sector: %d/%d\n", dir->entry[0].nextTrack, dir->entry[0].nextSector);

    for (int idx = 0;  idx < 8;  idx++) {
        FileEntry* fileEntry = &dir->entry[idx].file;
        char fileName[17];
        memcpy(fileName, fileEntry->fileName, 16);
        fileName[16] = 0;

        printf("%s  %16s  %d (%d/%d)\n",
               getType(fileEntry->fileType), fileName,
               fileEntry->blockCount, fileEntry->firstTrack, fileEntry->firstSector);
    }
}


void listContents() {
    Bam* bam = (Bam*) readSector(18, 0);

    printf("\"%16s\"\n", getName(bam->discName));
    printf("------------------------------------\n");

    uint8_t nextTrack = bam->nextTrack;
    uint8_t nextSector = bam->nextSector;
    while(nextTrack != 0 && nextSector != 255) {
        Dir* dir = (Dir*) readSector(nextTrack, nextSector);
        for (int idx = 0;  idx < 8;  idx++) {
            FileEntry* fileEntry = &dir->entry[idx].file;

            if (fileEntry->fileType != 0) {
                printf("%s  \"%16s\"  %d\n", getType(fileEntry->fileType), getName(fileEntry->fileName), fileEntry->blockCount);
            }
        }
        nextTrack = dir->entry[0].nextTrack;
        nextSector = dir->entry[0].nextSector;
    }
}


FileEntry* searchFile(const char* fileName) {
    Bam* bam = (Bam*) readSector(18, 0);
    uint8_t nextTrack = bam->nextTrack;
    uint8_t nextSector = bam->nextSector;
    free(bam);

    while(nextTrack != 0 && nextSector != 255) {
        Dir* dir = (Dir*) readSector(nextTrack, nextSector);
        for (int idx = 0;  idx < 8;  idx++) {
            FileEntry* fileEntry = &dir->entry[idx].file;
            if (fileEntry->fileType != 0) {
                string entryName = getName(fileEntry->fileName);
                entryName = rtrim(entryName);
                if (entryName == fileName) {
                    return fileEntry;
                }
            }
        }
        nextTrack = dir->entry[0].nextTrack;
        nextSector = dir->entry[0].nextSector;
    }

    return 0;
}


Dir* searchFile(const string& fileName, uint8_t& idx) {
    Bam* bam = (Bam*) readSector(18, 0);
    uint8_t nextTrack = bam->nextTrack;
    uint8_t nextSector = bam->nextSector;
    free(bam);

    while(nextTrack != 0 && nextSector != 255) {
        Dir* dir = (Dir*) readSector(nextTrack, nextSector);
        for (idx = 0;  idx < 8;  idx++) {
            FileEntry* fileEntry = &dir->entry[idx].file;
            if (fileEntry->fileType != 0) {
                string entryName = getName(fileEntry->fileName);
                entryName = rtrim(entryName);
                if (entryName == fileName) {
                    return dir;
                }
            }
        }
        nextTrack = dir->entry[0].nextTrack;
        nextSector = dir->entry[0].nextSector;
        free(dir);
    }

    return 0;
}

FileEntry* createEntry(const string& fileName) {
    Bam* bam = (Bam*) readSector(18, 0);
    uint8_t nextTrack = bam->nextTrack;
    uint8_t nextSector = bam->nextSector;
    while(nextTrack != 0 && nextSector != 255) {
        Dir* dir = (Dir*) readSector(nextTrack, nextSector);
        for (int idx = 0;  idx < 8;  idx++) {
            FileEntry* fileEntry = &dir->entry[idx].file;
            if (fileEntry->fileType == 0) {
                fileEntry->fileType = 0x82;
                strcpy((char*) fileEntry->fileName, fileName.c_str());
                for (int idx = strlen((char*) fileEntry->fileName);  idx < 16;  idx++) {
                    fileEntry->fileName[idx] = 0xA0;
                }
                fileEntry->blockCount = 0;
                fileEntry->firstSector = 0;
                fileEntry->firstTrack = 0;

                //cout << "WRITING ..." << (int) nextTrack  << "," << (int) nextSector << endl;
                //dumpDir(dir);
                writeSector((Sector*) dir, nextTrack, nextSector);
                return fileEntry;
            }
        }
        nextTrack = dir->entry[0].nextTrack;
        nextSector = dir->entry[0].nextSector;
    }

    return 0;
}

void dumpFile(FileEntry* entry) {
    uint8_t nextTrack = entry->firstTrack;
    uint8_t nextSector = entry->firstSector;
    while(nextTrack != 0) {

        Sector* sec = readSector(nextTrack, nextSector);
        nextTrack = sec->nextTrack;
        nextSector = sec->nextSector;
        int count = 254;
        if (nextTrack == 0) {
            count = nextSector - 1;
        }
        printf("%d bytes\n", count);
        HexDump* dump = new_HexDump((uint8_t*)sec + 2, sizeof(Sector) - 2);
        HexDump_dumpAll(dump);
        free(dump);

        nextTrack = sec->nextTrack;
        nextSector = sec->nextSector;
    }
}

std::vector<std::string> split(const std::string &text, char sep) {
  std::vector<std::string> tokens;
  std::size_t start = 0, end = 0;
  while ((end = text.find(sep, start)) != std::string::npos) {
    tokens.push_back(text.substr(start, end - start));
    start = end + 1;
  }
  tokens.push_back(text.substr(start));
  return tokens;
}


static string prompt = "> ";

void mount(const string& fileName) {
    if (imageFile != 0) {
        fclose(imageFile);
    }
    cout << "Mounting " << fileName << endl;
    imageFile = fopen(fileName.c_str(), "r+b");
    if (imageFile == 0) {
        cout << "Could not mount " << fileName << endl;
    } else {
        size_t pos = fileName.rfind("\\");
        if (pos == string::npos) {
            pos = fileName.rfind("/");
        }
        if (pos != string::npos) {
            prompt = fileName.substr(pos + 1) + "> ";
        } else {
            prompt = fileName + "> ";
        }
    }
}

void allocateBlock(uint8_t& trackResult, uint8_t& sectorResult) {
    Bam* bam = (Bam*) readSector(18, 0);
 //   dumpBam(bam);

    int track = -1;
    int sector = -1;
    // simple allocation algorithm - probably the 1541 uses a different one!
    for (int idx = 16;  track == -1 && idx >= 0;  idx--) {
        if (bam->allocationMap[idx].freeSectors > 0) {
            track = idx;

            for (int mapIdx = 0; mapIdx < 3;  mapIdx++) {
                sector = mapIdx * 8;
                if (bam->allocationMap[track].allocMap[mapIdx] > 0) {
                    // this map portion contains at least one non-allocated block
                    uint8_t mask = 0x01;
                    while((bam->allocationMap[track].allocMap[mapIdx] & mask) == 0) {
                       sector++;
                       mask = mask << 1;
                    }

                    bam->allocationMap[track].allocMap[mapIdx] &= ~mask;
                    bam->allocationMap[track].freeSectors -= 1;
                    break;
                }
            }
        }
    }

// TODO!!!
//    for (int idx = 18;  track == -1 && idx < 35;  idx++) {
//        if (bam->allocationMap[idx].freeSectors > 0) {
//            track = idx;
//        }
//    }

 //   dumpBam(bam);

    writeSector((Sector*) bam, 18, 0);

    trackResult = track + 1;
    sectorResult = sector;
    cout << "Using Track :" << (int) trackResult << endl;
    cout << "Using Sector:" << (int) sectorResult << endl;
}

// Algorithm:
// - calculate number of blocks
// - track=entry->firstTrack, sector=entry->firstSector

// - loop number of blocks
//   - does track/sector already exist? (track != 0)?
//      then read track/sector
//      else calloc sector (sector->nextTrack=0, sector->nextSector = 0)

//   - read next bunch of bytes (1..254) into sector->data
// Link to next sector(!!)
//   - last block? Then set sector->nextTrack=0, sector->nextSector=number of bytes
//   - else: if sector->nextTrack = 0 then allocate new sector, set sector->nextTrack/sector->nextSector
//   - write sector

//   - track = sector->nextTrack, sector=sector->nextSector

void copyIn(const string& srcFile, const string& dstFile) {
    // hack: look up the destination file
    // make sure at least one block is allocated
    // set the first sector of the destination file to the
    // first 254 bytes of the source file

    uint8_t entryIdx = 0;
    Dir* dir = searchFile(dstFile, entryIdx);
    if (dir == 0) {
        cout << "Destination file \"" << dstFile << "\" not found." << endl;
        return;
    }
    FileEntry* entry = &dir->entry[entryIdx].file;

    FILE* src = fopen(srcFile.c_str(), "rb");
    if (src == 0) {
        cout << "Source file \"" << srcFile << "\" not found." << endl;
        return;
    }

    // obtain the file size
    fseek(src, 0L, SEEK_END);
    long fileSize = ftell(src);
    fseek(src, 0L, SEEK_SET);
    long blockCount = 0;
    if (fileSize > 0) {
        blockCount = ((fileSize - 1) / 254) + 1;
    }

    cout << "File Size: " << fileSize << " bytes."  << endl;
    cout << "Block count: " << blockCount << " blocks" << endl;

    uint8_t nextTrack = entry->firstTrack;
    uint8_t nextSector = entry->firstSector;
    uint8_t track, sector;

    Sector* sect = 0;
    while(blockCount-- > 0) {
        cout << "  ----\nBlock count: " << blockCount << " blocks" << endl;
        if (nextTrack == 0) {
            cout << " Allocating new sector ..." << endl;
            allocateBlock(track, sector);

            sect->nextSector = sector;
            sect->nextTrack = track;
//             writeSector(sect, track, sector);

            sect = (Sector*) calloc(sizeof(Sector), 1);

    // TODO: add track/sector to previous block!!
//             writeSector(sect, track, sector);
            // does not work - there might not even be a previous/current sector!

    // TODO: properly set the FIRST block!
            // if (first) {
            // entry->firstTrack = track;
            // entry->firstSector = sector;
            // first = false;
            // }

            nextTrack = 0;
            nextSector = 0;
        } else {
           cout << " Using existing sector ..." << endl;
           track = nextTrack;
           sector = nextSector;
           sect = readSector(nextTrack, nextSector);
           nextTrack = sect->nextTrack;
           nextSector = sect->nextSector;
        }

        cout << "Sector: " << (int) track << "," << (int) sector << endl;

        // initialize a fresh block (as if it was the last one)
        memset(sect->data, 0, sizeof(Sector));
        size_t readCount = fread(sect->data, 1, 254, src);
        if (blockCount == 0) {
            sect->nextTrack = 0;
            sect->nextSector = readCount + 1;
        }
        writeSector(sect, track, sector);

        cout << "Next Sector: " << (int) nextTrack << "," << (int) nextSector << endl;
    }


    // TODO: deallocate remaining unused blocks!
}


void dir() {
    if (imageFile == 0) {
        cout << "No image mounted." << endl;
        return;
    }

    Bam* bam = (Bam*) readSector(18, 0);

    printf("\n0 \"%16s\" %c%c %c\n", getName(bam->discName), bam->discId[0], bam->discId[1], bam->dosVersion);
    printf("--------------------------\n");

    uint8_t nextTrack = bam->nextTrack;
    uint8_t nextSector = bam->nextSector;
    while(nextTrack != 0 && nextSector != 255) {
        Dir* dir = (Dir*) readSector(nextTrack, nextSector);
        for (int idx = 0;  idx < 8;  idx++) {
            FileEntry* fileEntry = &dir->entry[idx].file;

            if (fileEntry->fileType != 0) {
                string entryName = getName(fileEntry->fileName);
                entryName = "\"" + rtrim(entryName) + "\"";
                printf("%-3d  %-18s %s\n",
                       fileEntry->blockCount,
                       entryName.c_str(),
                       getType(fileEntry->fileType));
            }
        }
        nextTrack = dir->entry[0].nextTrack;
        nextSector = dir->entry[0].nextSector;

        free(dir);
    }
    uint16_t freeBlocks = getFreeBlocks(bam);
    cout << freeBlocks << " Blocks free." << endl << endl;
    free(bam);
}


void dump(const string& name) {

    FileEntry* fileEntry = searchFile(name.c_str());
    if (fileEntry == 0) {
        cout << "File not found." << endl;
        return;
    }

    dumpFile(fileEntry);
}


void dumpSector(uint8_t track, uint8_t sector) {
    Sector* sect = readSector(track, sector);

    HexDump* dump = new_HexDump((uint8_t*) sect, sizeof(Sector));
    HexDump_dumpAll(dump);
    free(dump);
    free(sect);
}



uint8_t nextTrack = 0;
uint8_t nextSector = 0;
uint16_t offset = 0;
Sector* sec = 0;

uint16_t readWord() {

    if (offset > 0xfd) {    // 254 data bytes per sector
        if (sec == 0) {
            free(sec);
        }
        sec = readSector(nextTrack, nextSector);
        nextTrack = sec->nextTrack;
        nextSector = sec->nextSector;
        offset = 0;
    }

    uint16_t result = sec->data[offset] + sec->data[offset+1] * 256;
    offset += 2;
    return result;
}

uint8_t readChar() {

    if (offset > 0xfd) {    // 254 data bytes per sector
        if (sec == 0) {
            free(sec);
        }
        sec = readSector(nextTrack, nextSector);
        nextTrack = sec->nextTrack;
        nextSector = sec->nextSector;
        offset = 0;
    }

    uint8_t result = sec->data[offset];
    offset += 1;
    return result;
}

void dumpBasic(const string& name) {
    FileEntry* fileEntry = searchFile(name.c_str());
    if (fileEntry == 0) {
        cout << "File not found." << endl;
        return;
    }

    nextTrack = fileEntry->firstTrack;
    nextSector = fileEntry->firstSector;
    offset = 0x100;

    uint16_t loadAddr = readWord();
    uint16_t nextAddr = readWord();  // TODO: does not work yet when the word is on a block border!
    uint16_t nextLine = readWord();
    string basLine = "";
    while(nextAddr != 0) {
        uint8_t data = readChar();
        if (data == 0) {
            cout << hex << setw(4) << loadAddr << ": " << dec << nextLine << " " << basLine << endl;
            loadAddr = nextAddr;
            nextAddr = readWord();
            nextLine = readWord();
            basLine = "";
        } else {
            if (data == 0x99) {
                basLine += "PRINT";
            } else if (data == 0x9e) {
                basLine += "SYS";
            } else {
                basLine += data;
            }
        }
    }
}

void createFile(const string& name) {
    FileEntry* fileEntry = searchFile(name.c_str());
    if (fileEntry != 0) {
        cout << "File already exists." << endl;
        return;
    }

    createEntry(name.c_str());
}

int main() {

    mount("C:\\Users\\afester\\Documents\\Andreas\\test.d64");

    string line;
    while(1) {
        cout << prompt;
        std::getline(cin, line);
        if (line != "") {

            vector<string> tokens = split(line, ' ');
    //        vector<string>::const_iterator t = tokens.begin();
    //        while(t != tokens.end()) {
    //            cout << "|  " << *t << endl;
    //            t++;
    //        }

            string command = tokens[0];
            if (command == "exit") {
                return 0;
            }
            else if (command == "mount") {
                if (tokens.size() != 2) {
                    cout << "Usage: mount pathToDiskImage" << endl;
                } else {
                    mount(tokens[1]);
                }
            }
            else if (command == "umount") {
                if (imageFile != 0) {
                    fclose(imageFile);
                    imageFile = 0;
                    prompt = "> ";
                }
            }
            else if (command == "dir") {
                dir();
            }
            else if (command == "dumpSector") {
                if (tokens.size() != 3) {
                    cout << "Usage: dumpSector track sector" << endl;
                } else {
                    dumpSector(atoi(tokens[1].c_str()), atoi(tokens[2].c_str()));
                }
            } else if (command == "dump") {
                if (tokens.size() != 2) {
                    cout << "Usage: dump fileName" << endl;
                } else {
                    dump(tokens[1]);
                }
            }
            else if (command == "dumpBasic") {
                if (tokens.size() != 2) {
                    cout << "Usage: dumpBasic fileName" << endl;
                } else {
                    dumpBasic(tokens[1]);
                }
            }
            else if (command == "create") {
                if (tokens.size() != 2) {
                    cout << "Usage: create filename" << endl;
                } else {
                    createFile(tokens[1]);
                }
            }
            else if (command == "copy") {
                if (tokens.size() != 3) {
                    cout << "Usage: copy src desc" << endl;
                } else {
                    string src = tokens[1];
                    string dest = tokens[2];
                    cout << "Copy " << src << " to " << dest << endl;
                }
            }
            else if (command == "copyin") {
                if (tokens.size() != 3) {
                    cout << "Usage: copyin src desc" << endl;
                } else {
                    string src = tokens[1];
                    string dest = tokens[2];
                    copyIn(src, dest);
                }
            }
            else if (command == "help") {
                cout << "exit" << endl;
                cout << "mount" << endl;
                cout << "umount" << endl;
                cout << "create" << endl;
                cout << "dump fileName" << endl;
                cout << "dumpSector track sector" << endl;
                cout << "dumpBasic fileName" << endl;
            } else {
                cout << "Unknown command: " << command << endl;
            }
        }
    }

    imageFile = fopen("c:\\Users\\afester\\Documents\\Andreas\\Test.d64", "rb");
    Bam* bam = (Bam*) readSector(18, 0);
    dumpBam(bam);


//    Sector* bam = readSector(18, 0);
//    dumpSector(bam);

//    Dir* dir = (Dir*) readSector(bam->nextTrack, bam->nextSector);
//    dumpDir(dir);

//    free(dir);
//    free(bam);

//    listContents();
//    printf("Dump which file?");
//    char input[255];
//    scanf("%s", input);
//    char fileName[20];
//    sprintf(fileName, "%-16s", input);
//    printf("Searching \"%s\"...\n", fileName);
//
//    FileEntry* entry = searchFile(fileName);
//    if (entry == 0) {
//        printf("File not found.\n");
//    } else {
//        dumpFile(entry);
//    }



    fclose(imageFile);

    return 0;
}
