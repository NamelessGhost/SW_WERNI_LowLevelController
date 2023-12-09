/*
 * Mutex.cpp
 *
 *  Created on: Apr 17, 2019
 *      Author: axel
 */

#include <Mutex.h>

Mutex::Mutex()
{
  mMutex = xSemaphoreCreateMutex();
}

Mutex::~Mutex()
{
  vSemaphoreDelete(mMutex);
}

void Mutex::lock(uint8_t causedLockId)
{
  if(xPortIsInsideInterrupt() == pdFALSE)
  {
    xSemaphoreTake(mMutex, portMAX_DELAY);
    mCausedLockId = causedLockId;
  }
}

void Mutex::unlock()
{
  if(xPortIsInsideInterrupt() == pdFALSE)
  {
    xSemaphoreGive(mMutex);
    mCausedLockId = 0;
  }
}
