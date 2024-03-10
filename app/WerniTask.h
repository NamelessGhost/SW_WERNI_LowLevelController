/*
 * WERNI.h
 *
 *  Created on: Dec 11, 2023
 *      Author: krumm
 */

#ifndef WERNITASK_H_
#define WERNITASK_H_

#include "TaskClass.h"
#include "paradef.h"
#include "CubeGrid.h"
#include "MagazineSlot.h"
#include "Lift.h"

class WerniTask: public Task
{
public:
  WerniTask(TaskId id, const char* name);
  virtual ~WerniTask();
  static WerniTask* instance(void);

protected:
  virtual void handleMessage(Message* message);

private:
  void handleWerniMessage(Message* message);

  static WerniTask* mspThis;

  MagazineSlot mMagazineSlotRed;
  MagazineSlot mMagazineSlotYellow;
  MagazineSlot mMagazineSlotBlue;
  CubeGrid mCubeGrid;
  Lift mCubeLift;
};

#endif /* WERNITASK_H_ */
