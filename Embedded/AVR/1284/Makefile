DEVICE=atmega1284p
CLOCK=8000000

PGM_PROTO=stk500v1
PGM_DEVFILE=/dev/ttyACM0

SOURCES=main.c
TARGET=main.hex
BUILDDIR=build

#------------------------
CC=avr-gcc
AS=avr-as

OBJECTS=$(addprefix $(BUILDDIR)/,$(notdir $(SOURCES:.c=.o)))
ELFFILE=$(addprefix $(BUILDDIR)/,$(TARGET:.hex=.elf))
HEXFILE=$(addprefix $(BUILDDIR)/,$(TARGET))
ASMFILES=$(SOURCES:.c=.S)
CPPFILES=$(SOURCES:.c=.C)
DEFFILES=$(SOURCES:.c=.DEF)
ASMLISTFILES=$(SOURCES:.c=.lst)

all: preBuild $(HEXFILE)

preBuild:
	@if [ ! -d $(BUILDDIR) ] ; then	\
	   mkdir -p $(BUILDDIR) ;	\
	fi

$(BUILDDIR)/%.o: %.c
	$(CC) -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) $(INCLUDES) -ffunction-sections -fdata-sections -MMD -MP -c $< -o $@

$(ELFFILE): $(OBJECTS)
	$(CC) -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) $(INCLUDES) -ffunction-sections -fdata-sections -o $(ELFFILE) $(OBJECTS) $(LIBS) -Wl,--gc-sections

$(HEXFILE): $(ELFFILE)
	rm -f $(HEXFILE)
	avr-objcopy -j .text -j .data -O ihex $(ELFFILE) $(HEXFILE)
	avr-size --format=berkeley $(ELFFILE)

clean:
	rm -rf build

upload: all
	avrdude -vvv -p $(DEVICE) -c $(PGM_PROTO) -P $(PGM_DEVFILE) -b9600 -Uflash:w:$(HEXFILE):i

setFuses:
	avrdude -vvv -p $(DEVICE) -c $(PGM_PROTO) -P $(PGM_DEVFILE) -b9600 -U lfuse:w:0xe2:m -U hfuse:w:0x99:m -U efuse:w:0xff:m

readFuses:
	avrdude -vvv -p $(DEVICE) -c $(PGM_PROTO) -P $(PGM_DEVFILE) -b9600 -U lfuse:r:-:i -U hfuse:r:-:i -U efuse:r:-:i

readFlash:
	avrdude -vvv -p $(DEVICE) -c $(PGM_PROTO) -P $(PGM_DEVFILE) -b9600 -Uflash:r:result.hex:i 

precomp: $(CPPFILES)

%.C: %.c
#	avr-gcc -dM -E -Wall -Os -DF_CPU=${CLOCK} -mmcu=${DEVICE} -I. -ffunction-sections -fdata-sections -MMD -MP -c main.c -o main.C
	$(CC)       -E -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) $(INCLUDES) -ffunction-sections -fdata-sections -MMD -MP -c $< -o $@

defines: $(DEFFILES)

%.DEF: %.c
	$(CC) -dM -E -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) $(INCLUDES) -ffunction-sections -fdata-sections -MMD -MP -c $< -o $@

assembly: $(ASMFILES)

%.S: %.c
	$(CC) -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) $(INCLUDES) -ffunction-sections -fdata-sections -MMD -MP -S $< -o $@

%.lst: %.S
	$(AS) -mmcu=$(DEVICE) $< -a=$@

asmlist: $(ASMLISTFILES)

disasm:
	avr-objdump -d $(ELFFILE)

help:
	@echo Sources : $(SOURCES)
	@echo Includes: $(INCLUDES)
	@echo Objects : $(OBJECTS)
	@echo Elf-file: $(ELFFILE)
	@echo Hex-File: $(HEXFILE)
	@echo
	@echo Targets:
	@echo -----------------------------------------------------------------------------
	@echo "all      : Default target - creates the target hex file."
	@echo "precomp  : Creates a pre-processed file for each source file."
	@echo "defines  : Creates a file containing all #define's for each source file."
	@echo "assembly : Creates the corresponding assembly file for each source file."
	@echo "asmlist  : Creates an assembler listing file for each source file."
	@echo "disasm   : Creates a disassembly of the target's elf file."
	@echo "upload   : Uploads the target's hex file to the Arduino board"
	@echo "readFuses: Downloads and shows the current fuse settings."
	@echo "readFlash: Downloads the current flash (code) memory into result.hex."

