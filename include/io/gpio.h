#ifndef _IO_GPIO_H
#define _IO_GPIO_H

#include <avr/io.h>

// #define ANALOG   PORTC
// #define DIGITALA PORTD
// #define DIGITALB PortB

#define ANALOG(x) PORTC##x
#define DIGITALA(x) PORTD##x
#define DIGITALB(x) PORTB##x 

#define DIGITAL_BANKA DDRD
#define DIGITAL_BANKB DDRB
#define ANALOG_BANK   DDRC

/*
    Writing to gpio:
        On an arduino UNO, the ports are as follows:

        Analog: ANALOG()
        Digital 0-7: DIGITALA()
        Digital 8-13: DIGITALB()

        The port number argument is constructed in the following way:

        <type><section>(<number>)

        So, digital 5 = DIGITALA(5)

        This means that setting digital 5 to input mode would be:

        gpio_setInput(DIGITALA(), DIGITALA(5))
*/
#define gpio_up( port_letter, number )   port_letter |= (1<<number)
#define gpio_down( port_letter, number ) port_letter &= (0<<number)

/*
    Changing gpio modes:
        On an arduino UNO, the ports are as follows:

        Analog: ANALOG()
        Digital 0-7: DIGITALA()
        Digital 8-13: DIGITALB()

        The port number argument is constructed in the following way:

        <type><section><number>

        So, digital 5 = DIGITALA(5)

        The bank argument is the register bank that you are writing to

        This means that setting digital 5 to input mode would be:

        gpio_setInput(DIGITAL_BANKA(), DIGITALA(5))
*/
#define gpio_setOutput( port_letter, number ) port_letter |= (1<<number)
#define gpio_setInput( port_letter, number )  port_letter &= (0<<number)

#endif