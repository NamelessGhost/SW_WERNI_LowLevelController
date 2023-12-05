/*
 * Filename: InterruptRouting.h
 * Author: André Krummenacher
 * Date: 12.11.2023
 */


#ifndef INC_INTERRUPTROUTING_H_
#define INC_INTERRUPTROUTING_H_

#include <vector>
#include "tim.h"

/*This class creates an interface for routing interrupts to member functions*/
class Iinterruptable
{
public:
  Iinterruptable();
  virtual ~Iinterruptable();
  virtual void OutputCompareIntCb(TIM_HandleTypeDef* htim);
  static std::vector<Iinterruptable*> outputCompareIntReceivers;
protected:


private:

};

//************ C-Function Prototypes below ***************

#ifdef __cplusplus    //Use C-Style linkage
extern "C" {
#endif

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef* htim);

#ifdef __cplusplus
}
#endif

#endif /* INC_INTERRUPTROUTING_H_ */
