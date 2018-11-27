/*
 * edge_detection.c
 *
 *  Created on: Nov 13, 2018
 *      Author: smpick
 *
 *
 *      This file is to practice moving the bot and using the edge sensor to stop the bot from falling off a cliff.
 *
 */

//#include "lcd.h"
#include "movement.h"
#include "open_interface.h"
#include "timer.h"
#include "inc/tm4c123gh6pm.h"


//int main(void) {
//    oi_t *sensor_data = oi_alloc();  //Allocate and clear all memory for OI Struct
//    oi_init(sensor_data);           //Initialize open interface communication with IRobot.
//
//    oi_setWheels(200, 200); // move forward
//
//    while (1) {
//        oi_update(sensor_data);
//        if (sensor_data -> cliffLeft) {     //if the left cliff sensor is tripped
//            oi_setWheels(0,0);          //stop moving
//           // move_backward(sensor_data, 150);   //move back 15cm
//        }
//        if (sensor_data -> cliffFrontLeft) {     //if the front left cliff sensor is tripped
//            oi_setWheels(0,0);          //stop moving
//          //  move_backward(sensor_data, 150);   //move back 15cm
//        }
//        if (sensor_data -> cliffRight) {     //if the right cliff sensor is tripped
//            oi_setWheels(0,0);          //stop moving
//         //   move_backward(sensor_data, 150);   //move back 15cm
//        }
//        if (sensor_data -> cliffFrontRight) {     //if the front right cliff sensor is tripped
//            oi_setWheels(0,0);          //stop moving
//          //  move_backward(sensor_data, 150);   //move back 15cm
//        }
//    }
//    oi_setWheels(0,0); // each wheel at a speed of zero
//    oi_free(sensor_data);
//}
