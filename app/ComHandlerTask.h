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
uint8_t checksum;
}message_header_t;

typedef struct{
  message_header_t header;
  uint8_t* pData;
  uint16_t checksum;
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
  uint8_t CalculateChecksum(void);
  void TransmitPendingData(void);
  void UartRxDataAvailableCb(UART_HandleTypeDef* huart);
  void UartRxCompleteCb(UART_HandleTypeDef* huart);
  void ProcessReceivedData(void);
  void FindPreamble(void);
  void UartRxFifoGetData(void);

  static ComHandlerTask* mspThis;
  Timer* mpUpdateTimer;
  Message* mpIsrEventMsg;
  UART_HandleTypeDef* mpHuart;

  std::queue<uint8_t> mTxData;
  RingBuffer mRxBuffer;

  bool mPreambleDetected;


};

#endif /* COMHANDLERTASK_H_ */
