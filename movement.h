
#ifndef MOVEMENT_H_
#define MOVEMENT_H_


#include "open_interface.h"

void move_forward(oi_t *sensor, int millimeters);

void move_backward(oi_t *sensor, int millimeters);

void turn_counterclockwise(oi_t *sensor, int degrees);

void turn_clockwise(oi_t *sensor, int degrees);


#endif /* TIMER_H_ */
