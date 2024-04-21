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

  mVoltageVDD = 0;
  mCurrentVDD = 0;
  mCurrent5V = 0;
  mEnergyVDD = 0;
  mEnergy5V = 0;


  HAL_TIM_Base_Start_IT(ADC_PERIODIC_TRIGGER_TIMER_HANDLE);
}

EnergyMeasurement::~EnergyMeasurement()
{
  // TODO Auto-generated destructor stub
}

float EnergyMeasurement::GetEnergy(void)
{
  return mEnergyVDD + mEnergy5V;
}

void EnergyMeasurement::ResetEnergy(void)
{
  mEnergyVDD = 0;
  mEnergy5V = 0;
}

void EnergyMeasurement::AdcConvCompleteCb(ADC_HandleTypeDef *hadc)
{
  ProcessMeasurements();
}

void EnergyMeasurement::TimPeriodElapsedCb(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == ADC_PERIODIC_TRIGGER_TIMER_HANDLE->Instance)
  {
    HAL_ADC_Start_DMA(ENERGY_MEASUREMENT_ADC_HANDLE, mAdcConversionResults, ADC_CHANNELS_SIZE);
  }
}

inline float EnergyMeasurement::CalcAdcInputVoltage(uint32_t adcConversionResult)
{
  return adcConversionResult * (float)ENERGY_MEASUREMENT_ADC_VREF / (float)ENERGY_MEASUREMENT_ADC_RESOLUTION;
}

//Function call takes 2us with optimization disabled
void EnergyMeasurement::ProcessMeasurements(void)
{
  mVoltageVDD = CalcAdcInputVoltage(mAdcConversionResults[ADC_CHANNEL14]) / (float)ENERGY_MEASUREMENT_VDD_DIF_GAIN;
  mCurrentVDD = CalcAdcInputVoltage(mAdcConversionResults[ADC_CHANNEL1]) / ((float)ENERGY_MEASUREMENT_CURR_AMP_GAIN * (float)(ENERGY_MEASUREMENT_CURR_SHUNT_R));
  mCurrent5V  = CalcAdcInputVoltage(mAdcConversionResults[ADC_CHANNEL2]) / ((float)ENERGY_MEASUREMENT_CURR_AMP_GAIN * (float)(ENERGY_MEASUREMENT_CURR_SHUNT_R));

  mEnergyVDD += mVoltageVDD * mCurrentVDD * ENERGY_MEASUREMENT_TIMER_CNT_PERIOD;
  mEnergy5V  += 5 * mCurrent5V * ENERGY_MEASUREMENT_TIMER_CNT_PERIOD;
}
