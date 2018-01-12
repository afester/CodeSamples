TARGET=toollib
all: debug

debug:
	@ml /c /Zd /Fl$(TARGET).lst $(TARGET).asm
	@ml /c /Zd /Fltests.lst tests.asm
	@link /debug /subsystem:console /nodefaultlib /entry:main tests.obj $(TARGET).obj kernel32.lib

release:
	@ml /c /Fl$(TARGET).lst $(TARGET).asm
	@link /subsystem:console /nodefaultlib /entry:_main $(TARGET).obj kernel32.lib

clean:
	del *.obj $(TARGET) tests.exe tests.pdb tests.ilk *.lst $(TARGET).pdb
