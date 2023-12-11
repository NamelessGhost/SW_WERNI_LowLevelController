/*
 * MagazineSlot.cpp
 *
 *  Created on: Dec 11, 2023
 *      Author: krumm
 */

#include <MagazineSlot.h>

MagazineSlot::MagazineSlot(void)
{
  mDriveMotorConf = Stepper::GetDefaultConfiguration();
  mDriveMotorConf.TargetAngularVelocity = CUBEGRID_TARGET_ANGULAR_VELOCITY * CUBEGRID_GEAR_FACTOR;
  mpDriveMotor = new Stepper(mDriveMotorConf);

  mState = IDLE;
}

MagazineSlot::~MagazineSlot(void)
{
  delete mpDriveMotor;
}

void MagazineSlot::StartDispensingCubes(uint32_t cnt)
{
  assert_param(mState == IDLE);
  mState = DISPENSING;
  mpDriveMotor->StartRotation(cnt * DEG_TO_RAD(MAGAZINESLOT_ANGLE_PER_CUBE));
}

bool MagazineSlot::CheckFinished(void)
{
  return (mpDriveMotor->GetState() != ROTATING);
}
