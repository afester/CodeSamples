TARGET=Minimal

all:
	as -a=$(TARGET).lst -o$(TARGET).o $(TARGET).asm
	ld -o $(TARGET) $(TARGET).o

clean:
	rm *~ *.o *.lst $(TARGET)
