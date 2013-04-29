TARGET=toollib

all: debug

debug:
	nasm -f elf -g -Fdwarf $(TARGET).asm -l $(TARGET).lst
	nasm -f elf -g -Fdwarf tests.asm -l tests.lst
	ld -m elf_i386 -o tests $(TARGET).o tests.o

release:
	nasm -f elf $(TARGET).asm -l $(TARGET).lst
	nasm -f elf tests.asm -l tests.lst
	ld -m elf_i386 -o tests $(TARGET).o tests.o
	strip tests

clean:
	rm *~ *.o *.lst $(TARGET) tests
