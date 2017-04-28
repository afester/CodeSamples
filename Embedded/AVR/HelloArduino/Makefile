# DEVICE=atmega328p
DEVICE=atmega2560
CLOCK=16000000

all:
	avr-gcc -Wall -Os -DF_CPU=${CLOCK} -mmcu=${DEVICE} -I. -ffunction-sections -fdata-sections -MMD -MP -c main.c -o main.o
	avr-gcc -Wall -Os -DF_CPU=${CLOCK} -mmcu=${DEVICE} -I. -ffunction-sections -fdata-sections -o main.elf main.o -lm -Wl,--gc-sections
	avr-size --format=berkeley main.elf
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex

clean:
	rm -f main.hex main.o main.elf main.d

precomp:
	avr-gcc -dM -E -Wall -Os -DF_CPU=${CLOCK} -mmcu=${DEVICE} -I. -ffunction-sections -fdata-sections -MMD -MP -c main.c -o main.C

assembly:
	avr-gcc -Wall -Os -DF_CPU=${CLOCK} -mmcu=${DEVICE} -I. -ffunction-sections -fdata-sections -MMD -MP -S main.c -o main.S
