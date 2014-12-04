OBJECTS  = Native.o COMProxy.o
TARGET   = Native.dll
TARGET_DIR= ../jlib

# Definitions for the javah-tool
JAVA_HOME = "/c/Program Files/Java/jdk1.8.0_11"
NATIVE_CLASS = com.example.java2com.IDispatch
JH_CLASSPATH = ../bin
JAVAH    =  $(JAVA_HOME)/bin/javah.exe

CXX      = g++
DEFINES  =
CXXFLAGS = -O2 -Wall -frtti -fexceptions -mthreads $(DEFINES)
INCPATH  = -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/win32

LINK     = dllwrap
#LFLAGS   = -enable-stdcall-fixup -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc -Wl,-s -mthreads -Wl -shared -Wl,--out-implib
#LFLAGS   = -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc -Wl,-s -Wl -shared
LFLAGS   = --driver-name g++ --def Native.def
LIBS     = -lole32 -loleaut32 -luuid

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

$(TARGET):  $(OBJECTS)
	$(LINK) $(LFLAGS) -o $(TARGET_DIR)/$(TARGET) $(OBJECTS) $(LIBS)

stub:
	$(JAVAH) -classpath $(JH_CLASSPATH) $(NATIVE_CLASS)

clean:
	rm $(OBJECTS) $(TARGET_DIR)/$(TARGET)

versions:
	$(CXX) --version
	$(JAVAH) -version
	$(LINK) -version
