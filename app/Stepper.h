/*
 * Filename: Stepper.h
 * Author: André Krummenacher
 * Date: 22.11.2023
 */

#ifndef STEPPER_H_
#define STEPPER_H_

#include <vector>
#include "InterruptRouting.h"
#include "paradef.h"
#include "tim.h"
#include "stm32g4xx_hal.h"
#include "Mutex.h"

#define FULL_ROTATION_RAD   _2PI
#define HALF_ROTATION_RAD   PI

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

  GPIO_TypeDef* pGpioEnableOutput;
  uint16_t GpioPinEnableOutput;

  GPIO_TypeDef* pGpioDirectionOutput;
  uint16_t GpioPinDirectionOutput;

  GPIO_TypeDef* pGpioMS1Output;
  uint16_t GpioPinMS1Output;

  GPIO_TypeDef* pGpioMS2Output;
  uint16_t GpioPinMS2Output;

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
  void StartRotationBlocking(float angle);  //Postitve -> CW  Negative -> CCW
  void StopRotation(void);
  void Enable(bool enable);
  StepperState GetState(void);


  void OutputCompareIntCb(TIM_HandleTypeDef* htim) override;

protected:

private:
  //**** Function prototypes ****
  void ReserveTimerChannel(void);
  void FreeTimerChannel(void);
  bool IsTimeToStartDecelerating(void);
  uint32_t CalculateTicksUntilNextStep(void);

  void SetDirection(bool cw);  //true= CW, false= CCW
  void SetDriverStepFactor(float stepFactor);
  //**** Variables ****
  static bool sUsedTimerChannels[];
  Mutex mMutex;
  bool mEnabled;

  TIM_HandleTypeDef* mpTimerHandle;
  int32_t mTimerChannel;
  HAL_TIM_ActiveChannel mTimerActiveChannel;

  GPIO_TypeDef* mpGpioStepOutput;
  uint16_t mGpioPinStepOutput;

  GPIO_TypeDef* mpGpioEnableOutput;
  uint16_t mGpioPinEnableOutput;

  GPIO_TypeDef* mpGpioDirectionOutput;
  uint16_t mGpioPinDirectionOutput;

  GPIO_TypeDef* mpGpioMS1Output;
  uint16_t mGpioPinMS1Output;

  GPIO_TypeDef* mpGpioMS2Output;
  uint16_t mGpioPinMS2Output;

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
