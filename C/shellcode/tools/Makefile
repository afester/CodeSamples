.PHONY:	attack

all: findst rootshell

rootshell:
	gcc -m32 -Wall -pedantic -o rootshell rootshell.c
	gcc -m32 -Wall -pedantic -S -o rootshell.S rootshell.c

findst:
	gcc -Wall -pedantic -m32 -o findstack findstack.c

getOpcodes:
	nasm -f elf exitsyscall.asm

	# Create a C array with opcodes
	objcopy -O binary exitsyscall.o exitsyscall.bin
	xxd -i exitsyscall.bin > exitsyscall.c

	@echo "objdump output:"
	@echo "---------------------------"
	@objdump -d exitsyscall.o

	@echo
	@echo "xxd output:"
	@echo "---------------------------"
	@cat exitsyscall.c

clean:
	rm findstack rootshell *.o *.S *.bin exitsyscall.c
