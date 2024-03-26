/*
 * EnergyMeasurement.h
 *
 *  Created on: Mar 25, 2024
 *      Author: krumm
 */

#ifndef ENERGYMEASUREMENT_H_
#define ENERGYMEASUREMENT_H_

#include "InterruptRouting.h"
#include "paradef.h"
#include "stm32g4xx_hal.h"

class EnergyMeasurement : public Iinterruptable
{
public:
  EnergyMeasurement();
  virtual ~EnergyMeasurement();

protected:

private:
  void AdcConvCompleteCb(ADC_HandleTypeDef *hadc) override;
  void TimPeriodElapsedCb(TIM_HandleTypeDef *htim) override;

  enum AdcChannels
  {
    ADC_CHANNEL1,
    ADC_CHANNEL2,
    ADC_CHANNEL14,
    ADC_CHANNELS_SIZE
  };

  uint32_t mAdcConversionResults[ADC_CHANNELS_SIZE];
  uint32_t mActiveAdcChannel;
  ADC_ChannelConfTypeDef mAdcChannelConfigs[ADC_CHANNELS_SIZE];
};

#endif /* ENERGYMEASUREMENT_H_ */
