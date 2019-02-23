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

    // test dio (pin 5 light)
    puts("Testing dio");
    gpio_setOutput(DIGITAL_BANKA, DIGITALA(5));
    gpio_up(DIGITALA(), DIGITALA(5));
    puts("Set pin 5 HIGH");


    puts("Enabling echo for debug");
    for(;;){
        char input = getchar();
        printf("%c", input);
        gpio_down(DIGITALA(), DIGITALA(5));
        puts("Set pin 5 to LOW");
    }

    return 0;
}