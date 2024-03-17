/*
 * Filename: Stepper.cpp
 * Author: André Krummenacher
 * Date: 22.11.2023
 * Description: Stepper Motor
 */

#include <stdio.h>
#include "Stepper.h"
#include "main.h"
#include "cmath"
#include "FreeRTOS.h"
#include "TMC2208.h"


bool Stepper::sUsedTimerChannels[STEPPER_TIMER_MAX_CHANNELS] = {0};

Stepper::Stepper(StepperConfig_t config) : Iinterruptable()
{
  SetConfiguration(config);  //Apply the configuration

  mpTimerHandle = STEPPER_STEP_TIMER_HANDLE;

  mStepperState = OFF;
  mRotationState = STANDSTILL;
  mStepOutputState = OUTPUT_LOW;

  mCurrentAngularVeloctiy = 0;          //in rad/s
  mCurrentRotationAngle = 0;			//in rad/s
  mTargetRotationAngle = 0;
  mStepsRotated = 0;
}

StepperConfig_t Stepper::GetDefaultConfiguration()
{
  StepperConfig_t lDefaultConfig;

  //Physical parameters
  lDefaultConfig.StartAngularVelocity = STEPPER_START_ANGULAR_VELOCITY;   //rad/s
  lDefaultConfig.TargetAngularVelocity = STEPPER_TARGET_ANGULAR_VELOCITY; //rad/s
  lDefaultConfig.AngularAcceleration = STEPPER_ANGULAR_ACCELERATION;      //rad/s^2
  lDefaultConfig.RolloutAngle = STEPPER_ROLLOUT_ANGLE;                    //rad

  //GPIO parameters
  lDefaultConfig.pGpioStepOutput = NULL;        //Step output GPIO peripheral pointer
  lDefaultConfig.GpioPinStepOutput = 0x00;      //Step output GPIO pin

  lDefaultConfig.pGpioEnableOutput = NULL;       //Step output GPIO peripheral pointer
  lDefaultConfig.GpioPinEnableOutput = 0x00;          //Step output GPIO pin

  lDefaultConfig.pGpioDirectionOutput = NULL;    //Step output GPIO peripheral pointer
  lDefaultConfig.GpioPinDirectionOutput = 0x00;       //Step output GPIO pin

  lDefaultConfig.pGpioMS1Output = NULL;          //Step output GPIO peripheral pointer
  lDefaultConfig.GpioPinMS1Output = 0x00;             //Step output GPIO pin

  lDefaultConfig.pGpioMS2Output = NULL;          //Step output GPIO peripheral pointer
  lDefaultConfig.GpioPinMS2Output = 0x00;             //Step output GPIO pin

  //Stepper driver parameters
  lDefaultConfig.DriverStepFactor = STEPPER_DRIVER_STEP_FACTOR; //Half-stepping
  lDefaultConfig.MotorStepFactor = STEPPER_MOTOR_STEP_FACTOR;   //Motor Steps/rotation

  return lDefaultConfig;
}

void Stepper::ReserveTimerChannel(void)
{
  mMutex.lock();

  mTimerChannel = -1;

  //Find a channel that is currently not in use
  for(uint32_t i = 0; i < STEPPER_TIMER_MAX_CHANNELS; i++)
  {
    if(sUsedTimerChannels[i] == false)
    {
      mTimerChannel = i * 4;    //User to manipulate timer
      mTimerActiveChannel = static_cast<HAL_TIM_ActiveChannel>(1<<i);   //Used for interrupt handling

      sUsedTimerChannels[i] = true;
      break;
    }
  }

  mMutex.unlock();

  assert_param(mTimerChannel != -1);   //Stop here if no channel is free
}

void Stepper::FreeTimerChannel(void)
{
  sUsedTimerChannels[ mTimerChannel/4 ] = 0;  //Todo:Beautyfy
}

StepperConfig_t Stepper::GetConfiguration(void)
{
  StepperConfig_t lConfig;

  //Physical parameters
  lConfig.StartAngularVelocity = mStartAngularVelocity;
  lConfig.TargetAngularVelocity = mTargetAngularVelocity;
  lConfig.AngularAcceleration = mAngularAcceleration;
  lConfig.RolloutAngle = mRolloutAngle;

  //GPIO parameters
  lConfig.pGpioStepOutput = mpGpioStepOutput;
  lConfig.GpioPinStepOutput = mGpioPinStepOutput;

  lConfig.pGpioEnableOutput = mpGpioEnableOutput;
  lConfig.GpioPinEnableOutput = mGpioPinEnableOutput;

  lConfig.pGpioDirectionOutput = mpGpioDirectionOutput;
  lConfig.GpioPinDirectionOutput = mGpioPinDirectionOutput;

  lConfig.pGpioMS1Output = mpGpioMS1Output;
  lConfig.GpioPinMS1Output = mGpioPinMS1Output;

  lConfig.pGpioMS2Output = mpGpioMS2Output;
  lConfig.GpioPinMS2Output = mGpioPinMS2Output;

  //Stepper driver parameters
  lConfig.DriverStepFactor = mDriverStepFactor;
  lConfig.MotorStepFactor = mMotorStepFactor;

  return lConfig;
}

