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

Stepper::Stepper(uint32_t timerChannel) : Iinterruptable()
{
  mTimerChannel = timerChannel;
  mpTimerHandle = STEPPER_STEP_TIMER_HANDLE;
  mpGpioStepOutput = GPIOC;
  mGpioPinStepOutput = GPIO_PIN_0;

  mStepperState = OFF;
  mRotationState = STANDSTILL;
  mStepOutputState = OUTPUT_LOW;

  mStartAngularVelocity = 0.1 * PI;     //in rad/s
  mTargetAngularVelocity = 30 * PI;     //in rad/s
  mAngularAcceleration = 2 * PI;        //in rad/s^2
  mCurrentAngularVeloctiy = 0;          //in rad/s
  mStepsRotated = 0;
  mCurrentRotationAngle = 0;
  mTargetRotationAngle = 0;

  mDriverStepFactor = 0.5;
  mMotorStepFactor = 1.0/200;
  mDriveTrainFactor = mDriverStepFactor * mMotorStepFactor;
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
  return ( lStopAngle >= (mTargetRotationAngle - mCurrentRotationAngle) );
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
        if(mCurrentAngularVeloctiy > mStartAngularVelocity)   //We still need to decelerate
        {
          mCurrentAngularVeloctiy = mStartAngularVelocity -
                                  ( mAngularAcceleration *
                                    std::sqrt( (2 * mCurrentRotationAngle) / mAngularAcceleration) );
        }
        else
        {
          mRotationState = ROLLOUT;
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

  velocitySWV = (uint32_t)mCurrentAngularVeloctiy;  //TODO:Remove DEBUG

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
