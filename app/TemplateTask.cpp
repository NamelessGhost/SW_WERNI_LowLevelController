/*
 * Filename: TemplateTask.cpp
 * Author: André Krummenacher
 * Date: 11.11.2023
 * Description: Template of a Task to serve as a starting point
 */

#include <string.h>
#include <stdio.h>
#include "TemplateTask.h"

TemplateTask* TemplateTask::mspThis = 0;

TemplateTask::TemplateTask(TaskId id, const char* name): Task(id, name)
{
  Message::reserveIsr(MSG_ID_TEMPLATETASK_EVENT, TemplateTaskId, 0);
}

TemplateTask* TemplateTask::instance()
{
  if(TemplateTask::mspThis == 0)
  {
    mspThis = new TemplateTask(TemplateTaskId, "TemplateTask\0");
  }
  return TemplateTask::mspThis;
}

void TemplateTask::someEvent()
{
  mspThis->mpIsrEventMsg->setValue(123);
  mspThis->mpIsrEventMsg->sendMsg();
}

void TemplateTask::handleMessage(Message* message)
{
//  switch(message->id())
//  {
//    case MSG_ID_ISR_GPIO:
//    {
//      switch(message->value())
//      {
//
//       break;
//      }
//      break;
//    }
//  }
}
