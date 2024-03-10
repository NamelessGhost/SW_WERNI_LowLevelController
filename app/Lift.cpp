/*
 * CubeGrid.cpp
 *
 *  Created on: March 10, 2023
 *      Author: krumm
 */

#include <Lift.h>

#include "FreeRTOS.h"

Lift::Lift(void)
{
  mDriveMotorConf = Stepper::GetDefaultConfiguration();
  mDriveMotorConf.GpioPinStepOutput = STPR_STP_LIFT_Pin;
  mDriveMotorConf.pGpioStepOutput = STPR_STP_LIFT_GPIO_Port;
  mDriveMotorConf.GpioPinEnableOutput = STPR_EN_LIFT_Pin;
  mDriveMotorConf.pGpioEnableOutput = STPR_EN_LIFT_GPIO_Port;
  mDriveMotorConf.GpioPinDirectionOutput = STPR_DIR_LIFT_Pin;
  mDriveMotorConf.pGpioDirectionOutput = STPR_DIR_LIFT_GPIO_Port;
  mDriveMotorConf.GpioPinMS1Output = STPR_MS1_Pin;
  mDriveMotorConf.pGpioMS1Output = STPR_MS1_GPIO_Port;
  mDriveMotorConf.GpioPinMS2Output = STPR_MS2_Pin;
  mDriveMotorConf.pGpioMS2Output = STPR_MS2_GPIO_Port;

  mDriveMotorConf.TargetAngularVelocity = LIFT_TARGET_VERTICAL_VELOCITY * LIFT_GEAR_FACTOR;

  mpDriveMotor = new Stepper(mDriveMotorConf);
  mpDriveMotor->Enable(true);
  mState = LIFT_UP;    //TODO:DoHoming(); instead!!!!!!!!!!!!!!!!!!!!!
}

Lift::~Lift(void)
{
  delete mpDriveMotor;
}

void Lift::MoveUp(void)
{
  if(mState == LIFT_DOWN)
  mpDriveMotor->StartRotationBlocking(LIFT_VERTICAL_TRAVEL * LIFT_GEAR_FACTOR);
  mState = LIFT_UP;
}

void Lift::MoveDown(void)
{
  if(mState == LIFT_UP)
  mpDriveMotor->StartRotationBlocking(-LIFT_VERTICAL_TRAVEL * LIFT_GEAR_FACTOR);
  mState = LIFT_DOWN;
}

void Lift::DoHoming(void)
{
  //1.) Reduce speed, rotate until limit switch is hit
  mDriveMotorConf.TargetAngularVelocity = LIFT_HOMING_VERTICAL_VELOCITY * LIFT_GEAR_FACTOR;
  mpDriveMotor->SetConfiguration(mDriveMotorConf);
  mpDriveMotor->StartRotation(LIFT_VERTICAL_TRAVEL * 2);   //2 * LIFT_VERTICAL_TRAVEL to make sure we hit limit switch

  //2.) Wait for Limit-Switch signal
  while(HAL_GPIO_ReadPin(LIMIT_LIFT_GPIO_Port, LIMIT_LIFT_Pin) == GPIO_PIN_RESET)
  {
    vTaskDelay(pdMS_TO_TICKS(1));
  }
  mpDriveMotor->StopRotation();

  //3.) Reconfigure for normal rotation speed
  mDriveMotorConf.TargetAngularVelocity = LIFT_TARGET_VERTICAL_VELOCITY * LIFT_GEAR_FACTOR;
  mpDriveMotor->SetConfiguration(mDriveMotorConf);

  mState = LIFT_UP;
}
