TARGET=helloworld
all: debug

debug:
	@ml /c $(TARGET).asm
	@link /subsystem:console /nodefaultlib /entry:_main $(TARGET).obj kernel32.lib

release:

clean:
	del *.obj $(TARGET) $(TARGET).exe
	