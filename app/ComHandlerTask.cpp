/*
 * Filename: TemplateTask.cpp
 * Author: André Krummenacher
 * Date: 12.11.2023
 * Description: Template of a Task to serve as a starting point
 */

#include <string.h>
#include <stdio.h>
#include "ComHandlerTask.h"
#include "main.h"
#include "paradef.h"
#include "InterruptRouting.h"

ComHandlerTask* ComHandlerTask::mspThis = 0;

ComHandlerTask::ComHandlerTask(TaskId id, const char* name)
:Task(id, name)
{
  mpIsrEventMsg = Message::reserveIsr(MSG_ID_TEMPLATETASK_EVENT, ComHandlerTaskId, 0);
  mpUpdateTimer = new Timer(ComHandlerTaskId, TimerComHandlerUpdate);
  mpUpdateTimer->setInterval(10);
  mpUpdateTimer->setSingleShot(false);
  mpUpdateTimer->start();

  mpHuart = &hlpuart1;
}

ComHandlerTask* ComHandlerTask::instance(void)
{
  if(ComHandlerTask::mspThis == 0)
  {
    mspThis = new ComHandlerTask(ComHandlerTaskId, "ComHandlerTask\0");
  }
  return ComHandlerTask::mspThis;
}

void ComHandlerTask::handleMessage(Message* message)
{
  switch(message->id())
  {
    case MSG_ID_START:
    {

      break;
    }
    case MSG_ID_TIMEOUT:
    {
      switch(message->data().longword)
      {
        case TimerComHandlerUpdate:
          //HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
          TransmitPendingData();

          break;

        default:
          break;
      }
      break;
    }
    default:
      break;
  }
}

void ComHandlerTask::ProcessReceivedData(void)
{

}

void ComHandlerTask::SearchPreamble(void)
{

}

void ComHandlerTask::TransmitPendingData(void)
{
  uint8_t txBuf;

  while(mTxData.size() > 0)   //Check if data in transmit buffer
  {
    txBuf = mTxData.pop();
    if(__HAL_UART_GET_FLAG(mpHuart,UART_FLAG_TXFNF) == true)
    {
      HAL_UART_Transmit_IT(mpHuart, &txBuf, 1U);
    }
  }
}

void ComHandlerTask::UartTxCompleteCb(UART_HandleTypeDef* huart)
{
  uint8_t rxBuf;

  while(__HAL_UART_GET_FLAG(mpHuart,UART_FLAG_RXFNE) == true)   //Check if data in receive fifo
  {
    HAL_UART_Receive_IT(mpHuart, &rxBuf, 1U);
    mRxData.push(rxBuf);
  }
}
