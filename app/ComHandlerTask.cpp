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
#include "crc.h"

ComHandlerTask* ComHandlerTask::mspThis = 0;

ComHandlerTask::ComHandlerTask(TaskId id, const char* name):
 Task(id, name),
 mTxBuffer(COMHANDLER_UART_TXBUF_SIZE),
 mRxBuffer(COMHANDLER_UART_RXBUF_SIZE)
{
  mpUpdateTimer = new Timer(ComHandlerTaskId, TimerComHandlerUpdate);
  mpUpdateTimer->setInterval(10);
  mpUpdateTimer->setSingleShot(false);
  mpUpdateTimer->stop();

  mpHuart = COMHANDLER_UART_HANDLE;
  mLastMessageId = 0;
  mTxMessageId = 1;
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
      mpUpdateTimer->start();
      __HAL_UART_ENABLE_IT(mpHuart, UART_IT_RXFT);    //Receive FIFO threshold reached interrupt
      break;
    }
    case MSG_ID_WERNI_MESSAGE:
    {
      message_t lMessage = *(message_t*)(message->mem()->memory);
      SendCommand((COMMAND)lMessage.cmd, &lMessage.dataUnion);
      break;
    }
    case MSG_ID_TIMEOUT:
    {
      switch(message->data().longword)
      {
        case TimerComHandlerUpdate:
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
    if(lChecksum == lReceivedMessage.checksum)  //If checksum matches
    {
      if(lReceivedMessage.id != mLastMessageId) //Check message is not a duplicate
      {
        SendCommand(CMD_ACKNOWLEDGE);
        mLastMessageId = lReceivedMessage.id;

        //Create memory message and send to Werni Task
        Message* lpMsg = Message::reserve(MSG_ID_WERNI_MESSAGE, WerniTaskId, sizeof(lReceivedMessage));
        memcpy(lpMsg->mem()->memory, &lReceivedMessage, sizeof(lReceivedMessage));
        lpMsg->sendMsg();
      }
      else
      {
        SendCommand(CMD_NOT_ACKNOWLEDGE);
      }
    }
    else
    {
      SendCommand(CMD_CRC_ERROR);
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

uint8_t ComHandlerTask::CalculateChecksum(const void* pData, uint32_t size)
{
  return HAL_CRC_Calculate(&hcrc, (uint32_t*)pData, size);
}

void ComHandlerTask::TransmitPendingData(void)
{
  while((__HAL_UART_GET_FLAG(mpHuart,UART_FLAG_TXFNF) == true) && (mTxBuffer.BytesAvailable() > 0))
  {
    mpHuart->Instance->TDR = mTxBuffer.ReadByte();    //Write byte into uart tx fifo
  }
}

void ComHandlerTask::SendCommand(COMMAND cmd, data_union_t* pData)
{
  message_t lMessage;

  lMessage.cmd = cmd;
  lMessage.id = mTxMessageId++;

  //Messages can have empty data fields, then just the command is relevant
  if(pData != NULL)
  {
    lMessage.dataUnion = *pData;
  }
  else
  {
    memset((void*)&lMessage.dataUnion, 0U,  sizeof(data_union_t));
  }
  lMessage.checksum = CalculateChecksum((void*)&lMessage, sizeof(lMessage) - sizeof(lMessage.checksum));

  AddMessageToTxBuffer(&lMessage);
  TransmitPendingData();
}

void ComHandlerTask::AddMessageToTxBuffer(message_t* pMsg)
{
  //TODO:Fix this, its ugly! Check if buffer has space first! Add memcpy interface to buffer!

  //Preamble
  mTxBuffer.WriteByte('A');
  mTxBuffer.WriteByte('A');
  mTxBuffer.WriteByte('A');
  mTxBuffer.WriteByte('B');

  for(uint32_t i=0; i<sizeof(message_t); i++)
  {
    mTxBuffer.WriteByte(((unsigned char*)pMsg)[i]);
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

void ComHandlerTask::UartRxRtoCallback(UART_HandleTypeDef* huart)
{

}
