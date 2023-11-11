/*
 * Task.h
 *
 *  Created on: Apr 17, 2019
 *      Author: axel
 */

#ifndef TASK_H_CLASS
#define TASK_H_CLASS

#define TASK_STACK_SIZE   0x100

#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include "TaskList.h"
#include "Message.h"

class Task
{
public:
  Task(TaskId id, const char* name);
  virtual ~Task();

protected:
  virtual void handleMessage(Message* message)=0;

private:
  static void mainLoop(void* args);

  TaskId mId;
  char mName[TASK_NAME_LEN+1];
  TaskHandle_t mHandle;
  QueueHandle_t mQueue;
};

#endif /* TASK_H_CLASS */
