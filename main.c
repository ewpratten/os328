#include "include/io/serial.h"

int main(){
    uart_init();
    stdout = &uart_output;
    stdin  = &uart_input;

    puts("Hello world!");

    return 0;
}