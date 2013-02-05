all:
	g++ -Wall -pedantic -c FileSystem.cpp Directory.cpp StaticInitOrder.cpp
	g++ -o initorder FileSystem.o Directory.o StaticInitOrder.o
	
clean:
	rm *.o *.exe