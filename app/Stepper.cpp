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



Stepper::Stepper(uint32_t timerChannel) : Iinterruptable()
{
  mTimerChannel = timerChannel;
  mpTimerHandle = STEPPER_STEP_TIMER_HANDLE;

  mStepperState = OFF;
  mDegreesToRotate = 0;
}

void Stepper::StartRotation(int32_t degrees)
{
  if(mStepperState != ROTATING)
  {
    mStepperState = ROTATING;
    uint32_t timerCount = __HAL_TIM_GET_COUNTER(mpTimerHandle);
    __HAL_TIM_SET_COMPARE(mpTimerHandle, mTimerChannel, timerCount + 1000);
    HAL_TIM_OC_Start_IT(mpTimerHandle, mTimerChannel);
  }
  else
  {
    assert_param(false);
  }
}

void Stepper::OutputCompareIntCb(TIM_HandleTypeDef* htim)
{
  uint32_t comp = __HAL_TIM_GET_COMPARE(&htim1, TIM_CHANNEL_1);
__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, comp + 100);
}
