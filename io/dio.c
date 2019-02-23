#include <avr/io.h>

void dio_up(int port){
    DDRB |= (1 << port);
}

void dio_down(int port){
    DDRB &= (0 << port);
}