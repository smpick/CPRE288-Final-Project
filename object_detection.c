/*
 * object_detection.c
 *
 *  Created on: Nov 27, 2018
 *      Author: smpick
 *
 *
 *      GOAL: this program should use the servo to scan for objects using the ir sensor. Once an object
 *      is found, the servo should stop and the sonar sensor should return the distance away the object is.
 *
 */

#include "servo.h"
#include "stdio.h"
#include "timer.h"
#include "inc/tm4c123gh6pm.h"
#include "stdbool.h"
#include "math.h"
#include "uart.h"
#include "movement.h"
#include "stdlib.h"
#include "open_interface.h"
#include "ir.h"
#include "sonar.h"




void main() {
    uart_init();        //initializes uart connection
    TIMER1_init();      //initializes servo motor
    clock_init();       //initializes servo clock
    sonar_init();       //initializes sonar
    ir_init();          //initializes ir
    uart_sendStr("\nIR object detect");

    while (1) {
        servo_sweep();      // function that not only sweeps the servo, but also collects IR and Sonar data
    }
}
