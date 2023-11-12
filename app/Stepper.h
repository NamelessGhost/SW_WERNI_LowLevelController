/*
 * Filename: Stepper.h
 * Author: André Krummenacher
 * Date: 12.11.2023
 */

#ifndef STEPPER_H_
#define STEPPER_H_

#include <vector>
#include "InterruptRouting.h"

#include "tim.h"

enum StepperState {
    OFF,
    HOLDING,
    ROTATING
};

class Stepper : public Iinterruptable
{
public:
  Stepper(uint32_t timerChannel);
  void StartRotation(int32_t degrees);    //Postitve -> CW  Negative -> CCW
  void OutputCompareIntCb(TIM_HandleTypeDef* htim) override;

protected:

private:
  TIM_HandleTypeDef* mpTimerHandle;
  uint32_t mTimerChannel;

  StepperState mStepperState;
  int32_t mDegreesToRotate;



};

#endif /* STEPPER_H_ */
