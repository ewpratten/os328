baud=115200
avrType=atmega328p
# avrType=atmega32u4
avrFreq=16000000 # 16 Mhz
programmerDev=/dev/ttyACM0
commsBaud=9600
programmerType=arduino

cflags=-DF_CPU=$(avrFreq) -mmcu=$(avrType) -Wall -Werror -Wextra -Os -Wno-incompatible-pointer-types -Wno-sign-compare
objects=$(patsubst %.c,%.o,$(wildcard *.c)) io/*.c shell/*.c


.PHONY: flash clean

all: main.hex

%.o: %.c
	avr-gcc $(cflags) -c $< -o $@

main.elf: $(objects)
	avr-gcc $(cflags) -o $@ $^

main.hex: main.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@

flash: main.hex
	avrdude -p$(avrType) -c$(programmerType) -P$(programmerDev) -b$(baud) -v -U flash:w:$<

clean:
	rm -f main.hex main.elf main.o shell.out
	#$(objects)

connect:
	screen $(programmerDev) $(commsBaud)

shell:
	gcc -Wall -Os -Wno-incompatible-pointer-types -Wno-implicit-function-declaration shell/*.c -o shell.out