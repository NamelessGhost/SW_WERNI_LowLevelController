/*
 * Filename: TemplateTask.h
 * Author: André Krummenacher
 * Date: 12.11.2023
 */

#ifndef COMHANDLERTASK_H_
#define COMHANDLERTASK_H_

#include "TaskClass.h"
#include "Timer.h"

#include "Stepper.h"


class ComHandlerTask: public Task
{
public:
  ComHandlerTask(TaskId id, const char* name);
  static ComHandlerTask* instance();
  static void someEvent();

protected:
  virtual void handleMessage(Message* message);

private:
  static ComHandlerTask* mspThis;
  Timer* mpTimerLed;
  Message* mpIsrEventMsg;

  Stepper* mpStepper;
};

#endif /* COMHANDLERTASK_H_ */
