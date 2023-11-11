/*
 * MsgPool.cpp
 *
 *  Created on: Apr 17, 2019
 *      Author: axel
 */

#include "os_init.h"
#include "MsgPool.h"


void MsgPool_init()
{
  Message::init(MsgPool::instance());
}

MsgPool* MsgPool::mspThis = 0;

MsgPool::MsgPool()
{
  for(int i = 0; i < MSG_POOL_SIZE; ++i)
  {
    mPool[i].mMsg = new Message;
    mPool[i].mUsed = false;
  }

}

MsgPool::~MsgPool()
{
  for(int i = 0; i < MSG_POOL_SIZE; ++i)
  {
    delete mPool[i].mMsg;
  }
}

MsgPool* MsgPool::instance()
{
  if(mspThis == 0)
  {
    mspThis = new MsgPool;
  }
  return mspThis;
}

Message* MsgPool::resMsg()
{
  Message* lpRetVal = 0;
  mspThis->mMutex.lock();
  for (int i = 0; i < MSG_POOL_SIZE; ++i)
  {
    if(mspThis->mPool[i].mUsed == false)
    {
      mspThis->mPool[i].mUsed = true;
      lpRetVal = mspThis->mPool[i].mMsg;
      break;
    }
  }
  mspThis->mMutex.unlock();
  return lpRetVal;
}

void MsgPool::freeMsg(Message* p_msg)
{
  if(p_msg->type() != MsgIsr)
  {
    mspThis->mMutex.lock();
    for (int i = 0; i < MSG_POOL_SIZE; ++i)
    {
      if(mspThis->mPool[i].mMsg == p_msg)
      {
        mspThis->mPool[i].mUsed = false;
        break;
      }
    }
    mspThis->mMutex.unlock();
  }
}
