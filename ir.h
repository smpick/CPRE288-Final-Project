/*
 * ir.h
 *
 *  Created on: Nov 27, 2018
 *      Author: smpick
 *
 *
 *      The ir sensor is useful for detecting edges and we can use it to detect objects.
 */

#ifndef IR_H_
#define IR_H_

#include <inc/tm4c123gh6pm.h>
#include "timer.h"
#include "open_interface.h"

void ir_init(void);

#endif /* IR_H_ */
