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
#include "ProtocolDefWERNI.h"

class Lift
{
public:
  Lift(void);
  virtual ~Lift(void);
  void MoveUp(void);
  void MoveDown(void);
  void DoHoming(void);
  LIFT_STATES GetPosition(void);

private:


  LIFT_STATES mState;
  Stepper* mpDriveMotor;
  StepperConfig_t mDriveMotorConf;
};

#endif /* CUBEGRID_H_ */
