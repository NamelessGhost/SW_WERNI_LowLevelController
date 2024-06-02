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
#include "RingBuffer.h"
#include "ProtocolDefWERNI.h"
#include "paradef.h"

#include "Stepper.h"


class ComHandlerTask: public Task, public Iinterruptable
{
public:
  ComHandlerTask(TaskId id, const char* name);
  static ComHandlerTask* instance(void);

protected:
  virtual void handleMessage(Message* message);

private:
  void ProcessReceivedData(void);
  bool FindPreamble(void);
  uint8_t CalculateChecksum(const void* pData, uint32_t size);
  void TransmitPendingData(void);
  void SendCommand(COMMAND cmd, data_union_t* pData = NULL);
  void AddMessageToTxBuffer(message_t* pMsg);

  void UartRxDataAvailableCb(UART_HandleTypeDef* huart) override;
  void UartRxRtoCallback(UART_HandleTypeDef* huart) override;
  void UartRxFifoGetData(void);

  static ComHandlerTask* mspThis;
  Timer* mpUpdateTimer;
  UART_HandleTypeDef* mpHuart;

  RingBuffer mTxBuffer;
  RingBuffer mRxBuffer;

  uint8_t mLastMessageId;
  uint8_t mTxMessageId;
};

#endif /* COMHANDLERTASK_H_ */
