/*
 * LightBarrier.h
 *
 *  Created on: Apr 21, 2024
 *      Author: krumm
 */

#ifndef LIGHTBARRIER_H_
#define LIGHTBARRIER_H_

#include "paradef.h"
#include "InterruptRouting.h"

class LightBarrier : public Iinterruptable
{
public:
  LightBarrier();
  virtual ~LightBarrier();

  bool IsObstructed(void);

private:
  void Enable(bool enable);
  void TimPeriodElapsedCb(TIM_HandleTypeDef *htim);

  bool mEnabled;
  uint32_t mAdcConversionResults[2];


};

#endif /* LIGHTBARRIER_H_ */
