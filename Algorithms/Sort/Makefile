CFLAGS=-pedantic -Wall
OBJECTS=SortAlgorithm.o Braindeadsort.o Simplesort.o Quicksort.o

all: $(OBJECTS)
	g++ -o SortAlgorithms $(OBJECTS)

.cpp.o:
	g++ $(CFLAGS) -c $< -o $@
	
clean:
	rm *.o SortAlgorithms.exe
