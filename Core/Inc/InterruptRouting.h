/*
 * Filename: InterruptRouting.h
 * Author: André Krummenacher
 * Date: 12.11.2023
 */


#ifndef INC_INTERRUPTROUTING_H_
#define INC_INTERRUPTROUTING_H_

#include <vector>
#include <queue>
#include "tim.h"
#include "usart.h"

/*This class creates an interface for routing interrupts to member functions*/
class Iinterruptable
{
public:
  Iinterruptable();
  virtual ~Iinterruptable();
  virtual void OutputCompareIntCb(TIM_HandleTypeDef* htim);
  virtual void UartTxCompleteCb(UART_HandleTypeDef* huart);
  virtual void UartRxDataAvailableCb(UART_HandleTypeDef* huart);
  virtual void UartRxCompleteCb(UART_HandleTypeDef* huart);

  static std::vector<Iinterruptable*> outputCompareIntReceivers;
protected:


private:

};

//************ C-Function Prototypes below ***************

#ifdef __cplusplus    //Use C-Style linkage
extern "C" {
#endif

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef* htim);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart);
void UART_RxRtoCallback(UART_HandleTypeDef* huart);

#ifdef __cplusplus
}
#endif

#endif /* INC_INTERRUPTROUTING_H_ */
