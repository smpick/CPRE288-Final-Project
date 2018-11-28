/*
 * sonar.c
 *
 *  Created on: Nov 13, 2018
 *      Author: smpick
 */
#include "sonar.h"

void sonar_init(void) {

    GPIO_PORTB_DEN_R |= 0b00001000; //port b bit 3

    GPIO_PORTB_AFSEL_R |= 0b00001000; //bit 3  enabling alt functions
    GPIO_PORTB_PCTL_R |= 0x00007000;

}

void send_pulse(void) {
    /* send out a pulse on PB3 */
    TIMER3_CTL_R &= ~(TIMER_CTL_TBEN);  //starts timer b
    //GPIO_PORTB_PCTL_R &= ~0x0000700;  //chooses alternate function???????

    GPIO_PORTB_AFSEL_R &= ~0b00001000;  //pin 3 on port b set as alternate function disabled
    GPIO_PORTB_DIR_R |= 0x08; // set PB3 as output
    GPIO_PORTB_DATA_R |= 0x08; // set PB3 to high

    // wait at least 5 microseconds based on data sheet
    timer_waitMicros(10);

    GPIO_PORTB_DATA_R &= 0xF7; // set PB3 to low
    GPIO_PORTB_DIR_R &= 0xF7; // set PB3 as input

    GPIO_PORTB_AFSEL_R |= 0b00001000; //enables alt function
    GPIO_PORTB_PCTL_R |= 0x00007000;

    TIMER3_CTL_R |= TIMER_CTL_TBEN;

    //timer_waitMillis(1000);

}
