
# Device definitions
DEVICE=atmega2560
CLOCK=16000000

# Programmer definitions
PGM_DEVICE=$(DEVICE)
PGM_DEVFILE=/dev/ttyACM0
PGM_PROTO=wiring

# Source and target definitions
BUILDDIR=build
SOURCES=encoder.c lcdisplay.c twi.c mcp4811.c

TARGET=encoder.hex
INCLUDES=-I. -I../Lib

include ../Lib/Makefile.avr
