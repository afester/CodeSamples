
# Device definitions

# DEVICE=atmega328p
DEVICE=atmega2560
CLOCK=16000000

# Programmer definitions

PGM_DEVICE=$(DEVICE)
# PGM_DEVFILE=usb
PGM_DEVFILE=/dev/ttyACM0

# PGM_PROTO=stk500v2
# PGM_PROTO=arduino
PGM_PROTO=wiring

# Source and target definitions

BUILDDIR=build
SOURCES=lcdisplay.c
TARGET=lcdisplay.hex
INCLUDES=-I. -I../Lib
LIBS=-lm

include ../Lib/Makefile.avr
