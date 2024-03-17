/*
 * WERNI.cpp
 *
 *  Created on: Dec 11, 2023
 *      Author: krumm
 */

#include <WerniTask.h>
#include <string.h>
#include "ComHandlerTask.h"


WerniTask* WerniTask::mspThis = 0;

WerniTask::WerniTask(TaskId id, const char* name):
Task(id, name),
mMagazineSlotRed(RED),
mMagazineSlotYellow(YELLOW),
mMagazineSlotBlue(BLUE)
{


}

WerniTask* WerniTask::instance(void)
{
  if(WerniTask::mspThis == 0)
  {
    mspThis = new WerniTask(WerniTaskId, "WerniTask\0");
  }
  return WerniTask::mspThis;
}

WerniTask::~WerniTask()
{
  // TODO Auto-generated destructor stub
}

void WerniTask::handleMessage(Message* message)
{
  switch(message->id())
  {
    case MSG_ID_START:
      break;

    case MSG_ID_WERNI_MESSAGE:
      handleWerniMessage(message);
      break;

    default:
      break;
  }
}

void WerniTask::handleWerniMessage(Message* message)
{
  message_t* lpMessage = (message_t*)message->mem()->memory;

  switch (lpMessage->cmd) {
    case CMD_ROTATE_GRID:
    {
      int16_t lAngle = lpMessage->dataUnion.cmdRotateGrid.degrees_h << 8;
      lAngle |= lpMessage->dataUnion.cmdRotateGrid.degrees_l;
      mCubeGrid.Rotate(lAngle);
    }
      break;

    case CMD_PLACE_CUBES:
    {
      mMagazineSlotRed.StartDispensingCubes(lpMessage->dataUnion.cmdPlaceCubes.cubes_red);
      mMagazineSlotYellow.StartDispensingCubes(lpMessage->dataUnion.cmdPlaceCubes.cubes_yellow);
      mMagazineSlotBlue.StartDispensingCubes(lpMessage->dataUnion.cmdPlaceCubes.cubes_blue);

      //Wait until all requested cubes have been dispensed
      while((mMagazineSlotRed.CheckFinished() == false) ||
            (mMagazineSlotYellow.CheckFinished() == false) ||
            (mMagazineSlotBlue.CheckFinished() == false))
      {
        vTaskDelay(pdMS_TO_TICKS(10));
      }
    }
      break;

    case CMD_MOVE_LIFT:
    {
      switch(lpMessage->dataUnion.cmdMoveLift)
      {
        case MOVE_UP:
          mCubeLift.MoveUp();
          break;

        case MOVE_DOWN:
          mCubeLift.MoveDown();
          break;

        default:
          break;
      }
    }
    break;

    case CMD_STATE:
      //TODO:Define what is supposed to happen here
      break;

    default:
      break;
  }
}

