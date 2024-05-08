/*
 * paradef.h
 *
 *  Created on: Jan 01, 2024
 *      Author: andre
 */

#ifndef PARADEF_H_
#define PARADEF_H_
#include <stdint.h>

#define PI  (3.14159f)    //float
#define _2PI (6.28318f)   //float
#define DEG_TO_RAD(degrees) ((degrees) * PI / 180.0)

#define STEPPER_STEP_TIMER_HANDLE           (&htim1)
#define STEPPER_TIMER_FREQUENCY             (170000000)   //Timer input clock
#define STEPPER_TIMER_PRESCALER             (850)         //Timer internal prescaler
#define STEPPER_TIMER_CNT_FREQUENCY         (STEPPER_TIMER_FREQUENCY / STEPPER_TIMER_PRESCALER)
#define STEPPER_TIMER_MAX_CHANNELS          (4)
//TODO: Define this after soldering bridges on stepper drivers #define STEPPER_USE_UART

//Stepper default motion parameters
#define STEPPER_START_ANGULAR_VELOCITY      (1 * PI)    //rad/s
#define STEPPER_TARGET_ANGULAR_VELOCITY     (10 * PI)    //rad/s
#define STEPPER_ANGULAR_ACCELERATION        (70 * PI)    //rad/s^2
#define STEPPER_ROLLOUT_ANGLE               (PI / 100)  //rad

#define STEPPER_DRIVER_STEP_FACTOR          (1.0f/2)    //Half-stepping
#define STEPPER_MOTOR_STEP_FACTOR           (1.0f/200)  //Motor 200Steps/rotation

#define COMHANDLER_UART_HANDLE				      &hlpuart1 //&huart1	  //UART peripheral
#define COMHANDLER_UART_RXBUF_SIZE          (256)       //Rx Buffer size in bytes
#define COMHANDLER_UART_TXBUF_SIZE          (256)       //Rx Buffer size in bytes
#define COMHANDLER_UART_PREAMBLE            ("AAAB")
#define COMHANDLER_UART_PREAMBLE_LEN        (4)

//Cubegrid motion parameters
#define CUBEGRID_HOMING_ANGULAR_VELOCITY    (2 * PI)        //rad/s
#define CUBEGRID_TARGET_ANGULAR_VELOCITY    (5 * PI)        //rad/S
#define CUBEGRID_ANGULAR_ACCELERATION       (70 * PI)       //rad/s^2
#define CUBEGRID_GEAR_FACTOR                (4)
//Configuration for timer in PWM input mode
#define CUBEGRID_HALL_IC_TIMER_HANDLE		    (&htim2)
#define CUBEGRID_HALL_IC_CH_PERIOD			    TIM_CHANNEL_1
#define CUBEGRID_HALL_IC_CH_DUTYCYCLE		    TIM_CHANNEL_2
//HALL-Sensor PWM frequency limits(1.8kHz - 2.2kHz)
#define CUBEGRID_HALL_SIGNAL_PERIOD_MIN     (77272)
#define CUBEGRID_HALL_SIGNAL_PERIOD_MAX     (94444)
#define CUBEGRID_HALL_SIGNAL_AVERAGING      (20)
#define CUBEGRID_HALL_SIGNAL_THRESHOLD      (45000)

#define LIFT_HOMING_VERTICAL_VELOCITY       (0.1f)          //m/s
#define LIFT_TARGET_VERTICAL_VELOCITY       (100)//(1)             //m/s
#define LIFT_GEAR_FACTOR                    (10 * _2PI)     //rad/m
#define LIFT_VERTICAL_TRAVEL                (0.15f)         //m

#define MAGAZINESLOT_ANGLE_PER_CUBE         (300)           //degrees
#define MAGAZINESLOT_ANGLE_PRIME            (90)            //degrees
#define MAGAZINESLOT_INVERT_ROTATION        (true)

#define ADC_PERIODIC_TRIGGER_TIMER_HANDLE         (&htim7)        //Used to trigger regular adc conversions

#define ENERGY_MEASUREMENT_TIMER_CNT_PERIOD       (0.01f)         //Timer update period
#define ENERGY_MEASUREMENT_ADC_HANDLE             (&hadc1)
#define ENERGY_MEASUREMENT_ADC_VREF               (3.3f)          //ADC fullscale voltage
#define ENERGY_MEASUREMENT_ADC_RESOLUTION         (4096)          //12bit
#define ENERGY_MEASUREMENT_VDD_DIF_GAIN           (90.9091e-3f)   //Voltage divider gain R2/(R1+R2)
#define ENERGY_MEASUREMENT_CURR_AMP_GAIN          (50)            //INA186 gain in V/V
#define ENERGY_MEASUREMENT_CURR_SHUNT_R           (0.02f)         //Shunt resistance in Ohm

#define LIGHT_BARRIER_PWM_TIMER_HANDLE            (&htim3)        //Used to generate PWM-signal driving LED
#define LIGHT_BARRIER_PWM_TIMER_CHANNEL           (TIM_CHANNEL_2)
#define LIGHT_BARRIER_INPUTS_ADC_HANDLE           (&hadc2)
#define LIGHT_BARRIER_CLOSED_THRESHOLD            (1000)          //Threshold voltage in ADC-samples 0-4096 -> 0-3.3V

#define BUTTON_LONG_PRESS_THRESHOLD_MS            (500)   //Pressing longer than 500ms results in a long press

#endif /* PARADEF_H_ */

