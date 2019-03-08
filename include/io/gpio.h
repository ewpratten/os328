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

#define OUTPUT 1
#define INPUT  0

#define HIGH 1
#define LOW 0

/* Better alternative for gpio. From digitalWriteFast library */

#define __digitalPinToPortReg(P) \
(((P) >= 0 && (P) <= 7) ? &PORTD : (((P) >= 8 && (P) <= 13) ? &PORTB : &PORTC))
#define __digitalPinToDDRReg(P) \
(((P) >= 0 && (P) <= 7) ? &DDRD : (((P) >= 8 && (P) <= 13) ? &DDRB : &DDRC))
#define __digitalPinToPINReg(P) \
(((P) >= 0 && (P) <= 7) ? &PIND : (((P) >= 8 && (P) <= 13) ? &PINB : &PINC))
#define __digitalPinToBit(P) \
(((P) >= 0 && (P) <= 7) ? (P) : (((P) >= 8 && (P) <= 13) ? (P) - 8 : (P) - 14))

#define ERROR_SEQUENCE 0b10101010 //digitalReadFast will return this value if pin number is not constant

// general macros/defines
#ifndef BIT_READ
# define BIT_READ(value, bit)  ((value) &   (1UL << (bit)))
#endif
#ifndef BIT_SET
# define BIT_SET(value, bit)   ((value) |=  (1UL << (bit)))
#endif
#ifndef BIT_CLEAR
# define BIT_CLEAR(value, bit) ((value) &= ~(1UL << (bit)))
#endif
#ifndef BIT_WRITE
# define BIT_WRITE(value, bit, bitvalue) (bitvalue ? BIT_SET(value, bit) : BIT_CLEAR(value, bit))
#endif


#define gpio_write(P, V) \
if (__builtin_constant_p(P) && __builtin_constant_p(V)) { \
  BIT_WRITE(*__digitalPinToPortReg(P), __digitalPinToBit(P), (V)); \
}




#define gpio_mode(P, V) \
if (__builtin_constant_p(P) && __builtin_constant_p(V)) { \
  BIT_WRITE(*__digitalPinToDDRReg(P), __digitalPinToBit(P), (V)); \
}



#define gpio_read(P) ( (byte) __digitalReadFast((P)) )
#define __digitalReadFast(P ) \
  (__builtin_constant_p(P) ) ? ( \
  ( BIT_READ(*__digitalPinToPINReg(P), __digitalPinToBit(P))) ) : \
  ERROR_SEQUENCE


void gpio_test();

#endif