void Stepper::SetConfiguration(StepperConfig_t config)
{
  //Physical parameters
  mStartAngularVelocity = config.StartAngularVelocity;
  mTargetAngularVelocity = config.TargetAngularVelocity;
  mAngularAcceleration = config.AngularAcceleration;
  mRolloutAngle = config.RolloutAngle;

  //GPIO parameters
  mpGpioStepOutput = config.pGpioStepOutput;
  mGpioPinStepOutput = config.GpioPinStepOutput;

  mpGpioEnableOutput = config.pGpioEnableOutput;
  mGpioPinEnableOutput = config.GpioPinEnableOutput;

  mpGpioDirectionOutput = config.pGpioDirectionOutput;
  mGpioPinDirectionOutput = config.GpioPinDirectionOutput;

  mpGpioMS1Output = config.pGpioMS1Output;
  mGpioPinMS1Output = config.GpioPinMS1Output;

  mpGpioMS2Output = config.pGpioMS2Output;
  mGpioPinMS2Output = config.GpioPinMS2Output;

  //Stepper driver parameters
  mDriverStepFactor = config.DriverStepFactor;
  mMotorStepFactor = config.MotorStepFactor;

  mDriveTrainFactor = mDriverStepFactor * mMotorStepFactor;
  assert_param(mpGpioStepOutput != NULL);
  assert_param(mpGpioEnableOutput != NULL);
  assert_param(mpGpioDirectionOutput != NULL);

  SetDriverStepFactor(mDriverStepFactor);

#ifdef STEPPER_USE_UART
  TMC2208::Instance()->InitDriver();
#endif
}

