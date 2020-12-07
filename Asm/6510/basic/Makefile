C64DISC=..\..\..\C++\C64disc\build\C64disc.exe
IMAGEFILE=..\..\..\C++\C64disc\build\test.d64

all:
	xa -v -lbasic.lst -r -o basic.prg basic.asm
	cat basic.lst

save:
    $(C64DISC) copyin $(IMAGEFILE) basic.prg SAMPLE.PRG

clean:
	rm *.lst *.prg
                                                      