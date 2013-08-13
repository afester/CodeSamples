all:
	gcc -Wall -pedantic -o createboot createboot.c
	nasm -f bin -o source.bin minimal.asm -l minimal.lst
	./createboot /home/andreas/VirtualBox\ VMs/Plain\ CPU/FreeDos.vdi
