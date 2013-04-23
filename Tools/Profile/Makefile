CFLAGS=-pedantic -Wall
OBJECTS=Win32Profile.o TestProfile.o

all: $(OBJECTS)
	g++ -o TestProfile $(OBJECTS)

.cpp.o:
	g++ -c $(CFLAGS) $< -o $@

clean:
	rm *.o TestProfile.exe
