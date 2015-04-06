.PHONY:	spawnshell attack

all: spawnshell attack

attack:
	gcc -Wall -pedantic -m32 -o attack attack.c

spawnshell:
	nasm -f elf spawnshell.asm
	ld -m elf_i386 -o spawnshell spawnshell.o

	# Create a C array with opcodes
	objcopy -O binary spawnshell.o spawnshell.bin
	xxd -i spawnshell.bin > spawnshell.c

	@echo "objdump output:"
	@echo "---------------------------"
	@objdump -d spawnshell.o

	@echo
	@echo "xxd output:"
	@echo "---------------------------"
	@cat spawnshell.c

clean:
	-rm attack spawnshell *.o *.S *.bin
