VBOX_IMAGE=$(HOME)/VirtualBox\ VMs/Plain/Plain.vdi

all:

minimal:
	gcc -Wall -pedantic -o createboot createboot.c
	nasm -f bin -o source.bin minimal.asm -l minimal.lst
	./createboot $(VBOX_IMAGE)

image:
	nasm -f aout -o minimal.o minimal.asm -l minimal.lst
	gcc -o main.o main.c
