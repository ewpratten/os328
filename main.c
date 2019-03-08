#include "include/io/serial.h"
#include "include/io/gpio.h"

#include "include/basic/ubasic.h"

#include "include/basic/utils.h"
#include "include/bootfiles.h"

#include "include/drivers/sdcard/sdcard.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    // Start serial connection and redirect stdin/out
    #ifndef X86
        uart_init();
        stdout = &uart_output;
        stdin  = &uart_input;
        uart_clearscreen();
    #endif

    /* Header text */
    puts("os328");
    puts("By: Evan Pratten <ewpratten@gmail.com>\n");
    puts("Starting");

    /* Init and read from SDCARD */
    sd_sdCardBegin(4);

    /* Run init.sys script before boot-time tests, but after component initalization*/
    ubasic_init(bootscript_init);
    ubasic_run();
    while (!ubasic_finished())
    {
        ubasic_run();
    }

    /* Run boot-time tests of components */

    // test dio (pin 13 light)
    printf("Testing gpio.. ");
    gpio_test();
    puts("DONE");

    /* Boot process finished. run main script */
    ubasic_init(bootscript_start);
    ubasic_run();
    while (!ubasic_finished())
    {
        ubasic_run();
    }

    // // Make space in memory for the terminal input buffers
    // printf("Allocating space for input buffers.. ");
    // char program[1000];
    // char buffer[80];
    // char *b = buffer;
    // size_t bufferSize = 80;
    // size_t lineSize;
    // puts("DONE");

    // /* BASIC interpreter loop */
    // puts("\nREADY"); // Just like the c64

    // for(;;){
    //     // Get a line from serial
    //     lineSize = getline(&b, &bufferSize, stdin);
    //     // puts(&buffer[strlen(buffer) - lineSize]);
    //     // char line[50];
        
    //     // Append line to program buffer
    //     strcat(program, &buffer[strlen(buffer) - lineSize]);
        
    //     // Check for special commands
    //     if(strncmp(&program[strlen(program) - lineSize], "run\n", 3)==0){
    //         puts(program);
    //         shell_utils_cutstr(program, 14);
    //         int len = strlen(program);
    //         program[len-4] = 0;
    //         puts(program);

    //         ubasic_init(program);

    //         do {
    //             ubasic_run();
    //         } while(!ubasic_finished());
    //     }
    // }

    return 0;
}