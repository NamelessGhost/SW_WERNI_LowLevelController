/*
 * WERNI.h
 *
 *  Created on: Dec 11, 2023
 *      Author: krumm
 */

#ifndef WERNITASK_H_
#define WERNITASK_H_

#include "queue"
#include "TaskClass.h"
#include "Timer.h"
#include "paradef.h"
#include "ProtocolDefWERNI.h"

#include "CubeGrid.h"
#include "MagazineSlot.h"
#include "Lift.h"
#include "EnergyMeasurement.h"

class WerniTask: public Task
{
public:
  WerniTask(TaskId id, const char* name);
  virtual ~WerniTask();
  static WerniTask* instance(void);

protected:
  virtual void handleMessage(Message* message);

private:
  void SortWerniMessage(Message* message);
  void HandleMessageQueue(void);
  void RotateGrid(message_t* message);
  void PlaceCubes(message_t* message);
  void PrimeMagazine(void);
  void MoveLift(message_t* message);
  void SendState(void);
  void SendExecutionFinished(COMMAND cmd, bool success = true);

  static WerniTask* mspThis;
  WERNI_STATES mState;
  Timer mUpdateTimer;
  std::queue<message_t> mMessageQueue;

  CubeGrid mCubeGrid;
  Lift mCubeLift;
  MagazineSlot mMagazineSlotRed;
  MagazineSlot mMagazineSlotYellow;
  MagazineSlot mMagazineSlotBlue;
  EnergyMeasurement mEnergyMeasurement;
};

#endif /* WERNITASK_H_ */
