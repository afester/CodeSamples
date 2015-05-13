all:
	gcc -S -o add.S add.c
	gcc -c -o add.o add.c

	gcc -S -o main.S main.c
	gcc -c -o main.o main.c

	# ld -e 0 -o main main.o add.o
	ld -Ttext 0 -e 0 -o main main.o add.o
	cp main main_stripped
	strip main_stripped

	objcopy -O binary main_stripped main_raw

clean:
	rm main main_stripped main_raw *.o *.S
