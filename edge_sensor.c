/*
 * edge_sensor.c
 *
 *  Created on: Nov 26, 2018
 *      Author: smpick
 */

//#include "lcd.h"
#include "movement.h"
#include "open_interface.h"
#include "timer.h"
#include "inc/tm4c123gh6pm.h"


int main(void) {
   //lcd_init();
    oi_t *sensor_data = oi_alloc();  //Allocate and clear all memory for OI Struct
    oi_init(sensor_data);           //Initialize open interface communication with IRobot.

    oi_setWheels(150,150); // move forward
    uint16_t signals[4];

    while (1) {
        timer_waitMillis(1000);
        oi_update(sensor_data);
        signals[0] = sensor_data->cliffLeftSignal;
        signals[1] = sensor_data->cliffFrontLeftSignal;
        signals[2] = sensor_data->cliffFrontRightSignal;
        signals[3] = sensor_data->cliffRightSignal;

        //lcd_printf("%u, %u, %u, %u", signals[0], signals[1], signals[2], signals[3]); //For testing purposes
        if ( signals[0] > 2000 ) {
            oi_setWheels(0,0);
        }
        if ( signals[1] > 2000 ) {
            oi_setWheels(0,0);
        }
        if ( signals[2] > 2000 ) {
            oi_setWheels(0,0);
        }
        if ( signals[3] > 2000 ) {
            oi_setWheels(0,0);
        }
    }
}
