
# Device definitions
DEVICE=atmega2560
CLOCK=16000000

# Programmer definitions
PGM_DEVICE=$(DEVICE)
PGM_DEVFILE=/dev/ttyACM0
PGM_PROTO=wiring

# Source and target definitions
BUILDDIR=build
SOURCES=ILI9481.c main.c glcdfont.c 7segFont.c
TARGET=tftsample.hex
INCLUDES=-I. -I../Lib

include ../Lib/Makefile.avr
