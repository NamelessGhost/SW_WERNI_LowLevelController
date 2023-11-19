/*
 * paradef.h
 *
 *  Created on: Dec 28, 2019
 *      Author: axel
 */

#ifndef PARADEF_H_
#define PARADEF_H_
#include <stdint.h>

#define STEPPER_STEP_TIMER_HANDLE   &htim1
#define STEPPER_TIMER_FREQUENCY     170000000   //Timer input clock
#define STEPPER_TIMER_PRESCALER     850         //Timer internal prescaler
#define STEPPER_TIMER_CNT_FREQUENCY STEPPER_TIMER_FREQUENCY / STEPPER_TIMER_PRESCALER

#define PI  3.14159f    //float
#define _2PI 6.28318f   //float


#endif /* PARADEF_H_ */

