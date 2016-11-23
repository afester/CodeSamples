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

upload:
	avrdude -p m2560 -c stk500v2 -P usb -v -B 10 -F -U flash:w:main.hex:i

readFuses:
	avrdude -p m2560 -c stk500v2 -P usb -v -F -U lfuse:r:-:i -U hfuse:r:-:i -U efuse:r:-:i


precomp:
	avr-gcc -E -Wall -Os -DF_CPU=${CLOCK} -mmcu=${DEVICE} -I. -ffunction-sections -fdata-sections -MMD -MP -c main.c -o main.C

defines:
	avr-gcc -dM -E -Wall -Os -DF_CPU=${CLOCK} -mmcu=${DEVICE} -I. -ffunction-sections -fdata-sections -MMD -MP -c main.c -o main.C

assembly:
	avr-gcc -Wall -Os -DF_CPU=${CLOCK} -mmcu=${DEVICE} -I. -ffunction-sections -fdata-sections -MMD -MP -S main.c -o main.S
