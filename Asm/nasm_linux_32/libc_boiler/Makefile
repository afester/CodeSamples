TARGET=hello

all: debug

debug:
	nasm -f elf -g -Fdwarf $(TARGET).asm -l $(TARGET).lst
	gcc -m32 -o $(TARGET) $(TARGET).o

release:
	nasm -f elf $(TARGET).asm -l $(TARGET).lst
	gcc -m32 -o $(TARGET) $(TARGET).o
	strip $(TARGET)

clean:
	rm *~ *.o *.lst $(TARGET)

