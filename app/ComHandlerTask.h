/*
 * Filename: TemplateTask.h
 * Author: André Krummenacher
 * Date: 12.11.2023
 */

#ifndef COMHANDLERTASK_H_
#define COMHANDLERTASK_H_

#include "TaskClass.h"
#include "Timer.h"
#include "usart.h"
#include "InterruptRouting.h"

#include "Stepper.h"


class ComHandlerTask: public Task, public Iinterruptable
{
public:
  ComHandlerTask(TaskId id, const char* name);
  static ComHandlerTask* instance(void);

protected:
  virtual void handleMessage(Message* message);

private:
  void TransmitPendingData(void);
  void UartTxCompleteCb(UART_HandleTypeDef* huart);
  void ProcessReceivedData(void);
  void SearchPreamble(void);

  static ComHandlerTask* mspThis;
  Timer* mpUpdateTimer;
  Message* mpIsrEventMsg;
  UART_HandleTypeDef* mpHuart;

  std::queue<uint8_t> mTxData;
  std::queue<uint8_t> mRxData;
};

#endif /* COMHANDLERTASK_H_ */
