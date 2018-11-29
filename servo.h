/*
 * servo_timer_init.h
 *
 *  Created on: Nov 27, 2018
 *      Author: smpick
 */

#ifndef SERVO_H_
#define SERVO_H_

#include "inc/tm4c123gh6pm.h"
#include "timer.h"
#include "math.h"
#include "stdio.h"
#include "uart.h"
#include "sonar.h"
#include "ir.h"

void TIMER3B_Handler(void);

void clock_init(void);

void TIMER1_init(void);

void servo_sweep(void);

#endif /* SERVO_H_ */
