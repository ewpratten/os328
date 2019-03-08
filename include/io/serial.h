#ifndef _IO_SERIAL_H
#define _IO_SERIAL_H

#include <stdio.h>

void uart_init(void);

void uart_putchar(char c, FILE *stream);
void uart_getchar(FILE *stream __attribute__((unused)));

void uart_clearscreen();

// alternate streams to use for serial io
FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE uart_input  = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

#endif