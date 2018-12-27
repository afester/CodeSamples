
C64DISC=..\..\..\C++\C64disc\build\C64disc.exe
IMAGEFILE=..\..\..\C++\C64disc\build\test.d64
CC=c:\program files (x86)\cl65\bin\cl65
DA=c:\program files (x86)\cl65\bin\da65

all:
    "$(CC)" -t c64 -l main.lst -o main.prg main.c
    $(C64DISC) copyin $(IMAGEFILE) main.prg SAMPLE.PRG

disas:
    "$(DA)" main

clean:
    del main.s main.prg main.o main.lst
