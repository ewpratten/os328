# avr-x
A kernel / os for the atmega328p

## Build requirements
avr-x must be built on a linux machine with the following packages:
```
gcc-avr
avr-libc
avrdude
```

## Building
Just run the make command:
```
make
```

## Deploying
Connect your atmega device over usb and set the interface port in the `makefile`
```
baud=115200
avrType=atmega328p
avrFreq=16000000 # 16 Mhz
programmerDev=/dev/<interface>
```

Next, run the following make command:
```
make flash
```