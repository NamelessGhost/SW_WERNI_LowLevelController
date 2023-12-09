/*
 * Filename: TemplateTask.cpp
 * Author: André Krummenacher
 * Date: 12.11.2023
 * Description: Template of a Task to serve as a starting point
 */

#include <string.h>
#include <stdio.h>
#
#include "ComHandlerTask.h"
#include "main.h"
#include "InterruptRouting.h"

ComHandlerTask* ComHandlerTask::mspThis = 0;

ComHandlerTask::ComHandlerTask(TaskId id, const char* name):
 Task(id, name),
 mRxBuffer(COMHANDLER_UART_RXBUF_SIZE)
{
  mpIsrEventMsg = Message::reserveIsr(MSG_ID_TEMPLATETASK_EVENT, ComHandlerTaskId, 0);


  mpUpdateTimer = new Timer(ComHandlerTaskId, TimerComHandlerUpdate);
  mpUpdateTimer->setInterval(10);
  mpUpdateTimer->setSingleShot(false);
  mpUpdateTimer->start();

  mpHuart = &huart1;
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
          mTxData.push('H');
          mTxData.push('$');
          mTxData.push('C');
          mTxData.push('2');
          mTxData.push('F');
          mTxData.push('F');
          mTxData.push('E');
          //ProcessReceivedData();
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
  if(mRxBuffer.BytesAvailable() >= (sizeof(message_header_t) + 3))
  {
    FindPreamble();
  }

  if(mPreambleDetected)
  {
    lReceivedMessage.header.cmd = mRxBuffer.ReadByte();
    lReceivedMessage.header.len = mRxBuffer.ReadByte();
    lReceivedMessage.header.checksum = mRxBuffer.ReadByte();

    if(CalculateChecksum() == lReceivedMessage.header.checksum)
    {
      //lReceivedMessage.pData = pvPortMalloc(lReceivedMessage.header.len);

    }
    else
    {
      mPreambleDetected = false;
    }
  }
}

uint8_t ComHandlerTask::CalculateChecksum(void)
{

}

//Returns true if a preamble was found, preamble is removed in the process
void ComHandlerTask::FindPreamble(void)
{
  while(mRxBuffer.IsEmpty() == false)
  {
    if(mRxBuffer.ReadByte() == COMHANDLER_UART_PREAMBLE)
    {
      mPreambleDetected = true;
      return;
    }
  };
}

void ComHandlerTask::TransmitPendingData(void)
{
  uint8_t txBuf;

  while(mTxData.size() > 0)   //Check if data in transmit buffer
  {
    txBuf = mTxData.front();
    mTxData.pop();
    if(__HAL_UART_GET_FLAG(mpHuart,UART_FLAG_TXFNF) == true)
    {
      HAL_UART_Transmit(mpHuart, &txBuf, 1U, 0U);
    }
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
