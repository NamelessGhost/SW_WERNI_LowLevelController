/*
 * Filename: TemplateTask.h
 * Author: André Krummenacher
 * Date: 11.11.2023
 */

#ifndef TEMPLATETASK_H_
#define TEMPLATETASK_H_

#include "TaskClass.h"


class TemplateTask: public Task
{
public:
  TemplateTask(TaskId id, const char* name);
  static TemplateTask* instance();
  static void someEvent();

protected:
  virtual void handleMessage(Message* message);

private:
  static TemplateTask* mspThis;
  Message* mpIsrEventMsg;

};

#endif /* TEMPLATETASK_H_ */
