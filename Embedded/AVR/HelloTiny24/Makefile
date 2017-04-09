DEVICE=attiny24
CLOCK=8000000
PGM_DEVICE=attiny24
PGM_PROTO=stk500v1
PGM_DEVFILE=/dev/ttyACM0
HEXFILE=main.hex

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

download:
	avrdude -vvv -p $(PGM_DEVICE) -c $(PGM_PROTO) -P $(PGM_DEVFILE) -b9600 -Uflash:r:download.hex:i 

upload:
	avrdude -vvv -p $(PGM_DEVICE) -c $(PGM_PROTO) -P $(PGM_DEVFILE) -b9600 -Uflash:w:$(HEXFILE):i
