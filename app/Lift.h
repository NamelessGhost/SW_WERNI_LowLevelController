/*
 * CubeGrid.h
 *
 *  Created on: March 10, 2023
 *      Author: krumm
 */

#ifndef LIFT_H_
#define LIFT_H_

#include "Stepper.h"
#include "paradef.h"

enum LiftState {
  UNHOMED,
  LIFT_UP,
  LIFT_DOWN
};

class Lift
{
public:
  Lift(void);
  virtual ~Lift(void);
  void MoveUp(void);
  void MoveDown(void);
  void DoHoming(void);

private:


  LiftState mState;
  Stepper* mpDriveMotor;
  StepperConfig_t mDriveMotorConf;
};

#endif /* CUBEGRID_H_ */
