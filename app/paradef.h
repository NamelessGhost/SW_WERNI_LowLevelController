/*
 * paradef.h
 *
 *  Created on: Dec 28, 2019
 *      Author: axel
 */

#ifndef PARADEF_H_
#define PARADEF_H_
#include <stdint.h>

#define STEPPER_STEP_TIMER_HANDLE         &htim1
#define STEPPER_TIMER_FREQUENCY           170000000   //Timer input clock
#define STEPPER_TIMER_PRESCALER           850         //Timer internal prescaler
#define STEPPER_TIMER_CNT_FREQUENCY       STEPPER_TIMER_FREQUENCY / STEPPER_TIMER_PRESCALER
#define STEPPER_TIMER_MAX_CHANNELS        4
#define STEPPER_USE_UART

#define STEPPER_START_ANGULAR_VELOCITY    1 * PI  //rad/s
#define STEPPER_TARGET_ANGULAR_VELOCITY   2 * PI    //rad/s
#define STEPPER_ANGULAR_ACCELERATION      4 * PI    //rad/s^2
#define STEPPER_ROLLOUT_ANGLE             PI / 100   //rad

#define STEPPER_DRIVER_STEP_FACTOR        1.0/2     //Half-stepping
#define STEPPER_MOTOR_STEP_FACTOR         1.0/200   //Motor 200Steps/rotation

#define COMHANDLER_UART_RXBUF_SIZE        256       //Rx Buffer size in bytes
#define COMHANDLER_UART_TXBUF_SIZE        256       //Rx Buffer size in bytes
#define COMHANDLER_UART_PREAMBLE          "AAAB"
#define COMHANDLER_UART_PREAMBLE_LEN      4

#define CUBEGRID_HOMING_ANGULAR_VELOCITY  1 * PI    //rad/s
#define CUBEGRID_TARGET_ANGULAR_VELOCITY  2 * PI    //rad/S
#define CUBEGRID_GEAR_FACTOR              3

#define MAGAZINESLOT_ANGLE_PER_CUBE       300       //degrees

#define PI  3.14159f    //float
#define _2PI 6.28318f   //float
#define DEG_TO_RAD(degrees) ((degrees) * PI / 180.0)


#endif /* PARADEF_H_ */

