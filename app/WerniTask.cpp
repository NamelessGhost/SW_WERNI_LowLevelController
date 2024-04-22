/*
 * WERNI.cpp
 *
 *  Created on: Dec 11, 2023
 *      Author: krumm
 */

#include <WerniTask.h>
#include "Message.h"
#include "string.h"


WerniTask* WerniTask::mspThis = 0;

WerniTask::WerniTask(TaskId id, const char* name):
Task(id, name),
mUpdateTimer(WerniTaskId, TimerWerniUpdate),
mMagazineSlotRed(RED),
mMagazineSlotYellow(YELLOW),
mMagazineSlotBlue(BLUE)
{
  mState = READY;

  mUpdateTimer.setInterval(10);
  mUpdateTimer.setSingleShot(false);
  mUpdateTimer.start();

  mCubeGrid.DoHoming();
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
      SortWerniMessage(message);
      break;

    case MSG_ID_TIMEOUT:
      HandleMessageQueue();   //Gets a command out of the build queue and executes it blocking
      break;

    default:
      break;
  }
}

void WerniTask::SortWerniMessage(Message* message)
{
  message_t* lpMessage = (message_t*)message->mem()->memory;

  switch(lpMessage->cmd)
  {
    case CMD_PAUSE_BUILD:
      mState = BUILD_PAUSED;
      break;

    case CMD_RESUME_BUILD:
      mState = READY;
      break;

    case CMD_GET_STATE:
      SendState();
      break;

    default:
      mMessageQueue.push(*lpMessage);
      break;
  }
}

void WerniTask::HandleMessageQueue()
{
  assert_param(mState != BUILDING);   //Should never be BUILDING here

  if(mMessageQueue.empty()) return;

  if(mState == READY)
  {
    mState = BUILDING;
    mUpdateTimer.stop();    //Stop update timer so it does not flood our queue while building

      message_t lMessage = mMessageQueue.front(); // Get the front element
      mMessageQueue.pop();                        // Remove the front element

      switch(lMessage.cmd) {
        case CMD_ROTATE_GRID:
          RotateGrid(&lMessage);
          break;

        case CMD_PLACE_CUBES:
          PlaceCubes(&lMessage);
          break;

        case CMD_MOVE_LIFT:
          MoveLift(&lMessage);
          break;

        case CMD_PRIME_MAGAZINE:
          PrimeMagazine();
          break;

        default:
          assert_param(false);
          break;
      }
      mState = READY;
      mUpdateTimer.start(); //Restart update timer to handle next command in build queue
  }
}

void WerniTask::RotateGrid(message_t* message)
{
  int16_t lAngle = message->dataUnion.cmdRotateGrid.degrees_h << 8;
  lAngle |= message->dataUnion.cmdRotateGrid.degrees_l;
  mCubeGrid.Rotate(lAngle);
}

void WerniTask::PlaceCubes(message_t* message)
{
  mMagazineSlotRed.StartDispensingCubes(message->dataUnion.cmdPlaceCubes.cubes_red);
  mMagazineSlotYellow.StartDispensingCubes(message->dataUnion.cmdPlaceCubes.cubes_yellow);
  mMagazineSlotBlue.StartDispensingCubes(message->dataUnion.cmdPlaceCubes.cubes_blue);

  //Wait until all requested cubes have been dispensed
  while((mMagazineSlotRed.CheckFinished() == false) ||
        (mMagazineSlotYellow.CheckFinished() == false) ||
        (mMagazineSlotBlue.CheckFinished() == false))
  {
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void WerniTask::PrimeMagazine(void)
{
  mMagazineSlotRed.Prime();
  mMagazineSlotYellow.Prime();
  mMagazineSlotBlue.Prime();

  //Wait until all magazines have been primed
  while((mMagazineSlotRed.CheckFinished() == false) ||
        (mMagazineSlotYellow.CheckFinished() == false) ||
        (mMagazineSlotBlue.CheckFinished() == false))
  {
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void WerniTask::MoveLift(message_t* message)
{
  switch(message->dataUnion.cmdMoveLift)
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

void WerniTask::SendState(void)
{
  message_t lStateMessage;

  //TODO:Actually fill the state here!
  lStateMessage.cmd = CMD_SEND_STATE;
  lStateMessage.dataUnion.cmdSendState.dummy1 = 0xCA;
  lStateMessage.dataUnion.cmdSendState.dummy2 = 0xFF;
  lStateMessage.dataUnion.cmdSendState.dummy3 = 0xEE;
  lStateMessage.dataUnion.cmdSendState.dummy4 = 0xAA;

  //Create memory message and send to Werni Task
  Message* lpMsg = Message::reserve(MSG_ID_WERNI_MESSAGE, ComHandlerTaskId, sizeof(lStateMessage));
  memcpy(lpMsg->mem()->memory, &lStateMessage, sizeof(lStateMessage));
  lpMsg->sendMsg();
}


