/*
 * Timer.h
 *
 *  Created on: Apr 18, 2019
 *      Author: axel
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <FreeRTOS.h>
#include <timers.h>

#include "TaskList.h"

enum TimerId
{
  TimerInvalid,
  TimerCanPeriodic,
  TimerBtnUp,
  TimerBtnDown,
  TimerCanOpenHeartbeat,
  TimerWallboxPeriodic
};

class Timer
{
public:
  Timer(TaskId addressee, TimerId timer);
  virtual ~Timer();

  void setInterval(int period);
  void setSingleShot(bool once);
  void start();
  void stop();

private:
  bool isInterrupt();

  struct timerDataStruct
  {
    TaskId addressee;
    TimerId timer;
  }mTimerData;
  static void CallbackFunction( TimerHandle_t xTimer );


  TimerHandle_t mHTimer;
};

#endif /* TIMER_H_ */
