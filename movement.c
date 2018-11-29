/*
 * movement.c
 *
 *  Created on: Sep 4, 2018
 *      Author: smpick
 */
//GOAL: be able to move roomba forward/backward and rotate
#include "movement.h"
#include "open_interface.h"

void move_forward(oi_t *sensor, int millimeters) { //function that returns 'sum' of distance traveled
    int sum = 0;
    uart_init();        //initializes uart connection
    oi_setWheels(150, 150); // move forward; full speed

    while (sum < millimeters) {  //takes in variable of millimeters; this is the desired distance to travel
        oi_update(sensor);
        char string[50];
        /*IF BUMP INTO SOMETHING, STOP*/
        if (sensor->bumpLeft) {
            // respond to left bumper being pressed
            sum += sensor->distance; //sensor tracks distance and puts it into sum
            oi_setWheels(0,0); // each wheel at a speed of zero
            sprintf(string, "\nBUMPED LEFT, %d\n", (sum/10));
            uart_sendStr(string);
            break;
        }
        else if (sensor->bumpRight) {
            // respond to left bumper being pressed
            sum += sensor->distance; //sensor tracks distance and puts it into sum
            oi_setWheels(0,0); // each wheel at a speed of zero
            sprintf(string, "\nBUMPED RIGHT, %d\n", (sum/10));
            uart_sendStr(string);
            break;
        }

        /*IF DETECT CLIFF, STOP*/
        if (sensor -> cliffLeft) {     //if the left cliff sensor is tripped
            oi_setWheels(0,0);          //stop moving
            sum += sensor->distance; //sensor tracks distance and puts it into sum
            sprintf(string, "\nCLIFF LEFT, %d\n", (sum/10));
            uart_sendStr(string);
            break;
        }
        if (sensor -> cliffFrontLeft) {     //if the front left cliff sensor is tripped
            oi_setWheels(0,0);           //stop moving
            sum += sensor->distance; //sensor tracks distance and puts it into sum
            sprintf(string, "\nCLIFF FRONT LEFT, %d\n", (sum/10));
            uart_sendStr(string);
            break;
        }
        if (sensor -> cliffRight) {     //if the right cliff sensor is tripped
            oi_setWheels(0,0);          //stop moving
            sum += sensor->distance; //sensor tracks distance and puts it into sum
            sprintf(string, "\nCLIFF RIGHT, %d\n", (sum/10));
            uart_sendStr(string);
            break;
        }
        if (sensor -> cliffFrontRight) {     //if the front right cliff sensor is tripped
            oi_setWheels(0,0);           //stop moving
            sum += sensor->distance; //sensor tracks distance and puts it into sum
            sprintf(string, "\nCLIFF FRONT RIGHT, %d\n", (sum/10));
            uart_sendStr(string);
            break;
        }

        /*IF TAPE DETECTED, STOP*/
        uint16_t signals[4];
        signals[0] = sensor->cliffLeftSignal;
        signals[1] = sensor->cliffFrontLeftSignal;
        signals[2] = sensor->cliffFrontRightSignal;
        signals[3] = sensor->cliffRightSignal;
        if (2800 < signals[0]) {
            oi_setWheels(0,0);
            sum += sensor->distance; //sensor tracks distance and puts it into sum
            sprintf(string, "\nTAPE LEFT, %d\n", (sum/10));
                        uart_sendStr(string);
                        break;
        }
        if (2800 < signals[1]) {
            oi_setWheels(0,0);
            sum += sensor->distance; //sensor tracks distance and puts it into sum
            sprintf(string, "\nTAPE FRONT LEFT, %d\n", (sum/10));
            uart_sendStr(string);
            break;
        }
        if (2800 < signals[2]) {
            oi_setWheels(0,0);
            sum += sensor->distance; //sensor tracks distance and puts it into sum
            sprintf(string, "\nTAPE FRONT RIGHT, %d\n", (sum/10));
            uart_sendStr(string);
            break;
        }
        if (2800 < signals[3]) {
            oi_setWheels(0,0);
            sum += sensor->distance; //sensor tracks distance and puts it into sum
            sprintf(string, "\nTAPE RIGHT, %d\n", (sum/10));
            uart_sendStr(string);
            break;
        }

        sum += sensor->distance;
    }
    oi_setWheels(0,0); // each wheel at a speed of zero
    //return sum;
}

void move_backward(oi_t *sensor, int millimeters) { //function that returns 'sum' of distance traveled
        int sum = 0;
        oi_setWheels(-150, -150); // move forward; full speed

         while (sum < millimeters) {  //takes in variable of millimeters; this is the desired distance to travel
                  oi_update(sensor);
                  sum -= sensor->distance; //sensor tracks distance and puts it into sum
         }
        oi_setWheels(0,0); // each wheel at a speed of zero
}



void turn_counterclockwise(oi_t *sensor, int degrees) {
         int total = 0;
         oi_setWheels(-500, 500); // one wheel forward and one wheel back so that the roomba turns; first number negative so moves counterclockwise

         while (total < degrees) {
         oi_update(sensor);
         total -= sensor->angle; //need -= to ensure it goes counterclockwise
         }

         oi_setWheels(0,0); // each wheel at a speed of zero

}

void turn_clockwise(oi_t *sensor, int degrees) {
         int total = 0;
         oi_setWheels(500, -500); // one wheel forward and one wheel back so that the roomba turns; first number negative so moves counterclockwise

         while (total < degrees) {
         oi_update(sensor);
         total += sensor->angle; //need -= to ensure it goes counterclockwise
         }

         oi_setWheels(0,0); // each wheel at a speed of zero

}

