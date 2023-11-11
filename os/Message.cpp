/*
 * Message.cpp
 *
 *  Created on: Apr 17, 2019
 *      Author: axel
 */

#include "Message.h"
#include "MsgPool.h"
#include "TaskList.h"
#include "FreeRTOS.h"
#include <task.h>
#include <stdlib.h>
#include <stdio.h>


MsgPool* Message::mspPool = 0;

Message::Message()
{

}

void Message::init(MsgPool* pPool)
{
  Message::mspPool = pPool;
}

Message* Message::reserve(uint8_t id, uint8_t destination)
{
  Message* pMsg = Message::mspPool->resMsg();
  pMsg->mMsg.NoneMsg.Id = id;
  pMsg->mMsg.NoneMsg.Type = MsgNone;
  pMsg->mMsg.NoneMsg.DestAddr = destination;
  pMsg->mMsg.NoneMsg.SrcAddr = TaskList::getTaskId(xTaskGetCurrentTaskHandle());
  configASSERT(pMsg);
  return pMsg;
}

Message* Message::reserve(uint8_t id, uint8_t destination, size_t shared_memory_size)
{
  Message* pMsg = Message::mspPool->resMsg();
  pMsg->mMsg.MemMsg.Id = id;
  pMsg->mMsg.MemMsg.Type = MsgMem;
  pMsg->mMsg.MemMsg.DestAddr = destination;
  pMsg->mMsg.MemMsg.SrcAddr = TaskList::getTaskId(xTaskGetCurrentTaskHandle());
  SharedMem* lpSharedMem = new SharedMem;
  lpSharedMem->memory = pvPortMalloc(shared_memory_size);
  lpSharedMem->length = shared_memory_size;
  pMsg->mMsg.MemMsg.Memory = lpSharedMem;
  configASSERT(pMsg && lpSharedMem);
  return pMsg;
}

Message* Message::reserve(uint8_t id, uint8_t destination, DataUnion data)
{
  Message* pMsg = Message::mspPool->resMsg();
  pMsg->mMsg.DataMsg.Id = id;
  pMsg->mMsg.DataMsg.Type = MsgData;
  pMsg->mMsg.DataMsg.DestAddr = destination;
  pMsg->mMsg.DataMsg.SrcAddr = TaskList::getTaskId(xTaskGetCurrentTaskHandle());
  pMsg->mMsg.DataMsg.Data = data;
  configASSERT(pMsg);
  return pMsg;
}

Message* Message::reserveIsr(uint8_t id, uint8_t destination, uint32_t isr_value)
{
  Message* pMsg = Message::mspPool->resMsg();
  pMsg->mMsg.IsrMsg.Id = id;
  pMsg->mMsg.IsrMsg.Type = MsgIsr;
  pMsg->mMsg.IsrMsg.DestAddr = destination;
  pMsg->mMsg.IsrMsg.Value = isr_value;
  configASSERT(pMsg);
  return pMsg;
}

void Message::sendMsg()
{
  QueueHandle_t lDestQueue = TaskList::getQueue((TaskId)mMsg.NoneMsg.DestAddr);
  Message* lpMessage = this;
  if(lDestQueue)
  {
    BaseType_t lRes;
    if(lpMessage->type() == MsgIsr)
    {
      BaseType_t xHigherPriorityTaskWoken = pdFALSE;
      lRes = xQueueSendFromISR(lDestQueue, &lpMessage, &xHigherPriorityTaskWoken);
    }
    else
    {
      lRes = xQueueSend(lDestQueue, &lpMessage, (TickType_t) 0);
    }
    configASSERT(lRes == 1);
  }
  else
  {
    freeMsg();
  }
}


void Message::freeMsg()
{
  switch(type())
  {
    case MsgMem:
    {
      if(mMsg.MemMsg.Memory)
      {
        vPortFree(mMsg.MemMsg.Memory->memory);
        delete mMsg.MemMsg.Memory;
        mMsg.MemMsg.Memory = 0;
      }
      break;
    }
    case MsgData:
    {
      mMsg.DataMsg.Data.longword = 0;
      break;
    }
    default:
      break;
  }

  Message::mspPool->freeMsg(this);
}

uint8_t Message::id()
{
  return mMsg.NoneMsg.Id;
}

uint8_t Message::src()
{
  return mMsg.NoneMsg.SrcAddr;
}

uint8_t Message::dst()
{
  return mMsg.NoneMsg.DestAddr;
}

uint8_t Message::type()
{
  return mMsg.NoneMsg.Type;
}

DataUnion Message::data()
{
  DataUnion lRetval;
  lRetval.longword = (uint32_t)(-1);
  if(mMsg.DataMsg.Type == MsgData)
  {
    lRetval = mMsg.DataMsg.Data;
  }
  return lRetval;
}

SharedMem* Message::mem()
{
  SharedMem* lRetval = 0;
  if(mMsg.MemMsg.Type == MsgMem)
  {
    lRetval = mMsg.MemMsg.Memory;
  }
  return lRetval;
}

uint32_t Message::value()
{
  return mMsg.IsrMsg.Value;
}

void Message::setValue(uint32_t value)
{
  mMsg.IsrMsg.Value = value;
}
