/*
 * RingBuffer.cpp
 *
 *  Created on: Dec 9, 2023
 *      Author: krumm
 */

#include <RingBuffer.h>
#include "FreeRTOS.h"
#include "stm32g4xx_hal.h"    //Used for assert function

//Convention: Write index points to next empty slot

RingBuffer::RingBuffer(unsigned int size)
{
  mSize = size;
  mpBuffer = pvPortMalloc(mSize);
  assert_param(mpBuffer != nullptr);
  mEmpty = true;
  mFull = false;
  mReadIndex = 0;
  mWriteIndex = 0;

}

RingBuffer::~RingBuffer()
{
  vPortFree(mpBuffer);
}

void RingBuffer::WriteByte(unsigned char data)
{
  mMutex.lock();

  if((mWriteIndex != mReadIndex) || (mEmpty))
  {
    *((unsigned char*)mpBuffer + mWriteIndex) = data;
    mWriteIndex++;
    mWriteIndex = mWriteIndex % mSize;    //Limit index to max buffer size
    if(mWriteIndex == mReadIndex)
      mFull = true;
    mEmpty = false;
  }

  mMutex.unlock();
}

unsigned char RingBuffer::ReadByte()
{
  mMutex.lock();

  unsigned char lRet = 0;
  if((mWriteIndex != mReadIndex) || mFull)
  {
    lRet = *((unsigned char*)mpBuffer + mReadIndex);
    mReadIndex++;
    mReadIndex = mReadIndex % mSize;    //Limit index to max buffer size
    if(mWriteIndex == mReadIndex)
      mEmpty = true;
    mFull = false;
  }

  mMutex.unlock();
  return lRet;
}

bool RingBuffer::ReadBytes(size_t size, const void* pData)
{
  if(size > mSize)
    return false;

  if(pData == nullptr)
    return false;

  for(unsigned int i = 0; i < size; i++)
  {
    *((unsigned char*)pData + i) = ReadByte();
  }
  return true;
}

unsigned int RingBuffer::BytesAvailable(void)
{
  unsigned int lRet = 0;

  if (mWriteIndex >= mReadIndex) {
    // No wrap-around, straightforward calculation
    lRet = mWriteIndex - mReadIndex;
  }
  else
  {
    // Wrap-around occurred, calculate separately
    lRet = mSize - (mReadIndex - mWriteIndex);
  }
  return lRet;
}

unsigned int RingBuffer::Size(void)
{
  return mSize;
}

bool RingBuffer::IsEmpty(void)
{
  return mEmpty;
}
