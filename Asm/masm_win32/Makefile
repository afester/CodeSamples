TARGET=helloworld
all: debug

debug:
	@ml /c /Zd /Fl$(TARGET).lst $(TARGET).asm
	@link /debug /subsystem:console /nodefaultlib /entry:_main $(TARGET).obj kernel32.lib

release:
	@ml /c /Fl$(TARGET).lst $(TARGET).asm
	@link /subsystem:console /nodefaultlib /entry:_main $(TARGET).obj kernel32.lib

clean:
	del *.obj $(TARGET) $(TARGET).exe $(TARGET).ilk $(TARGET).lst $(TARGET).pdb
