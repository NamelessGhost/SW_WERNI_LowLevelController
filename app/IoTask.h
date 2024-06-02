/*
 * Filename: IoTask.h
 * Author: André Krummenacher
 * Date: 08.05.2024
 */

#ifndef IOTASK_H
#define IOTASK_H

#include "TaskClass.h"
#include "Timer.h"
#include "paradef.h"
#include "ProtocolDefWERNI.h"
#include "Button.h"


class IoTask: public Task
{
public:
  IoTask(TaskId id, const char* name);
  static IoTask* instance();

  static void EnableBuzzer(bool enable);

protected:
  virtual void handleMessage(Message* message);

private:
  void SendIoState(void);

  static IoTask* mspThis;
  Timer mIoUpdateTimer;
  Timer mIoStatusUpdateTimer;
  Timer mLedTimer;

  Button mBtnEStop;
  Button mBtnStart;
};

#endif /* IOTASK_H */
