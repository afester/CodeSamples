PROJECT=cpuid

all: debug

debug:
	nasm -f elf -g -Fdwarf $(PROJECT).asm -l $(PROJECT).lst
	ld -m elf_i386 -o $(PROJECT) $(PROJECT).o ../toollib/toollib.o

release:
	nasm -f elf $(PROJECT).asm -l $(PROJECT).lst
	ld -m elf_i386 -o $(PROJECT) $(PROJECT).o ../toollib/toollib.o
	strip $(PROJECT)

clean:
	rm *~ *.o *.lst $(PROJECT)

