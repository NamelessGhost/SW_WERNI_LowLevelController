/*
 * Filename: TemplateTask.cpp
 * Author: André Krummenacher
 * Date: 12.11.2023
 * Description: Template of a Task to serve as a starting point
 */

#include <string.h>
#include <stdio.h>
#include "ComHandlerTask.h"
#include "main.h"
#include "paradef.h"

ComHandlerTask* ComHandlerTask::mspThis = 0;

ComHandlerTask::ComHandlerTask(TaskId id, const char* name): Task(id, name)
{
  mpIsrEventMsg = Message::reserveIsr(MSG_ID_TEMPLATETASK_EVENT, ComHandlerTaskId, 0);
  mpTimerLed = new Timer(ComHandlerTaskId, TimerComLed);
  mpTimerLed->setInterval(500);
  mpTimerLed->setSingleShot(false);
  mpTimerLed->start();

  StepperConfig_t conf = Stepper::GetDefaultConfiguration();
  conf.pGpioStepOutput = GPIOC;
  conf.GpioPinStepOutput = GPIO_PIN_0;
  mpStepper = new Stepper(conf);
  mpStepper->StartRotation(3*_2PI);
  mpStepper = new Stepper(conf);
  mpStepper->StartRotation(3*_2PI);
  mpStepper = new Stepper(conf);
  //mpStepper->StartRotation(3*_2PI);

  conf.pGpioStepOutput = GPIOC;
  conf.GpioPinStepOutput = GPIO_PIN_1;
  mpStepper = new Stepper(conf);

  mpStepper->StartRotation(3*_2PI);

}

ComHandlerTask* ComHandlerTask::instance()
{
  if(ComHandlerTask::mspThis == 0)
  {
    mspThis = new ComHandlerTask(ComHandlerTaskId, "ComHandlerTask\0");
  }
  return ComHandlerTask::mspThis;
}

void ComHandlerTask::someEvent()
{
  mspThis->mpIsrEventMsg->setValue(123);
  mspThis->mpIsrEventMsg->sendMsg();
}

void ComHandlerTask::handleMessage(Message* message)
{
  switch(message->id())
  {
    case MSG_ID_START:
    {

      break;
    }
    case MSG_ID_TIMEOUT:
    {
      switch(message->data().longword)
      {
        case TimerComLed:
          HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
          break;
        default:
          break;
      }
      break;
    }
    default:
      break;
  }

}
