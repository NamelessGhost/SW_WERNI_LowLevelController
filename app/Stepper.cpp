/*
 * Filename: Stepper.cpp
 * Author: André Krummenacher
 * Date: 12.11.2023
 * Description: Stepper Motor
 */

#include <stdio.h>
#include "Stepper.h"
#include "main.h"
#include "paradef.h"
#include "cmath"

uint32_t Stepper::sUsedTimerChannels = 0;

Stepper::Stepper(StepperConfig_t config) : Iinterruptable()
{
  SetConfiguration(config);  //Apply the configuration

  mpTimerHandle = STEPPER_STEP_TIMER_HANDLE;
  ReserveTimerChannel();

  mStepperState = OFF;
  mRotationState = STANDSTILL;
  mStepOutputState = OUTPUT_LOW;

  mCurrentAngularVeloctiy = 0;          //in rad/s
  mCurrentRotationAngle = 0;			//in rad/s
  mTargetRotationAngle = 0;
  mStepsRotated = 0;


}

static StepperConfig_t Stepper::GetDefaultConfiguration()
{
  StepperConfig_t lDefaultConfig;

  //Physical parameters
  lDefaultConfig.StartAngularVelocity = STEPPER_START_ANGULAR_VELOCITY;   //rad/s
  lDefaultConfig.TargetAngularVelocity = STEPPER_TARGET_ANGULAR_VELOCITY; //rad/s
  lDefaultConfig.AngularAcceleration = STEPPER_ANGULAR_ACCELERATION;      //rad/s^2
  lDefaultConfig.RolloutAngle = STEPPER_ROLLOUT_ANGLE;                    //rad

  //GPIO parameters
  lDefaultConfig.pGpioStepOutput = NULL;    //Step output GPIO peripheral pointer
  lDefaultConfig.GpioPinStepOutput = 0x00;  //Step output GPIO pin

  //Stepper driver parameters
  lDefaultConfig.DriverStepFactor = STEPPER_DRIVER_STEP_FACTOR; //Half-stepping
  lDefaultConfig.MotorStepFactor = STEPPER_MOTOR_STEP_FACTOR;   //Motor Steps/rotation

  return lDefaultConfig;
}

void Stepper::ReserveTimerChannel()
{
  //Make sure a channel is available
  if(sUsedTimerChannels < STEPPER_TIMER_MAX_CHANNELS)
  {
    sUsedTimerChannels++;
    mTimerChannel = sUsedTimerChannels * 4;
  }
}

StepperConfig_t Stepper::GetConfiguration()
{
  StepperConfig_t lConfig;

  //Physical parameters
  lConfig.StartAngularVelocity = mStartAngularVelocity;
  lConfig.TargetAngularVelocity = mTargetAngularVelocity;
  lConfig.AngularAcceleration = mAngularAcceleration;
  lConfig.RolloutAngle = mRolloutAngle;

  //GPIO parameters
  lConfig.pGpioStepOutput = mpGpioStepOutput;
  lConfig.GpioPinStepOutput = mGpioPinStepOutput;

  //Stepper driver parameters
  lConfig.DriverStepFactor = mDriverStepFactor;
  lConfig.MotorStepFactor = mMotorStepFactor;

  return lConfig;
}

void Stepper::SetConfiguration(StepperConfig_t config)
{
  //Physical parameters
  mStartAngularVelocity = config.StartAngularVelocity;
  mTargetAngularVelocity = config.TargetAngularVelocity;
  mAngularAcceleration = config.AngularAcceleration;
  mRolloutAngle = config.RolloutAngle;

  //GPIO parameters
  mpGpioStepOutput = config.pGpioStepOutput;
  mGpioPinStepOutput = config.GpioPinStepOutput;

  //Stepper driver parameters
  mDriverStepFactor = config.DriverStepFactor;
  mMotorStepFactor = config.MotorStepFactor;

  mDriveTrainFactor = mDriverStepFactor * mMotorStepFactor;
  assert_param(mpGpioStepOutput != NULL);
}

