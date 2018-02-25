
# Device definitions
DEVICE=atmega1284p
CLOCK=2000000

# Programmer definitions
PGM_DEVICE=$(DEVICE)

PGM_PROTO=stk500v1
PGM_DEVFILE=/dev/ttyACM0

# Fuse definitions for this project
#
#  | Low                             | High                            | Extended
# -+---------------------------------+---------------------------------+-----------------------------------------------
# 7| [ ] CKDIV8 Divide clock by 8    | [ ] OCDEN   Enable OCD          |
# 6| [ ] CKOUT  Clock output         | [ ] JTAGEN  Enable JTAG         |
# 5| [X] SUT1   Select start-up time | [X] SPIEN   Enable Serial programming and Data Downloading
# 4| [X] SUT0   Select start-up time | [ ] WDTON   Watchdog timer always on
# 3| [X] CKSEL3 Select Clock Source  | [ ] EESAVE  EEPROM memory is preserved through chip erase
# 2| [ ] CKSEL2 Select Clock Source  | [X] BOOTSZ1 Select Boot Size    | [ ] BODLEVEL2 Brown-out Detector trigger level
# 1| [ ] CKSEL1 Select Clock Source  | [X] BOOTSZ0 Select Boot Size    | [ ] BODLEVEL1 Brown-out Detector trigger level
# 0| [ ] CKSEL0 Select Clock Source  | [ ] BOOTRST Select Reset Vector | [ ] BODLEVEL0 Brown-out Detector trigger level
# NOTE: programmed fuse is 0!

LFUSE=0xc7
HFUSE=0xd9
EFUSE=0xff

# Source and target definitions
BUILDDIR=build
# SOURCES=ILI9481.c main.c glcdfont.c 7segFont.c encoder.c mcp4811.c adc.c
SOURCES=cfa533.c twi.c main.c glcdfont.c 7segFont.c encoder.c mcp4811.c adc.c
TARGET=tftsample.hex
INCLUDES=-I. -I../Lib

include ../Lib/Makefile.avr
