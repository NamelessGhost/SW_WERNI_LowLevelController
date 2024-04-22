/*
 * MagazineSlot.cpp
 *
 *  Created on: Dec 11, 2023
 *      Author: krumm
 */

#include <MagazineSlot.h>

MagazineSlot::MagazineSlot(MagazineSlotColor color)
{
  mColor = color;
  mDriveMotorConf = Stepper::GetDefaultConfiguration();

  switch(mColor){
    case RED:
      mDriveMotorConf.GpioPinStepOutput = STPR_STP_MAG_R_Pin;
      mDriveMotorConf.pGpioStepOutput = STPR_STP_MAG_R_GPIO_Port;
      mDriveMotorConf.GpioPinEnableOutput = STPR_EN_MAG_R_Pin;
      mDriveMotorConf.pGpioEnableOutput = STPR_EN_MAG_R_GPIO_Port;
      mDriveMotorConf.GpioPinDirectionOutput = STPR_DIR_MAG_R_Pin;
      mDriveMotorConf.pGpioDirectionOutput = STPR_DIR_MAG_R_GPIO_Port;
      break;
    case YELLOW:
      mDriveMotorConf.GpioPinStepOutput = STPR_STP_MAG_Y_Pin;
      mDriveMotorConf.pGpioStepOutput = STPR_STP_MAG_Y_GPIO_Port;
      mDriveMotorConf.GpioPinEnableOutput = STPR_EN_MAG_Y_Pin;
      mDriveMotorConf.pGpioEnableOutput = STPR_EN_MAG_Y_GPIO_Port;
      mDriveMotorConf.GpioPinDirectionOutput = STPR_DIR_MAG_Y_Pin;
      mDriveMotorConf.pGpioDirectionOutput = STPR_DIR_MAG_Y_GPIO_Port;
      break;
    case BLUE:
      mDriveMotorConf.GpioPinStepOutput = STPR_STP_MAG_B_Pin;
      mDriveMotorConf.pGpioStepOutput = STPR_STP_MAG_B_GPIO_Port;
      mDriveMotorConf.GpioPinEnableOutput = STPR_EN_MAG_B_Pin;
      mDriveMotorConf.pGpioEnableOutput = STPR_EN_MAG_B_GPIO_Port;
      mDriveMotorConf.GpioPinDirectionOutput = STPR_DIR_MAG_B_Pin;
      mDriveMotorConf.pGpioDirectionOutput = STPR_DIR_MAG_B_GPIO_Port;
      break;
    default:
      assert_param(false);    //Invalid color
      break;
  }
  mDriveMotorConf.GpioPinMS1Output = STPR_MS1_Pin;
  mDriveMotorConf.pGpioMS1Output = STPR_MS1_GPIO_Port;
  mDriveMotorConf.GpioPinMS2Output = STPR_MS2_Pin;
  mDriveMotorConf.pGpioMS2Output = STPR_MS2_GPIO_Port;

  mDriveMotorConf.TargetAngularVelocity = CUBEGRID_TARGET_ANGULAR_VELOCITY * CUBEGRID_GEAR_FACTOR;
  mpDriveMotor = new Stepper(mDriveMotorConf);

  mInvertRotationDirection = MAGAZINESLOT_INVERT_ROTATION ? -1: 1;

  mState = IDLE;
}

MagazineSlot::~MagazineSlot(void)
{
  delete mpDriveMotor;
}

void MagazineSlot::StartDispensingCubes(uint32_t cnt)
{
  assert_param(mState == IDLE);
  mpDriveMotor->Enable(true);
  mState = DISPENSING;
  mpDriveMotor->StartRotation(cnt * DEG_TO_RAD(MAGAZINESLOT_ANGLE_PER_CUBE) * mInvertRotationDirection);
}

bool MagazineSlot::CheckFinished(void)
{
  if(mpDriveMotor->GetState() != ROTATING)
  {
    mpDriveMotor->Enable(false);
    mState = IDLE;
    return true;
  }
  return false;
}

void MagazineSlot::Prime(void)
{
  assert_param(mState == IDLE);
  mpDriveMotor->Enable(true);
  mState = PRIMEING;
  mpDriveMotor->StartRotation(DEG_TO_RAD(MAGAZINESLOT_ANGLE_PRIME) * mInvertRotationDirection);
}
