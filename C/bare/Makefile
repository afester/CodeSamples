all:
	@echo "Compiling ..."
	gcc -fno-asynchronous-unwind-tables  -fno-unwind-tables -fno-exceptions -m32 -S -o add.S add.c
	gcc -fno-asynchronous-unwind-tables -fno-unwind-tables -fno-exceptions -m32 -c -o add.o add.c

	gcc -fno-asynchronous-unwind-tables -fno-unwind-tables -fno-exceptions -m32 -S -o main.S main.c
	gcc -fno-asynchronous-unwind-tables -fno-unwind-tables -fno-exceptions -m32 -c -o main.o main.c

	@echo
	@echo "Linking ..."
	ld -cref -melf_i386 -T ldscript.txt -e 0 -o main main.o add.o | sed 's/^/  /'

	@echo
	@echo "Section sizes:"
	size -A main | sed 's/^/  /'

	@echo
	@echo "Creating binary image ..."
	cp main main_stripped
	strip main_stripped

	objcopy -O binary main_stripped main_raw

	# objdump -D -b binary -mi386 -Maddr16,data16 main_raw
	objdump -D -b binary -mi386 main_raw

clean:
	rm main main_stripped main_raw *.o *.S
