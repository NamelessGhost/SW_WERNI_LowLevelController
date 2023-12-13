/*
 * CubeGrid.cpp
 *
 *  Created on: Dec 11, 2023
 *      Author: krumm
 */

#include <CubeGrid.h>

#include "FreeRTOS.h"

CubeGrid::CubeGrid(void)
{
  mDriveMotorConf = Stepper::GetDefaultConfiguration();
  mDriveMotorConf.GpioPinStepOutput = GPIO_PIN_0;
  mDriveMotorConf.pGpioStepOutput = GPIOC;
  mDriveMotorConf.TargetAngularVelocity = CUBEGRID_TARGET_ANGULAR_VELOCITY * CUBEGRID_GEAR_FACTOR;
  mpDriveMotor = new Stepper(mDriveMotorConf);
}

CubeGrid::~CubeGrid(void)
{
  delete mpDriveMotor;
}

void CubeGrid::Rotate(int16_t degrees)
{
  assert_param(ClearToRotate());
  mpDriveMotor->StartRotationBlocking(DEG_TO_RAD(degrees));
}

void CubeGrid::DoHoming(void)
{
  assert_param(ClearToRotate());

  //1.) Turn in one direction with normal speed
  mpDriveMotor->StartRotation(FULL_ROTATION_RAD);

  //2.) Wait for HAL-Switch signal
  while(/*Detect HAL Sensor Here*/false)
  {
    vTaskDelay(pdMS_TO_TICKS(1));
  }
  mpDriveMotor->StopRotation();

  //3.) Reduce speed, rotate in other direction
  mDriveMotorConf.TargetAngularVelocity = CUBEGRID_HOMING_ANGULAR_VELOCITY * CUBEGRID_GEAR_FACTOR;
  mpDriveMotor->SetConfiguration(mDriveMotorConf);
  mpDriveMotor->StartRotation(-FULL_ROTATION_RAD);

  //4.) Wait for HAL-Switch signal
  while(/*Detect HAL Sensor Here*/false)
  {
    vTaskDelay(pdMS_TO_TICKS(1));
  }
  mpDriveMotor->StopRotation();

  //5.) Reconfigure for normal rotation speed
  mDriveMotorConf.TargetAngularVelocity = CUBEGRID_TARGET_ANGULAR_VELOCITY * CUBEGRID_GEAR_FACTOR;
  mpDriveMotor->SetConfiguration(mDriveMotorConf);

}

bool CubeGrid::ClearToRotate(void)
{
  return true;  //TODO:Check light beam here
}

//Returns the angle in radians the motor needs to rotate for the cube grid to
//rotate the desired angle
float CubeGrid::CalculateMotorRotationAngle(int16_t degrees)
{
  return DEG_TO_RAD(degrees * CUBEGRID_GEAR_FACTOR);
}
