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
#include "InterruptRouting.h"

ComHandlerTask* ComHandlerTask::mspThis = 0;

ComHandlerTask::ComHandlerTask(TaskId id, const char* name):
 Task(id, name),
 mRxBuffer(COMHANDLER_UART_RXBUF_SIZE),
 mTxBuffer(COMHANDLER_UART_TXBUF_SIZE)
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
          ProcessReceivedData();
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
  message_t lReceivedMessage;
  if(mRxBuffer.BytesAvailable() >= sizeof(message_t) + COMHANDLER_UART_PREAMBLE_LEN)
  if(FindPreamble())
  {
    mRxBuffer.ReadBytes(sizeof(lReceivedMessage), (void*)&lReceivedMessage);

    uint8_t lChecksum = CalculateChecksum(&lReceivedMessage, sizeof(lReceivedMessage) - sizeof(lReceivedMessage.checksum));
    if(lChecksum == lReceivedMessage.checksum)
    {
      //Create memory message and send to Werni Task
      Message* lpMsg = new Message();
      lpMsg->reserve(MSG_ID_WERNI_MESSAGE, WerniTaskId, sizeof(lReceivedMessage));
      memcpy(lpMsg->mem(), &lReceivedMessage, sizeof(lReceivedMessage));
      lpMsg->sendMsg();
    }
  }
}

//Returns true if a preamble was found, preamble is removed in the process
bool ComHandlerTask::FindPreamble(void)
{
  char lPreamble[COMHANDLER_UART_PREAMBLE_LEN + 1] = {0};
  mRxBuffer.ReadBytes(COMHANDLER_UART_PREAMBLE_LEN, (void*)&lPreamble);
  if(strcmp(COMHANDLER_UART_PREAMBLE, lPreamble) == 0)
    return true;

  return false;
}

uint8_t ComHandlerTask::CalculateChecksum(const void* pData, size_t size)
{
  //TODO:Implement proper checksum
  return 12;
}

void ComHandlerTask::TransmitPendingData(void)
{
  uint8_t txBuf;

  while((__HAL_UART_GET_FLAG(mpHuart,UART_FLAG_TXFNF) == true) && (mTxBuffer.BytesAvailable() > 0))
  {
    txBuf = mTxBuffer.ReadByte();
    HAL_UART_Transmit(mpHuart, &txBuf, 1U, 0U);
  }
}

void ComHandlerTask::UartRxFifoGetData(void)
{
  while(__HAL_UART_GET_FLAG(mpHuart,UART_FLAG_RXFNE) == true)   //Check if data in receive fifo
  {
    mRxBuffer.WriteByte(mpHuart->Instance->RDR);       //Read data from receive register
  }
}

void ComHandlerTask::UartRxDataAvailableCb(UART_HandleTypeDef* huart)
{
  UartRxFifoGetData();
}

void ComHandlerTask::UartRxCompleteCb(UART_HandleTypeDef* huart)
{
  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
}
