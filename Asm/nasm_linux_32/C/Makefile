TARGET=hello
TARGETASM=asmproc

all: debug

debug:
	# gcc -Wall -pedantic -m32 -g -c $(TARGET).c
	nasm -f elf -g -Fdwarf $(TARGETASM).asm -l $(TARGETASM).lst
	gcc -m32 -o $(TARGET) $(TARGETASM).o

release:
	gcc -Wall -pedantic -m32 -O3 -c $(TARGET).c
	nasm -f elf $(TARGETASM).asm -l $(TARGETASM).lst
	gcc -m32 -o $(TARGET) $(TARGET).o $(TARGETASM).o
	strip $(TARGET)

clean:
	rm *~ *.o *.lst $(TARGET)

