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

int app_init_done = 0;

void app_init()
{
  /////////////////    create tasks

  ComHandlerTask::instance();
  WerniTask::instance();

  //Periodic CAN timer task
//  CanTask::instance();
//
//  LopTask::instance();


  /////////////////    start tasks

  //Periodic CAN timer task
//  Message* lpMsg = Message::reserve(MSG_ID_START, PeriodicCanTaskId);
//  lpMsg->sendMsg();
//
//  // start CanOpen task
//  lpMsg = Message::reserve(MSG_ID_START, LopTaskId);
//  lpMsg->sendMsg();
//
//  SET_BLUE_STATE_LED(LED_OFF);
//  app_init_done = 1;
}