void Stepper::StartRotation(float angle)
{
  if(mStepperState != ROTATING)
  {
    mStepperState = ROTATING;
    mRotationState = ACCELERATING;
    mTargetRotationAngle = angle;

    //Zero parameters at start of rotation
    mStepsRotated = 0;
    mCurrentAngularVeloctiy = 0;
    mCurrentRotationAngle = 0;
    mCurrentRotationAngle = 0;

    volatile uint32_t* ptr = &velocitySWV;  //TODO:Remove DEBUG

    uint32_t timerCount = __HAL_TIM_GET_COUNTER(mpTimerHandle);
    __HAL_TIM_SET_COMPARE(mpTimerHandle, mTimerChannel, timerCount + CalculateTicksUntilNextStep()/2);
    HAL_TIM_OC_Start_IT(mpTimerHandle, mTimerChannel);
  }
  else
  {
    assert_param(false);
  }
}

void Stepper::StopRotation()
{
  mStepperState = HOLDING;
  mRotationState = STANDSTILL;

  HAL_TIM_OC_Stop_IT(mpTimerHandle, mTimerChannel);
}

bool Stepper::IsTimeToStartDecelerating()
{
  float lStopAngle = (mCurrentAngularVeloctiy * mCurrentAngularVeloctiy) / (2 * mAngularAcceleration);
  return ( (lStopAngle + mRolloutAngle) >= (mTargetRotationAngle - mCurrentRotationAngle) );
}

uint32_t Stepper::CalculateTicksUntilNextStep()
{
  uint32_t lRet = 0;
  mCurrentRotationAngle = mStepsRotated * mDriverStepFactor * mMotorStepFactor * _2PI;

  switch(mRotationState)
    {
      case ACCELERATING:
        if(mCurrentAngularVeloctiy < mTargetAngularVelocity)   //We still need to accelerate
        {
          mCurrentAngularVeloctiy = mAngularAcceleration *
                                    std::sqrt( (2 * mCurrentRotationAngle) / mAngularAcceleration) +
                                    mStartAngularVelocity;
        }
        else    //Target velocity reached
        {
          mRotationState = CONSTVELOCITY;
        }
        if(IsTimeToStartDecelerating()) mRotationState = DECCELERATING;
        break;

      case CONSTVELOCITY:
        if(IsTimeToStartDecelerating()) mRotationState = DECCELERATING;
        break;


      case DECCELERATING:
      {
        float lRemainingAngle = (mTargetRotationAngle - mCurrentRotationAngle - mRolloutAngle);
        if(mCurrentAngularVeloctiy > mStartAngularVelocity)   //We still need to decelerate
        {
          mCurrentAngularVeloctiy = mAngularAcceleration *
                                    std::sqrt( (2 * lRemainingAngle) / mAngularAcceleration);

          if(mCurrentAngularVeloctiy < mStartAngularVelocity) //Limit minimum velocity to prevent large period times
          {
            mCurrentAngularVeloctiy = mStartAngularVelocity;
          }
        }
        else
        {
          mRotationState = ROLLOUT;
        }
      }
        break;

      case ROLLOUT:
        if(mCurrentRotationAngle >= mTargetRotationAngle)
        {
          mRotationState = STANDSTILL;
          mCurrentAngularVeloctiy = 0;
          StopRotation();
        }
        break;

      default:
        break;
    }

  velocitySWV = (uint32_t)(mCurrentAngularVeloctiy*10000);  //TODO:Remove DEBUG

  if(mCurrentAngularVeloctiy > 0)
  {
    float lAbsRotationFrequency = std::abs(mCurrentAngularVeloctiy/_2PI);
    lRet = (uint32_t)(STEPPER_TIMER_CNT_FREQUENCY / (lAbsRotationFrequency / mDriveTrainFactor ));
    if(lRet > 0x0001FFFF)assert_param(false);   //TODO:Remove DEBUG, prevent ticks greater than timer cnt register size
  }

  return lRet;
}

void Stepper::OutputCompareIntCb(TIM_HandleTypeDef* htim)
{
  uint32_t lPreviousCompareValue = __HAL_TIM_GET_COMPARE(mpTimerHandle, mTimerChannel);

  if(mStepOutputState == OUTPUT_LOW){
    mStepOutputState = OUTPUT_HIGH;
    mStepsRotated += 1;
  }
  else mStepOutputState = OUTPUT_LOW;

  HAL_GPIO_WritePin(mpGpioStepOutput, mGpioPinStepOutput, (GPIO_PinState)mStepOutputState);

  uint32_t ticks = CalculateTicksUntilNextStep()/2;

  __HAL_TIM_SET_COMPARE(mpTimerHandle, mTimerChannel, lPreviousCompareValue + ticks);   // Divide by to to get half period step output low and half period step output high
}
