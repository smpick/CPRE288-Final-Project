/*
 * sonar.h
 *
 *  Created on: Nov 27, 2018
 *      Author: smpick
 *
 *
 *      The sonar sensor is useful for accurately returning the distance away an object is.
 */

#ifndef SONAR_H_
#define SONAR_H_

#include <inc/tm4c123gh6pm.h>
#include "timer.h"
#include "open_interface.h"

void sonar_init(void);

void send_pulse(void);


#endif /* SONAR_H_ */
