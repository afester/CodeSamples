CFLAGS=-pedantic -Wall -std=c99 -O3
OBJECTS=benchmark.o

all: $(OBJECTS)
	gcc -o benchmark $(OBJECTS)

benchmark.o: ../../Tools/Profile/Win32Profile.h

clean:
	rm *.o benchmark
