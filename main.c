#include "include/io/serial.h"

int main(){
    // Start serial connection and redirect stdin/out
    uart_init();
    stdout = &uart_output;
    stdin  = &uart_input;

    puts("AVR-X");
    puts("By: Evan Pratten <ewpratten@gmail.com>\n");
    puts("Starting");

    return 0;
}