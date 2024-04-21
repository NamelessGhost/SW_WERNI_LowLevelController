/*
 * LightBarrier.cpp
 *
 *  Created on: Apr 21, 2024
 *      Author: krumm
 */

#include <LightBarrier.h>
#include "stm32g4xx_hal.h"

#include "tim.h"
#include "adc.h"


LightBarrier::LightBarrier() : Iinterruptable()
{
  mEnabled = true;
  Enable(true);
}

LightBarrier::~LightBarrier()
{
  mEnabled = false;
  Enable(false);
}

void LightBarrier::Enable(bool enable)
{
  mEnabled = enable;
  if(mEnabled)
  {
    HAL_TIM_PWM_Start(LIGHT_BARRIER_PWM_TIMER_HANDLE, LIGHT_BARRIER_PWM_TIMER_CHANNEL);
  }
  else
  {
    HAL_TIM_PWM_Stop(LIGHT_BARRIER_PWM_TIMER_HANDLE, LIGHT_BARRIER_PWM_TIMER_CHANNEL);
  }
}

void LightBarrier::TimPeriodElapsedCb(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == ADC_PERIODIC_TRIGGER_TIMER_HANDLE->Instance)
  {
    HAL_ADC_Start_DMA(LIGHT_BARRIER_INPUTS_ADC_HANDLE, mAdcConversionResults, 2);
  }
}

bool LightBarrier::IsObstructed(void)
{
  if((mAdcConversionResults[0] >= LIGHT_BARRIER_CLOSED_THRESHOLD) &&
     (mAdcConversionResults[1] >= LIGHT_BARRIER_CLOSED_THRESHOLD) &&
      mEnabled)
  {
    return true;
  }
  return false;
}


