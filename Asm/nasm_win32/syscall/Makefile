TARGET=helloworld
all: debug

debug:
	nasm -fwin32 $(TARGET).asm
	link /subsystem:console /nodefaultlib /entry:main $(TARGET).obj kernel32.lib

release:

clean:
	del *.obj $(TARGET) $(TARGET).exe
	