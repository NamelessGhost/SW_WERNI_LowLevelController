/*
 * Task.cpp
 *
 *  Created on: Apr 17, 2019
 *      Author: axel
 */

#include <stdio.h>
#include <string.h>
#include "TaskClass.h"



Task::Task(TaskId id, const char* name)
{
  mId = id;
  strncpy(mName,name, TASK_NAME_LEN);
  mQueue = xQueueCreate(MSG_QUEUE_SIZE, sizeof(Message*));
  xTaskCreate(Task::mainLoop, mName,  TASK_STACK_SIZE , this, tskIDLE_PRIORITY, &mHandle);
  configASSERT(mHandle != 0);
  TaskList::createEntry(mId, mName, mHandle, mQueue);
}

Task::~Task()
{
  // TODO Auto-generated destructor stub
}

void Task::mainLoop(void* args)
{
  Task* lpThis = (Task*)args;
  Message* lpReceiveMsg = 0;
  while(1)
  {
    BaseType_t lResult = xQueueReceive(lpThis->mQueue, &lpReceiveMsg, portMAX_DELAY);
    if(lResult == true)
    {
      //printf("queue message received %d\n",lpThis->mId);
      lpThis->handleMessage(lpReceiveMsg);
      lpReceiveMsg->freeMsg();
    }
    else
    {
    	printf("deqeuing failed");
    }
  }
}

