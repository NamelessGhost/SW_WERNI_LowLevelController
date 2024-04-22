/*
 * CubeGrid.cpp
 *
 *  Created on: Dec 11, 2023
 *      Author: krumm
 */

#include <CubeGrid.h>

#include "FreeRTOS.h"
#include "stm32g4xx_hal_tim.h"

CubeGrid::CubeGrid(void)
{
  mDriveMotorConf = Stepper::GetDefaultConfiguration();
  mDriveMotorConf.GpioPinStepOutput = STPR_STP_GRID_Pin;
  mDriveMotorConf.pGpioStepOutput = STPR_STP_GRID_GPIO_Port;
  mDriveMotorConf.GpioPinEnableOutput = STPR_EN_GRID_Pin;
  mDriveMotorConf.pGpioEnableOutput = STPR_EN_GRID_GPIO_Port;
  mDriveMotorConf.GpioPinDirectionOutput = STPR_DIR_GRID_Pin;
  mDriveMotorConf.pGpioDirectionOutput = STPR_DIR_GRID_GPIO_Port;
  mDriveMotorConf.GpioPinMS1Output = STPR_MS1_Pin;
  mDriveMotorConf.pGpioMS1Output = STPR_MS1_GPIO_Port;
  mDriveMotorConf.GpioPinMS2Output = STPR_MS2_Pin;
  mDriveMotorConf.pGpioMS2Output = STPR_MS2_GPIO_Port;
  mDriveMotorConf.TargetAngularVelocity = CUBEGRID_TARGET_ANGULAR_VELOCITY * CUBEGRID_GEAR_FACTOR;
  mDriveMotorConf.AngularAcceleration = CUBEGRID_ANGULAR_ACCELERATION * CUBEGRID_GEAR_FACTOR;

  mpDriveMotor = new Stepper(mDriveMotorConf);

  mHallSensorMaxValue = 0;
}

CubeGrid::~CubeGrid(void)
{
  delete mpDriveMotor;
}

void CubeGrid::Rotate(int16_t degrees)
{
  mpDriveMotor->Enable(true);
  assert_param(ClearToRotate());
  mpDriveMotor->StartRotationBlocking(DEG_TO_RAD(degrees) * CUBEGRID_GEAR_FACTOR);
  mpDriveMotor->Enable(false);
}

void CubeGrid::DoHoming(void)
{
  assert_param(ClearToRotate());
  static uint32_t lSensorValue;
  lSensorValue = 0;

  mpDriveMotor->Enable(true);
  //1.) Reduce speed
  mDriveMotorConf.TargetAngularVelocity = CUBEGRID_HOMING_ANGULAR_VELOCITY * CUBEGRID_GEAR_FACTOR;
  mpDriveMotor->SetConfiguration(mDriveMotorConf);

  //2.) Do full rotation with reduced speed, search highest sensor value
  mMagnetInfo.maxValue = 0;
  mMagnetInfo.relativeAngle = 0;

  mpDriveMotor->StartRotation(FULL_ROTATION_RAD * CUBEGRID_GEAR_FACTOR);

  while(mpDriveMotor->GetState() == ROTATING)
  {
    lSensorValue = GetHallSensorValue();
    if(lSensorValue > mMagnetInfo.maxValue)
    {
      mMagnetInfo.maxValue = lSensorValue;
      mMagnetInfo.relativeAngle = mpDriveMotor->GetRotationAngle();
    }
  }

  if(mMagnetInfo.maxValue < CUBEGRID_HALL_SIGNAL_THRESHOLD)
  {
    //TODO: Implement what to do when no magnet is detected
    assert_param(false);
  }

  //3.) Rotate to position 20 steps away from previous magnet position
  mpDriveMotor->StartRotationBlocking(mMagnetInfo.relativeAngle - (20 * FULL_STEP_RAD));

  //4.) Dual step until we reach highest Hall-Sensor value
  mHallSensorMaxValue = 0;
  while(mHallSensorMaxValue < ((lSensorValue = GetHallSensorValue()) + 100))
   {
     mHallSensorMaxValue = lSensorValue;
     mpDriveMotor->StartRotationBlocking(FULL_STEP_RAD * 2);
     vTaskDelay(pdMS_TO_TICKS(10));
   }

  //5.) Take dual step back because we overstepped
  mpDriveMotor->StartRotationBlocking(-FULL_STEP_RAD * 2);

  //6.) Reconfigure for normal rotation speed
  mDriveMotorConf.TargetAngularVelocity = CUBEGRID_TARGET_ANGULAR_VELOCITY * CUBEGRID_GEAR_FACTOR;
  mpDriveMotor->SetConfiguration(mDriveMotorConf);
  mpDriveMotor->Enable(false);
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

void CubeGrid::EnableHallSensor(void)
{
  HAL_TIM_IC_Start(CUBEGRID_HALL_IC_TIMER_HANDLE, CUBEGRID_HALL_IC_CH_PERIOD);
  HAL_TIM_IC_Start(CUBEGRID_HALL_IC_TIMER_HANDLE, CUBEGRID_HALL_IC_CH_DUTYCYCLE);
}

void CubeGrid::DisableHallSensor(void)
{
  HAL_TIM_IC_Stop(CUBEGRID_HALL_IC_TIMER_HANDLE, CUBEGRID_HALL_IC_CH_PERIOD);
  HAL_TIM_IC_Stop(CUBEGRID_HALL_IC_TIMER_HANDLE, CUBEGRID_HALL_IC_CH_DUTYCYCLE);
}

uint32_t CubeGrid::GetHallSensorValue(void)
{
  uint32_t lPeriod = 0;
  uint64_t lDutyCycleSum = 0;

  lPeriod = HAL_TIM_ReadCapturedValue(CUBEGRID_HALL_IC_TIMER_HANDLE, CUBEGRID_HALL_IC_CH_PERIOD);

  //Make sure the hall frequency matches what we expect
  if((lPeriod <= CUBEGRID_HALL_SIGNAL_PERIOD_MAX) && (lPeriod >= CUBEGRID_HALL_SIGNAL_PERIOD_MIN))
  {
    for(uint32_t i = 0; i < CUBEGRID_HALL_SIGNAL_AVERAGING; i++)
    {
      lDutyCycleSum += HAL_TIM_ReadCapturedValue(CUBEGRID_HALL_IC_TIMER_HANDLE, CUBEGRID_HALL_IC_CH_DUTYCYCLE);
      vTaskDelay(pdMS_TO_TICKS(1));   //Wait a bit for the measured value to change
    }
    return lDutyCycleSum/CUBEGRID_HALL_SIGNAL_AVERAGING;
  }
  else
  {
    assert_param(false);  //No HALL-Sensor connected or signal invalid
    return 0;
  }
}

