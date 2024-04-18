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

enum MagazineSlotColor {
  RED,
  YELLOW,
  BLUE
};

class MagazineSlot
{
public:
  MagazineSlot(MagazineSlotColor color);
  virtual ~MagazineSlot();
  void StartDispensingCubes(uint32_t cnt);
  bool CheckFinished(void);

  MagazineSlotState mState;
  MagazineSlotColor mColor;

private:
  Stepper* mpDriveMotor;
  StepperConfig_t mDriveMotorConf;
  int32_t mInvertRotationDirection;   //-1 or 1, -1 inverts rotation direction
};

#endif /* MAGAZINESLOT_H_ */
