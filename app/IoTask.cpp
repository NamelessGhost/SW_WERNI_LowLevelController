/*
 * Filename: IoTask.cpp
 * Author: André Krummenacher
 * Date: 08.05.2024
 * Description: Task handling inputs and outputs
 */

#include <string.h>
#include <stdio.h>
#include "IoTask.h"
#include "Message.h"
#include "gpio.h"

IoTask* IoTask::mspThis = 0;

IoTask::IoTask(TaskId id, const char* name):
Task(id, name),
mIoUpdateTimer(IoTaskId, TimerIoUpdate),
mIoStatusUpdateTimer(IoTaskId, TimerIoStatusUpdate),
mBtnEStop(BTN_E_STOP_GPIO_Port, BTN_E_STOP_Pin),
mBtnStart(BTN_START_GPIO_Port, BTN_START_Pin)
{
  mIoUpdateTimer.setInterval(20);
  mIoUpdateTimer.setSingleShot(false);
  mIoUpdateTimer.start();

  //Disable this timer if no periodic IO status updates are desired
  mIoStatusUpdateTimer.setInterval(1000);
  mIoStatusUpdateTimer.setSingleShot(false);
  mIoStatusUpdateTimer.start();
}

IoTask* IoTask::instance()
{
  if(IoTask::mspThis == 0)
  {
    mspThis = new IoTask(IoTaskId, "IoTask\0");
  }
  return IoTask::mspThis;
}


void IoTask::handleMessage(Message* message)
{
  switch(message->id())
  {
    case MSG_ID_TIMEOUT:
    {
      switch(message->data().longword)
      {
        case TimerIoUpdate:
        {
          mBtnEStop.PeriodicUpdate(20);
          mBtnStart.PeriodicUpdate(20);

          if(mBtnEStop.CheckStateChanged() || mBtnEStop.CheckStateChanged())
          {
            SendIoState();
          }
          break;
        }


        case TimerIoStatusUpdate:
          SendIoState();
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

void IoTask::SendIoState()
{
  message_t lStateMessage;

  lStateMessage.cmd = CMD_SEND_STATE;
  lStateMessage.dataUnion.cmdSendIoState.btnEStopState = mBtnEStop.GetState();
  lStateMessage.dataUnion.cmdSendIoState.btnStartState = mBtnStart.GetState();

  //Create memory message and send to Werni Task
  Message* lpMsg = Message::reserve(MSG_ID_WERNI_MESSAGE, ComHandlerTaskId, sizeof(lStateMessage));
  memcpy(lpMsg->mem()->memory, &lStateMessage, sizeof(lStateMessage));
  lpMsg->sendMsg();
}
