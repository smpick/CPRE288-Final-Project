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
         oi_setWheels(500, 500); // move forward; full speed

        while (sum < millimeters) {  //takes in variable of millimeters; this is the desired distance to travel
         oi_update(sensor);
           /*  if (sensor->bumpLeft) {
              // respond to left bumper being pressed
                 sum += sensor->distance; //sensor tracks distance and puts it into sum
                 oi_setWheels(0,0); // each wheel at a speed of zero
             }
             else if (sensor->bumpRight) {
              // respond to left bumper being pressed
                 sum += sensor->distance; //sensor tracks distance and puts it into sum
                 oi_setWheels(0,0); // each wheel at a speed of zero
             }
             else {
                 sum += sensor->distance; //sensor tracks distance and puts it into sum
             } */
          sum += sensor->distance;
         }
         oi_setWheels(0,0); // each wheel at a speed of zero
         //return sum;
}

void move_backward(oi_t *sensor, int millimeters) { //function that returns 'sum' of distance traveled
        int sum = 0;
        oi_setWheels(-500, -500); // move forward; full speed

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

