#include "include/io/serial.h"
#include "include/io/gpio.h"

int main(){
    // Start serial connection and redirect stdin/out
    uart_init();
    stdout = &uart_output;
    stdin  = &uart_input;

    puts("AVR-X");
    puts("By: Evan Pratten <ewpratten@gmail.com>\n");
    puts("Starting");

    /* Run boot-time tests of components */

    // test dio (pin 13 light)
    printf("Testing gpio.. ");
    gpio_test();
    puts("DONE");


    puts("Enabling echo for debug");
    for(;;){
        char input = getchar();
        printf("%c", input);
    }

    return 0;
}