TARGET=minimal

all: debug

debug:
	as -a=$(TARGET).lst -o$(TARGET).o $(TARGET).asm
	# as --32 -a=$(TARGET).lst -o$(TARGET).o $(TARGET).asm
	ld -m elf_i386 -o $(TARGET) $(TARGET).o

release:
	nasm -f elf $(TARGET).asm -l $(TARGET).lst
	nasm -f elf tests.asm -l tests.lst
	ld -m elf_i386 -o tests $(TARGET).o tests.o
	strip tests

clean:
	rm *~ *.o *.lst $(TARGET) tests
