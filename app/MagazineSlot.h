/*
 * MagazineSlot.h
 *
 *  Created on: Dec 11, 2023
 *      Author: krumm
 */

#ifndef MAGAZINESLOT_H_
#define MAGAZINESLOT_H_

#include "Stepper.h"
#include "paradef.h"

enum MagazineSlotState {
  IDLE,
  DISPENSING,
};

class MagazineSlot
{
public:
  MagazineSlot();
  virtual ~MagazineSlot();
  void StartDispensingCubes(uint32_t cnt);
  bool CheckFinished(void);

  MagazineSlotState mState;

private:
  Stepper* mpDriveMotor;
  StepperConfig_t mDriveMotorConf;
};

#endif /* MAGAZINESLOT_H_ */
