# DEVICE=atmega328p
DEVICE=atmega2560
CLOCK=16000000

# PROGRAMMER=usb
PROGRAMMER=/dev/ttyACM0

# PROTOCOL=stk500v2
# PROTOCOL=arduino
PROTOCOL=wiring

all:
	avr-gcc -Wall -Os -DF_CPU=${CLOCK} -mmcu=${DEVICE} -I. -ffunction-sections -fdata-sections -MMD -MP -c main.c -o main.o
	avr-gcc -Wall -Os -DF_CPU=${CLOCK} -mmcu=${DEVICE} -I. -ffunction-sections -fdata-sections -o main.elf main.o -lm -Wl,--gc-sections
	avr-size --format=berkeley main.elf
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex

clean:
	rm -f main.hex main.o main.elf main.d

upload:
# See https://github.com/sudar/Arduino-Makefile/issues/114 - upload requires the -D option!
	avrdude -p atmega2560 -c ${PROTOCOL} -P ${PROGRAMMER} -v -D -b 115200 -F -U flash:w:main.hex:i

readFuses:
	avrdude -p m2560 -c ${PROTOCOL} -P ${PROGRAMMER} -v -F -U lfuse:r:-:i -U hfuse:r:-:i -U efuse:r:-:i

readFlash:
	avrdude -p m2560 -c ${PROTOCOL} -P ${PROGRAMMER} -v -F -U flash:r:result.hex:i


precomp:
	avr-gcc -E -Wall -Os -DF_CPU=${CLOCK} -mmcu=${DEVICE} -I. -ffunction-sections -fdata-sections -MMD -MP -c main.c -o main.C

defines:
	avr-gcc -dM -E -Wall -Os -DF_CPU=${CLOCK} -mmcu=${DEVICE} -I. -ffunction-sections -fdata-sections -MMD -MP -c main.c -o main.C

assembly:
	avr-gcc -Wall -Os -DF_CPU=${CLOCK} -mmcu=${DEVICE} -I. -ffunction-sections -fdata-sections -MMD -MP -S main.c -o main.S

disasm:
	avr-objdump -d main.elf
