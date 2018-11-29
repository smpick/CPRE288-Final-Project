/*
 * mainfinal.c
 *
 *  Created on: Nov 28, 2018
 *      Author: smpick
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

    oi_t *sensor_data = oi_alloc();  //Allocate and clear all memory for OI Struct
    oi_init(sensor_data);           //Initialize open interface communication with IRobot

    while (1) {
        /*DRIVING*/
        char nextChar;
        nextChar = uart_receive(); //take from the keyboard
        if (nextChar != '\0' || nextChar!='\r') {
            if(nextChar == 'f') {               //f moves bot forward at speed of 150
                oi_update(sensor_data);
                move_forward(sensor_data, 160);
            }
            if(nextChar == 'l') {               //l turns bot 90 degrees left
                oi_update(sensor_data);
                turn_clockwise(sensor_data, 90);
            }
            if(nextChar == 'r') {               //r turns bot 90 degrees right
                oi_update(sensor_data);
                turn_counterclockwise(sensor_data, 90);
            }
            /*SWEEPING FUNCTION*/
            if (nextChar == 's') {              //s initiates the sweeping function which gives ir and sonar data
                servo_sweep();      // function that not only sweeps the servo, but also collects IR and Sonar data
            }
//            /*ALARM FUNCTION*/
//            if (nextChar == 'm') {
//                //SOUND ALARM
//            }

        }
    }
}
