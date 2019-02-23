#include "../include/io/gpio.h"

/*
    gpio_test() tests the gpio. 
    It will quickly send a pulse to pin 13 on boot to verify that the gpio is working

    Most of the reason behine this function is to test the gpio macros during compile time
*/
void gpio_test(){
    // Pulse to pin 13
    gpio_setOutput(DIGITAL_BANKB, DIGITALA(7)); // Set output mode
    gpio_up(DIGITALB(), DIGITALB(7)); // Write
    gpio_down(DIGITALB(), DIGITALB(7));
}