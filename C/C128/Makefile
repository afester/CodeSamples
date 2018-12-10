
CC=c:\program files (x86)\cl65\bin\cl65
DA=c:\program files (x86)\cl65\bin\da65

all:
    "$(CC)" -t c64 -l main.lst -o main.prg main.c

disas:
    "$(DA)" main

clean:
    del main.s main.prg main.o main.lst
