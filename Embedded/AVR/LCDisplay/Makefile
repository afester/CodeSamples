# Device definitions

# DEVICE=atmega328p
# DEVICE=atmega2560
DEVICE=atmega1284p
CLOCK=20000000

# Programmer definitions

PGM_DEVICE=$(DEVICE)

# PGM_DEVFILE=usb
# PGM_PROTO=stk500v2
# PGM_PROTO=arduino

# PGM_DEVFILE=/dev/ttyACM0
# PGM_PROTO=wiring

PGM_PROTO=stk500v1
PGM_DEVFILE=/dev/ttyACM0

# Source and target definitions

BUILDDIR=build
SOURCES=twi.c lcdisplay.c adc.c
TARGET=lcdisplay.hex
INCLUDES=-I. -I../Lib
LIBS=-lm

include ../Lib/Makefile.avr
