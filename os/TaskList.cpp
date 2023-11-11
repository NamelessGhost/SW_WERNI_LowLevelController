/*
 * TaskList.cpp
 *
 *  Created on: Apr 17, 2019
 *      Author: axel
 */

#include "os_init.h"
#include "TaskList.h"
#include <string.h>

void TaskList_init()
{
  TaskList::instance();
}

TaskList::table_struct TaskList::mTaskTable[TASK_MAX_COUNT];

TaskList* TaskList::mspThis=0;

TaskList* TaskList::instance()
{
  if(TaskList::mspThis == 0)
  {
    TaskList::mspThis = new TaskList;
  }
  return TaskList::mspThis;
}

TaskList::TaskList()
{
  memset(&mTaskTable, 0, sizeof(mTaskTable));
}

void TaskList::createEntry(TaskId id, char* name, TaskHandle_t task, QueueHandle_t address)
{
  TaskList::getMutex()->lock();
  for(int i = 0; i < TASK_MAX_COUNT; ++i)
  {
    if(TaskList::mTaskTable[i].id == NoTask)
    {
      TaskList::mTaskTable[i].id = id;
      strncpy(TaskList::mTaskTable[i].name, name, TASK_NAME_LEN);
      TaskList::mTaskTable[i].hTask = task;
      TaskList::mTaskTable[i].hQueue = address;
      break;
    }
  }
  TaskList::getMutex()->unlock();
}

Mutex* TaskList::getMutex()
{
  return &TaskList::mspThis->mMutex;
}

TaskHandle_t TaskList::getTaskHandle(TaskId id)
{
  TaskHandle_t lRetVal = 0;
  for(int i = 0; i < TASK_MAX_COUNT; ++i)
  {
    if(id == TaskList::mTaskTable[i].id)
    {
      lRetVal = TaskList::mTaskTable[i].hTask; //atomic
      break;
    }
  }
  return lRetVal;
}

TaskId TaskList::getTaskId(TaskHandle_t handle)
{
  TaskId lRetVal = NoTask;
  for(int i = 0; i < TASK_MAX_COUNT; ++i)
  {
    if(handle == TaskList::mTaskTable[i].hTask)
    {
      lRetVal = TaskList::mTaskTable[i].id; //atomic
      break;
    }
  }
  return lRetVal;
}

QueueHandle_t TaskList::getQueue(TaskId id)
{
  QueueHandle_t lRetVal = 0;
  for(int i = 0; i < TASK_MAX_COUNT; ++i)
  {
    if(id == TaskList::mTaskTable[i].id)
    {
      lRetVal = TaskList::mTaskTable[i].hQueue; //atomic
      break;
    }
  }
  return lRetVal;
}

