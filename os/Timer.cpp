/*
 * Timer.cpp
 *
 *  Created on: Apr 18, 2019
 *      Author: axel
 */

#include "Timer.h"
#include "Message.h"
#include <stdio.h>
#include <stm32g491xx.h>

#define BLOCK_TIME   1000

Timer::Timer(TaskId addressee, TimerId timer)
{
  mTimerData.addressee = addressee;
  mTimerData.timer = timer;
  const int cDefaultPeriod = 1000;
  mHTimer = xTimerCreate("Timer\0", pdMS_TO_TICKS(cDefaultPeriod), pdFALSE, &mTimerData, CallbackFunction);
  configASSERT(mHTimer != 0);
}

Timer::~Timer()
{
  xTimerDelete(mHTimer, BLOCK_TIME);
}

void Timer::CallbackFunction( TimerHandle_t xTimer )
{
  timerDataStruct* lpTimerData = (timerDataStruct*)pvTimerGetTimerID(xTimer);
  DataUnion lTimerId;
  lTimerId.longword = lpTimerData->timer;
  Message* lpMsg = Message::reserve(MSG_ID_TIMEOUT, lpTimerData->addressee, lTimerId);
  lpMsg->sendMsg();
}

void Timer::setInterval(int period)
{
  xTimerChangePeriod( mHTimer, pdMS_TO_TICKS(period), BLOCK_TIME);
}

void Timer::setSingleShot(bool once)
{
  vTimerSetReloadMode(mHTimer, !once);
}

void Timer::start()
{
  bool lResult = pdFAIL;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  if(isInterrupt())
  {
    lResult = xTimerStartFromISR(mHTimer, &xHigherPriorityTaskWoken);
  }
  else
  {
    lResult = xTimerStart(mHTimer, BLOCK_TIME);
  }
  if(lResult == pdFAIL)
  {
    printf("start timer failed\n");
  }
}

void Timer::stop()
{
  xTimerStop(mHTimer, BLOCK_TIME);
}

bool Timer::isInterrupt()
{
  return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0 ;
}

