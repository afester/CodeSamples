
# Device definitions
DEVICE=atmega1284p
CLOCK=2000000

# Programmer definitions
PGM_DEVICE=$(DEVICE)

PGM_PROTO=stk500v1
PGM_DEVFILE=/dev/ttyACM0

# Fuse definitions for this project
#
#  | Low                             | High                             | Extended
# -+---------------------------------+----------------------------------+-----------------------------------------------
# 7| [ ]1 CKDIV8 Divide clock by 8   | [ ]1 OCDEN   Enable OCD          |
# 6| [ ]1 CKOUT  Clock output        | [ ]1 JTAGEN  Enable JTAG         |
# 5| [X]0 SUT1   Select startup time | [X]0 SPIEN   Enable Serial programming and Data Downloading
# 4| [X]0 SUT0   Select startup time | [ ]1 WDTON   Watchdog timer always on
# 3| [X]0 CKSEL3 Select Clock Source | [ ]1 EESAVE  EEPROM memory is preserved through chip erase
# 2| [ ]1 CKSEL2 Select Clock Source | [X]0 BOOTSZ1 Select Boot Size    | [ ]1 BODLEVEL2 Brown-out Detector trigger level
# 1| [ ]1 CKSEL1 Select Clock Source | [X]0 BOOTSZ0 Select Boot Size    | [ ]1 BODLEVEL1 Brown-out Detector trigger level
# 0| [ ]1 CKSEL0 Select Clock Source | [ ]1 BOOTRST Select Reset Vector | [ ]1 BODLEVEL0 Brown-out Detector trigger level
#   ====================================================================================================================
#     0xC7                              0xD9                               0xFF
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
