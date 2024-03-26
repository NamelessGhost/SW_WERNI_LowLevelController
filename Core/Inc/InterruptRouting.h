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
#include "adc.h"

/*This class creates an interface for routing interrupts to member functions*/
class Iinterruptable
{
public:
  virtual void OutputCompareIntCb(TIM_HandleTypeDef* htim);
  virtual void UartTxCompleteCb(UART_HandleTypeDef* huart);
  virtual void UartRxDataAvailableCb(UART_HandleTypeDef* huart);
  virtual void UartRxRtoCallback(UART_HandleTypeDef* huart);
  virtual void AdcConvCompleteCb(ADC_HandleTypeDef *hadc);
  virtual void TimPeriodElapsedCb(TIM_HandleTypeDef *htim);

  static std::vector<Iinterruptable*> sInterruptSubscriberVector;

protected:
  Iinterruptable();
  virtual ~Iinterruptable();

private:

};

//************ C-Function Prototypes below ***************

#ifdef __cplusplus    //Use C-Style linkage
extern "C" {
#endif

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef* htim);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart);
void UART_RxFtCallback(UART_HandleTypeDef* huart);
void UART_RxRtoCallback(UART_HandleTypeDef* huart);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);
void HAL_TIM_PeriodElapsedCallback_FWD(TIM_HandleTypeDef *htim);

#ifdef __cplusplus
}
#endif

#endif /* INC_INTERRUPTROUTING_H_ */
