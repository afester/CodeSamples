CFLAGS=-pedantic -Wall
#OBJECTS=Win32Profile.o TestProfile.o
OBJECTS=UnixProfile.o TestProfile.o

all: $(OBJECTS)
	g++ -o TestProfile $(OBJECTS)

.cpp.o:
	g++ -c $(CFLAGS) $< -o $@

UnixProfile.o: UnixProfile.h
TestProfile.o: UnixProfile.h

clean:
	rm *.o TestProfile.exe
