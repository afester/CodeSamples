
# Device definitions
DEVICE=atmega1284p
CLOCK=2000000

# Programmer definitions
PGM_DEVICE=$(DEVICE)

PGM_PROTO=stk500v1
PGM_DEVFILE=/dev/ttyACM0

# Source and target definitions
BUILDDIR=build
SOURCES=ILI9481.c main.c glcdfont.c 7segFont.c encoder.c mcp4811.c
TARGET=tftsample.hex
INCLUDES=-I. -I../Lib

include ../Lib/Makefile.avr
