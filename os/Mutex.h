/*
 * Mutex.h
 *
 *  Created on: Apr 17, 2019
 *      Author: axel
 */

#ifndef MUTEX_H_
#define MUTEX_H_

#include "FreeRTOS.h"
#include "semphr.h"

class Mutex
{
public:
  Mutex();
  virtual ~Mutex();
  void lock(uint8_t causedLock = 1);
  void unlock();

private:
  SemaphoreHandle_t mMutex;
  uint8_t mCausedLockId;
};

#endif /* MUTEX_H_ */
