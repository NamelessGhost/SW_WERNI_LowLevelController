/*
 * EnergyMeasurement.cpp
 *
 *  Created on: Mar 25, 2024
 *      Author: krumm
 */

#include <EnergyMeasurement.h>
#include "main.h"

EnergyMeasurement::EnergyMeasurement()
{
  // TODO Auto-generated constructor stub
  mActiveAdcChannel = ADC_CHANNEL1;

  mAdcChannelConfigs[ADC_CHANNEL1].Channel = ADC_CHANNEL_1;
  mAdcChannelConfigs[ADC_CHANNEL1].Rank = ADC_REGULAR_RANK_1;
  mAdcChannelConfigs[ADC_CHANNEL1].SamplingTime = ADC_SAMPLETIME_92CYCLES_5;
  mAdcChannelConfigs[ADC_CHANNEL1].SingleDiff = ADC_SINGLE_ENDED;

  mAdcChannelConfigs[ADC_CHANNEL2].Channel = ADC_CHANNEL_2;
  mAdcChannelConfigs[ADC_CHANNEL2].Rank = ADC_REGULAR_RANK_1;
  mAdcChannelConfigs[ADC_CHANNEL2].SamplingTime = ADC_SAMPLETIME_92CYCLES_5;
  mAdcChannelConfigs[ADC_CHANNEL2].SingleDiff = ADC_SINGLE_ENDED;

  mAdcChannelConfigs[ADC_CHANNEL14].Channel = ADC_CHANNEL_14;
  mAdcChannelConfigs[ADC_CHANNEL14].Rank = ADC_REGULAR_RANK_1;
  mAdcChannelConfigs[ADC_CHANNEL14].SamplingTime = ADC_SAMPLETIME_92CYCLES_5;
  mAdcChannelConfigs[ADC_CHANNEL14].SingleDiff = ADC_SINGLE_ENDED;

  HAL_TIM_Base_Start_IT(ENERGY_MEASUREMENT_TIMER_HANDLE);
  HAL_ADC_Start_IT(&hadc1);
}

EnergyMeasurement::~EnergyMeasurement()
{
  // TODO Auto-generated destructor stub
}

void EnergyMeasurement::AdcConvCompleteCb(ADC_HandleTypeDef *hadc)
{
  mAdcConversionResults[mActiveAdcChannel++] = HAL_ADC_GetValue(hadc);
  if(mActiveAdcChannel >= ADC_CHANNELS_SIZE)
  {
    mActiveAdcChannel = ADC_CHANNEL1;
  }

}

void EnergyMeasurement::TimPeriodElapsedCb(TIM_HandleTypeDef *htim)
{
  HAL_ADC_ConfigChannel(ENERGY_MEASUREMENT_ADC_HANDLE, &mAdcChannelConfigs[mActiveAdcChannel]);
  HAL_ADC_Start_IT(ENERGY_MEASUREMENT_ADC_HANDLE);
}
