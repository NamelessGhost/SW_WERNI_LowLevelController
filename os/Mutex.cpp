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
  xSemaphoreTake(mMutex, portMAX_DELAY);
  mCausedLockId = causedLockId;
}

void Mutex::unlock()
{
  xSemaphoreGive(mMutex);
  mCausedLockId = 0;
}
