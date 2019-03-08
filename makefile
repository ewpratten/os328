######## CONFIGURATION ########

# Board
avrType=atmega328p # Arduino UNO
# avrType=m32U4 # Arduino pro micro

# CPU clock speed
avrFreq=16000000 # 16 Mhz

# Linux port
programmerDev=/dev/ttyACM0

# Other
baud=115200
programmerDev=/dev/ttyACM0
commsBaud=9600
programmerType=arduino # Arduino UNO
# programmerType=avr109

###### END CONFIGURATION ######

cflags=-DF_CPU=$(avrFreq) -mmcu=$(avrType)  -Os -Wno-incompatible-pointer-types -Wno-sign-compare -Wno-int-conversion 
# -Wall -Werror -Wextra
objects=$(patsubst %.c,%.o,$(wildcard *.c)) io/*.c basic/*.c drivers/*/*.c


.PHONY: flash clean

all: main.hex

%.o: %.c
	avr-gcc $(cflags) -c $< -o $@

main.elf: $(objects)
	avr-gcc $(cflags) -o $@ $^

main.hex: main.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@

flash: main.hex
	stty -F $(programmerDev) speed 1200
	stty -F $(programmerDev) speed $(baud)
	avrdude -p$(avrType) -c$(programmerType) -P$(programmerDev) -b$(baud) -v -U flash:w:$<

clean:
	rm -f main.hex main.elf main.o shell.out
	#$(objects)

connect:
	screen $(programmerDev) $(commsBaud)

shell:
	gcc -Wall -Os -Wno-incompatible-pointer-types -Wno-implicit-function-declaration shell/*.c -o shell.out

finddevices:
	ls /dev/ttyACM*

x86:
	gcc -Os -Wno-incompatible-pointer-types -Wno-sign-compare -D X86 *.c shell/*.c io/*.c -o x86.out