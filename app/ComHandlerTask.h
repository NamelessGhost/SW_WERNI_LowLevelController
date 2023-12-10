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
#include "paradef.h"

#include "Stepper.h"

#pragma pack(1) // Set alignment to 1 byte

typedef struct{
  uint8_t cmd;
  uint8_t len;
  uint8_t data[16];
  uint8_t checksum;
}message_t;

#pragma pack() // Reset alignment to default

class ComHandlerTask: public Task, public Iinterruptable
{
public:
  ComHandlerTask(TaskId id, const char* name);
  static ComHandlerTask* instance(void);

protected:
  virtual void handleMessage(Message* message);

private:
  bool FindPreamble(void);
  uint8_t CalculateChecksum(const void* pData, size_t size);
  void DeliverMessage(message_t message);
  void TransmitPendingData(void);
  void UartRxDataAvailableCb(UART_HandleTypeDef* huart);
  void UartRxCompleteCb(UART_HandleTypeDef* huart);
  void ProcessReceivedData(void);
  void UartRxFifoGetData(void);

  static ComHandlerTask* mspThis;
  Timer* mpUpdateTimer;
  Message* mpIsrEventMsg;
  UART_HandleTypeDef* mpHuart;

  RingBuffer mTxBuffer;
  RingBuffer mRxBuffer;
};

#endif /* COMHANDLERTASK_H_ */
