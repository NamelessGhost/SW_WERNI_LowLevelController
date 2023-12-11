/*
 * CubeGrid.h
 *
 *  Created on: Dec 11, 2023
 *      Author: krumm
 */

#ifndef CUBEGRID_H_
#define CUBEGRID_H_

#include "Stepper.h"
#include "paradef.h"

class CubeGrid
{
public:
  CubeGrid(void);
  virtual ~CubeGrid(void);
  void Rotate(int16_t degrees);
  void DoHoming(void);


private:
  bool ClearToRotate(void);
  float CalculateMotorRotationAngle(int16_t degrees);

  Stepper* mpDriveMotor;
  StepperConfig_t mDriveMotorConf;
};

#endif /* CUBEGRID_H_ */
