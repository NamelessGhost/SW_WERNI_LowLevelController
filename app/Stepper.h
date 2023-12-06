/*
 * Filename: Stepper.h
 * Author: André Krummenacher
 * Date: 22.11.2023
 */

#ifndef STEPPER_H_
#define STEPPER_H_

#include <vector>
#include "InterruptRouting.h"

#include "tim.h"
#include "stm32g4xx_hal.h"
#include "Mutex.h"

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

typedef struct {
  //Physical parameters
  float StartAngularVelocity;       //in rad/s
  float TargetAngularVelocity;      //in rad/s
  float AngularAcceleration;        //in rad/s^2
  float RolloutAngle;               //in rad

  //GPIO parameters
  GPIO_TypeDef* pGpioStepOutput;
  uint16_t GpioPinStepOutput;

  //Stepper driver parameters
  float DriverStepFactor = 0.5;
  float MotorStepFactor = 1.0/200;
}StepperConfig_t;


class Stepper : public Iinterruptable
{
public:
  //**** Function prototypes ****
  Stepper(StepperConfig_t config);
  static StepperConfig_t GetDefaultConfiguration(void);
  StepperConfig_t GetConfiguration(void);
  void SetConfiguration(StepperConfig_t config);
  void StartRotation(float angle);    //Postitve -> CW  Negative -> CCW
  void StopRotation(void);
  void OutputCompareIntCb(TIM_HandleTypeDef* htim) override;

protected:

private:
  //**** Function prototypes ****
  void ReserveTimerChannel(void);
  void FreeTimerChannel(void);
  uint32_t CalculateTicksUntilNextStep(void);
  bool IsTimeToStartDecelerating(void);

  //**** Variables ****
  static bool sUsedTimerChannels[];
  Mutex mMutex;
  TIM_HandleTypeDef* mpTimerHandle;
  int32_t mTimerChannel;
  HAL_TIM_ActiveChannel mTimerActiveChannel;
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
