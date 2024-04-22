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
#include "LightBarrier.h"

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

  void EnableHallSensor(void);
  void DisableHallSensor(void);
  uint32_t GetHallSensorValue(void);

  uint32_t mHallSensorMaxValue;

  Stepper* mpDriveMotor;
  StepperConfig_t mDriveMotorConf;

  LightBarrier mLightBarrier;

  struct {
    uint32_t maxValue;
    float relativeAngle;
  }mMagnetInfo;
};

#endif /* CUBEGRID_H_ */