void Stepper::StartRotationBlocking(float angle)
{
  StartRotation(angle);
  while(mStepperState == ROTATING)
  {
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void Stepper::StartRotation(float angle)
{
  float lAngle = fabsf(angle);

  SetDirection(angle > 0);
  ReserveTimerChannel();

  if(mStepperState != ROTATING)
  {
    mStepperState = ROTATING;
    mRotationState = ACCELERATING;
    mTargetRotationAngle = lAngle;

    //Zero parameters at start of rotation
    mStepsRotated = 0;
    mCurrentAngularVeloctiy = 0;
    mCurrentRotationAngle = 0;
    mCurrentRotationAngle = 0;

    volatile uint32_t* ptr = &velocitySWV;  //TODO:Remove DEBUG

    uint32_t timerCount = __HAL_TIM_GET_COUNTER(mpTimerHandle);
    __HAL_TIM_SET_COMPARE(mpTimerHandle, mTimerChannel, timerCount + CalculateTicksUntilNextStep()/2);
    HAL_TIM_OC_Start_IT(mpTimerHandle, mTimerChannel);
  }
  else
  {
    assert_param(false);
  }
}

void Stepper::StopRotation()
{
  mStepperState = HOLDING;
  mRotationState = STANDSTILL;

  HAL_TIM_OC_Stop_IT(mpTimerHandle, mTimerChannel);

  FreeTimerChannel();
}

void Stepper::Enable(bool enable)
{
  mEnabled = enable;
  GPIO_PinState state = mEnabled ? GPIO_PIN_RESET:GPIO_PIN_SET;   //EN is low active
  HAL_GPIO_WritePin(mpGpioEnableOutput, mGpioPinEnableOutput, state);
}

StepperState Stepper::GetState(void)
{
  return mStepperState;
}

bool Stepper::IsTimeToStartDecelerating()
{
  float lStopAngle = (mCurrentAngularVeloctiy * mCurrentAngularVeloctiy) / (2 * mAngularAcceleration);
  return ( (lStopAngle + mRolloutAngle) >= (mTargetRotationAngle - mCurrentRotationAngle) );
}

uint32_t Stepper::CalculateTicksUntilNextStep()
{
  uint32_t lRet = 0;
  mCurrentRotationAngle = mStepsRotated * mDriverStepFactor * mMotorStepFactor * _2PI;

  switch(mRotationState)
    {
      case ACCELERATING:
        if(mCurrentAngularVeloctiy < mTargetAngularVelocity)   //We still need to accelerate
        {
          mCurrentAngularVeloctiy = mAngularAcceleration *
                                    std::sqrt( (2 * mCurrentRotationAngle) / mAngularAcceleration) +
                                    mStartAngularVelocity;
        }
        else    //Target velocity reached
        {
          mRotationState = CONSTVELOCITY;
        }
        if(IsTimeToStartDecelerating()) mRotationState = DECCELERATING;
        break;

      case CONSTVELOCITY:
        if(IsTimeToStartDecelerating()) mRotationState = DECCELERATING;
        break;


      case DECCELERATING:
      {
        float lRemainingAngle = (mTargetRotationAngle - mCurrentRotationAngle - mRolloutAngle);
        if(mCurrentAngularVeloctiy > mStartAngularVelocity)   //We still need to decelerate
        {
          mCurrentAngularVeloctiy = mAngularAcceleration *
                                    std::sqrt( (2 * lRemainingAngle) / mAngularAcceleration);

          if(mCurrentAngularVeloctiy < mStartAngularVelocity) //Limit minimum velocity to prevent large period times
          {
            mCurrentAngularVeloctiy = mStartAngularVelocity;
          }
        }
        else
        {
          mRotationState = ROLLOUT;
        }
      }
        break;

      case ROLLOUT:
        if(mCurrentRotationAngle >= mTargetRotationAngle)
        {
          mRotationState = STANDSTILL;
          mCurrentAngularVeloctiy = 0;
          StopRotation();
        }
        break;

      default:
        break;
    }

  velocitySWV = (uint32_t)(mCurrentAngularVeloctiy*10000);  //TODO:Remove DEBUG

  if(mCurrentAngularVeloctiy > 0)
  {
    float lAbsRotationFrequency = std::abs(mCurrentAngularVeloctiy/_2PI);
    lRet = (uint32_t)(STEPPER_TIMER_CNT_FREQUENCY / (lAbsRotationFrequency / mDriveTrainFactor ));
    if(lRet > 0x0001FFFF)assert_param(false);   //TODO:Remove DEBUG, prevent ticks greater than timer cnt register size
  }

  return lRet;
}

void Stepper::SetDirection(bool cw)
{
  GPIO_PinState lState = cw ? GPIO_PIN_SET : GPIO_PIN_RESET;
  HAL_GPIO_WritePin(mpGpioDirectionOutput, mGpioPinDirectionOutput, lState);
}

void Stepper::SetDriverStepFactor(float stepFactor)
{
#ifdef STEPPER_USE_UART
  TMC2208::Instance()->SetDriverStepFactor(stepFactor);
#else
  assert_param(mpGpioMS1Output != NULL);
  assert_param(mpGpioMS2Output != NULL);

  uint32_t lStepFactor = 1/stepFactor;    //Half-Stepping -> = 2
  switch(lStepFactor)
  {
    case 2:
      HAL_GPIO_WritePin(mpGpioMS1Output, mGpioPinMS1Output, GPIO_PIN_SET);
      HAL_GPIO_WritePin(mpGpioMS2Output, mGpioPinMS2Output, GPIO_PIN_RESET);
      break;

    case 4:
      HAL_GPIO_WritePin(mpGpioMS1Output, mGpioPinMS1Output, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(mpGpioMS2Output, mGpioPinMS2Output, GPIO_PIN_SET);
      break;

    case 8:
      HAL_GPIO_WritePin(mpGpioMS1Output, mGpioPinMS1Output, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(mpGpioMS2Output, mGpioPinMS2Output, GPIO_PIN_RESET);
      break;

    case 16:
      HAL_GPIO_WritePin(mpGpioMS1Output, mGpioPinMS1Output, GPIO_PIN_SET);
      HAL_GPIO_WritePin(mpGpioMS2Output, mGpioPinMS2Output, GPIO_PIN_SET);
      break;

    default:
      assert_param(false);
      break;
  }
#endif
}

void Stepper::OutputCompareIntCb(TIM_HandleTypeDef* htim)
{
  if(htim->Channel == mTimerActiveChannel){       //If the corresponding timer channel creates an interrupt

    uint32_t lPreviousCompareValue = __HAL_TIM_GET_COMPARE(mpTimerHandle, mTimerChannel);

    if(mStepOutputState == OUTPUT_LOW){
      mStepOutputState = OUTPUT_HIGH;
      mStepsRotated += 1;
    }
    else mStepOutputState = OUTPUT_LOW;

    HAL_GPIO_WritePin(mpGpioStepOutput, mGpioPinStepOutput, (GPIO_PinState)mStepOutputState);

    uint32_t ticks = CalculateTicksUntilNextStep()/2;

    __HAL_TIM_SET_COMPARE(mpTimerHandle, mTimerChannel, lPreviousCompareValue + ticks);   // Divide by to to get half period step output low and half period step output high
  }
}
