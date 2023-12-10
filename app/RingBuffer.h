/*
 * RingBuffer.h
 *
 *  Created on: Dec 9, 2023
 *      Author: krumm
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include "Mutex.h"

class RingBuffer
{
public:
  RingBuffer(unsigned int size);
  virtual ~RingBuffer();
  void WriteByte(unsigned char data);
  unsigned char ReadByte();
  bool ReadBytes(size_t size, const void* pData);
  unsigned int BytesAvailable(void);
  unsigned int Size();
  bool IsEmpty();
  void* mpBuffer;
private:
  Mutex mMutex;
  bool mEmpty;
  bool mFull;
  unsigned int mSize;
  unsigned int mReadIndex;
  unsigned int mWriteIndex;
};

#endif /* RINGBUFFER_H_ */
