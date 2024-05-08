/*
 * Button.cpp
 *
 *  Created on: May 8, 2024
 *      Author: krumm
 */

#include <Button.h>

Button::Button(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  mpGPIOx = GPIOx;
  mGPIO_Pin = GPIO_Pin;

  assert_param(mpGPIOx != NULL);

  mState = (HAL_GPIO_ReadPin(mpGPIOx, mGPIO_Pin) ? BTN_PRESSED:BTN_RELEASED);
  mButtonPressedCycles = 0;
}
Button::~Button(void)
{

}

//Call this function periodically (every 20-50ms) to update button states
//Button debounce using undersampling
void Button::PeriodicUpdate(uint32_t periodMs)
{
  mStateChanged = false;

  switch(mState)
  {
    case BTN_RELEASED:
      if(HAL_GPIO_ReadPin(mpGPIOx, mGPIO_Pin) == GPIO_PIN_SET)
      {
        mState = BTN_PRESSED;
        mStateChanged = true;
      }
      break;

    case BTN_PRESSED:
      mButtonPressedCycles += 1;

      if(HAL_GPIO_ReadPin(mpGPIOx, mGPIO_Pin) == GPIO_PIN_RESET)
      {
        if(mButtonPressedCycles * periodMs >= BUTTON_LONG_PRESS_THRESHOLD_MS)
        {
          mState = BTN_LONG_CLICKED;
          mStateChanged = true;
        }
        else
        {
          mState = BTN_SHORT_CLICKED;
          mStateChanged = true;
        }
      }
      break;

    default:
      mState = BTN_RELEASED;
      mStateChanged = true;
      break;
  }
}


bool Button::CheckStateChanged(void)
{
  if(mStateChanged)
  {
    mStateChanged = false;
    return true;
  }
  return false;
}

enum BTN_STATES Button::GetState(void)
{
  return mState;
}

