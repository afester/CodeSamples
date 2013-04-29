PROJECT=helloworld

all: debug

debug:
	nasm -f elf64 -g -Fdwarf $(PROJECT).asm -l $(PROJECT).lst
	ld -m elf_x86_64 -o $(PROJECT) $(PROJECT).o

release:
	nasm -f elf64 $(PROJECT).asm -l $(PROJECT).lst
	ld -m elf_x86_64 -o $(PROJECT) $(PROJECT).o
	strip $(PROJECT)

clean:
	rm *.o *.lst $(PROJECT)

