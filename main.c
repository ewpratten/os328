#include "include/io/serial.h"
#include "include/io/gpio.h"

#include "include/shell/ubasic.h"

#include "include/shell/utils.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    // Start serial connection and redirect stdin/out
    uart_init();
    stdout = &uart_output;
    stdin  = &uart_input;

    /* Header text */
    puts("AVR-X");
    puts("By: Evan Pratten <ewpratten@gmail.com>\n");
    puts("Starting");

    /* Run boot-time tests of components */

    // test dio (pin 13 light)
    printf("Testing gpio.. ");
    gpio_test();
    puts("DONE");

    // Make space in memory for the terminal input buffers
    printf("Allocating space for input buffers.. ");
    char program[1000];
    char buffer[80];
    char *b = buffer;
    size_t bufferSize = 80;
    size_t lineSize;
    puts("DONE");
    
    /* BASIC interpreter loop */
    for(;;){
        // Get a line from serial
        lineSize = getline(&b, &bufferSize, stdin);
        // char line[50];
        
        // Append line to program buffer
        strcat(program, &buffer[strlen(buffer) - lineSize]);
        
        // Check for special commands
        if(strncmp(&program[strlen(program) - lineSize], "run\n", 3)==0){
            shell_utils_cutstr(program, 6);
            int len = strlen(program);
            program[len-4] = 0;

            ubasic_init(program);

            do {
                ubasic_run();
            } while(!ubasic_finished());
        } else if(strncmp(&program[strlen(program) - lineSize], "shutdown\n", 8)==0){
            return 0;
        }
    }

    return 0;
}