CFLAGS=-pedantic -Wall
OBJECTS=Profile.o

all: $(OBJECTS)
	g++ -o Profile $(OBJECTS)

.cpp.o:
	g++ -c $(CFLAGS) $< -o $@

clean:
	rm *.o Profile.exe

asm:
	g++ -S $(CFLAGS) Profile.cpp
