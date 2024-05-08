/*
 * TaskList.h
 *
 *  Created on: Apr 17, 2019
 *      Author: axel
 */

#ifndef TASKLIST_H_
#define TASKLIST_H_

#define TASK_NAME_LEN  20
#define TASK_MAX_COUNT 20
#define TIMER_MAX_COUNT 20

#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <timers.h>
#include "Mutex.h"

enum TaskId
{
  NoTask,
  TemplateTaskId,
  ComHandlerTaskId,
  WerniTaskId,
  IoTaskId,
};

class TaskList
{
public:
  static TaskList* instance();
  static void createEntry(TaskId id, char* name, TaskHandle_t task, QueueHandle_t address);
  static TaskHandle_t getTaskHandle(TaskId id);
  static TaskId getTaskId(TaskHandle_t handle);
  static QueueHandle_t getQueue(TaskId id);
  static Mutex* getMutex();

private:
  TaskList();
  struct table_struct
  {
    TaskId id;
    char name[TASK_NAME_LEN+1];
    TaskHandle_t hTask;
    QueueHandle_t hQueue;
  };
  static table_struct mTaskTable[TASK_MAX_COUNT];

  Mutex mMutex;
  static TaskList* mspThis;
};

#endif /* TASKLIST_H_ */
