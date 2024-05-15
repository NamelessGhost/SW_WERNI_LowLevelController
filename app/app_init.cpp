/*
 * app_init.cpp
 *
 *  Created on: Dec 27, 2019
 *      Author: axel
 */

//#include "LopTask.h"
#include <WerniTask.h>
#include "app_init.h"
#include "TemplateTask.h"
#include "ComHandlerTask.h"
#include "IoTask.h"

void app_init()
{
  // *** Create Tasks ***

  ComHandlerTask::instance();
  WerniTask::instance();
  IoTask::instance();

  // *** Start Tasks ***

  Message* lpMsgWerniTask = Message::reserve(MSG_ID_START, WerniTaskId);
  Message* lpMsgComHandlerTask = Message::reserve(MSG_ID_START, ComHandlerTaskId);
  Message* lpMsgIoTask = Message::reserve(MSG_ID_START, IoTaskId);

  lpMsgComHandlerTask->sendMsg();
  lpMsgWerniTask->sendMsg();
  lpMsgIoTask->sendMsg();
}
