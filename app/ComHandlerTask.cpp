/*
 * Filename: TemplateTask.cpp
 * Author: André Krummenacher
 * Date: 12.11.2023
 * Description: Template of a Task to serve as a starting point
 */

#include <string>
#include <stdio.h>
#include <map>
#include "ComHandlerTask.h"
#include "main.h"
#include "paradef.h"
#include "mjson.h"

ComHandlerTask* ComHandlerTask::mspThis = 0;

enum colors{
  COLOR_BLANK,
  COLOR_RED,
  COLOR_YELLOW,
  COLOR_BLUE
};

ComHandlerTask::ComHandlerTask(TaskId id, const char* name): Task(id, name)
{
  mpIsrEventMsg = Message::reserveIsr(MSG_ID_TEMPLATETASK_EVENT, ComHandlerTaskId, 0);
  mpTimerLed = new Timer(ComHandlerTaskId, TimerComLed);
  mpTimerLed->setInterval(500);
  mpTimerLed->setSingleShot(false);
  mpTimerLed->start();

  //Just testing some stuff here
  StepperConfig_t conf = Stepper::GetDefaultConfiguration();
  conf.pGpioStepOutput = GPIOC;
  conf.GpioPinStepOutput = GPIO_PIN_0;
  mpStepper = new Stepper(conf);
  mpStepper->StartRotation(2*_2PI);


  std::string str = "{\n\"time\": \"2023-10-10 17:10:05\",\n\"config\": {\n\"1\": \"red\",\n\"2\": \"blue\",\n\"3\": \"red\",\n\"4\": \"yellow\",\n\"5\": \"\",\n\"6\": \"\",\n\"7\": \"yellow\",\n\"8\": \"red\"\n}\n}";

  colors cubeColors[8];
  std::map<std::string, int> colorMap;

  colorMap[""] = COLOR_BLANK;
  colorMap["red"] = COLOR_RED;
  colorMap["yellow"] = COLOR_YELLOW;
  colorMap["blue"] = COLOR_BLUE;

  for(int i = 0; i <= 8; i++)
  {
    const char* buf;    //TODO:Check if this causes memory leak

    std::string path = "$.config[\"" + std::to_string(i) + "\"]";
    mjson_find(str.c_str(), str.length(), path.c_str(), &buf, NULL);

    if(buf != nullptr)
    {
      cubeColors[i] = colorMap[buf];
    }
    assert_param(buf != nullptr);
  }


  //end

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
