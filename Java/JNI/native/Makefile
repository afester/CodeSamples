OBJECTS  = Native.o
TARGET   = Native.dll
TARGET_DIR= ../jlib

# Definitions for the javah-tool
JAVA_HOME = /c/Program\ Files/Java/jdk1.7.0_21
NATIVE_CLASS = com.example.Native
JH_CLASSPATH = ../bin
JAVAH    =  $(JAVA_HOME)/bin/javah.exe

CXX      = g++
DEFINES  =
CXXFLAGS = -O2 -pedantic -Wall -frtti -fexceptions -mthreads $(DEFINES)
INCPATH  = -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/win32

# This was necessary for some MINGW 32 bit build (using gcc 4.6.2)
# LINK     = dllwrap
#LFLAGS   = --driver-name g++ --def Native.def

# With MingW-64 (gcc 4.8.0), it seems that neither dllwrap nor a .def file is needed 
LINK   = g++
LFLAGS = -shared

LIBS =

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

$(TARGET): stub $(OBJECTS)
	$(LINK) $(LFLAGS) -o $(TARGET_DIR)/$(TARGET) $(OBJECTS) $(LIBS)

stub:
	$(JAVAH) -classpath $(JH_CLASSPATH) $(NATIVE_CLASS)

clean:
	rm  com_example_Native.h $(OBJECTS) $(TARGET_DIR)/$(TARGET)
