
# Device definitions
DEVICE=atmega2560
CLOCK=16000000

# Programmer definitions
PGM_DEVICE=$(DEVICE)
PGM_DEVFILE=/dev/ttyACM0
PGM_PROTO=wiring

# Source and target definitions
BUILDDIR=build
SOURCES=mcp4811.c
TARGET=mcp4811.hex
INCLUDES=-I. -I../Lib

include ../Lib/Makefile.avr
