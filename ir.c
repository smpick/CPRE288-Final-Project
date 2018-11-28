/*
 * ir.c
 *
 *  Created on: Nov 13, 2018
 *      Author: smpick
 */
#include "ir.h"


void ir_init (void) {
    //enable ADC 0 module on port PB4
    // SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    SYSCTL_RCGCGPIO_R |= 0b000010;

    //enable clock for ADC
    SYSCTL_RCGCADC_R |= 0x1;

    //enable port B pin 4 to work as alternate function
    GPIO_PORTB_AFSEL_R |= 0x10;

    //set pin to input - 0
    GPIO_PORTB_DEN_R &= 0xEF;

    //disable analog isolation for the pin
    GPIO_PORTB_AMSEL_R |= 0x10;

    //set direction of pin
    // GPIO_PORTB_DIR_R = 0xEF;

    //initialize the port trigger source as processor (default)
    GPIO_PORTB_ADCCTL_R = 0x00;

    //disable SS1 sample sequencer to configure it
    // ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN1;
    ADC0_ACTSS_R &= ~(0x00000002);

    //initialize the ADC trigger source as processor (default)
    // ADC0_EMUX_R = ADC_EMUX_EM1_PROCESSOR;
    ADC0_EMUX_R = ~(0x000000F0);

    // Sample sequence Control
    // ADC0_ADCSSCTL_R = 0x0007;

    // set 1st sample to use the AIN10 ADC pin
    ADC0_SSMUX1_R |= 0x000A;

    //enable raw interrupt staus
    ADC0_SSCTL1_R |= (0x0004 | 0x0002);

    // enable oversampling to average
    // ADC0_SAC_R |= ADC_SAC_AVG_64X;
    ADC0_SAC_R |= 0x4;

    // re-enable ADC0 SS1
    // ADC0_ACTSS_R |= ADC_ACTSS_ASEN1;
    ADC0_ACTSS_R |= 0x00000002;
}

