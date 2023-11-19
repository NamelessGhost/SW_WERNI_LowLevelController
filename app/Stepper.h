/*
 * Filename: Stepper.h
 * Author: André Krummenacher
 * Date: 12.11.2023
 */

#ifndef STEPPER_H_
#define STEPPER_H_

#include <vector>
#include "InterruptRouting.h"

#include "tim.h"
#include "stm32g4xx_hal.h"

enum StepperState {
    OFF,
    HOLDING,
    ROTATING
};

enum RotationState {
  STANDSTILL,
  ACCELERATING,
  CONSTVELOCITY,
  DECCELERATING,
  ROLLOUT
};

enum OutputState {
  OUTPUT_LOW = 0U,
  OUTPUT_HIGH
};


class Stepper : public Iinterruptable
{
public:
  Stepper(uint32_t timerChannel);
  void StartRotation(float angle);    //Postitve -> CW  Negative -> CCW
  void StopRotation();
  void OutputCompareIntCb(TIM_HandleTypeDef* htim) override;

protected:

private:
  uint32_t CalculateTicksUntilNextStep();
  bool IsTimeToStartDecelerating();
  TIM_HandleTypeDef* mpTimerHandle;
  uint32_t mTimerChannel;
  GPIO_TypeDef* mpGpioStepOutput;
  uint16_t mGpioPinStepOutput;


  StepperState mStepperState;
  RotationState mRotationState;

  //All parameters are specified for the motor output shaft in radian!
  float mTargetAngularVelocity;
  float mStartAngularVelocity;
  float mCurrentAngularVeloctiy;
  float mCurrentRotationAngle;
  float mTargetRotationAngle;
  float mAngularAcceleration;
  float mRolloutAngle;

  //Steps in step signal
  int32_t mStepsRotated;
  OutputState mStepOutputState;

  float mDriverStepFactor;    //e.g. 1/2 for half stepping mode
  float mMotorStepFactor;     // 1/(Steps per full rotation of motor shaft)
  float mDriveTrainFactor;    //mDriverStepFactor * mMotorStepFactor

  //For Debug
  volatile uint32_t velocitySWV;



};

#endif /* STEPPER_H_ */
