/*
 * Button.h
 *
 *  Created on: May 8, 2024
 *      Author: krumm
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "paradef.h"
#include "ProtocolDefWERNI.h"
#include "stm32g4xx_hal.h"
#include "Message.h"
#include "TaskList.h"


class Button
{
public:
  Button(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
  virtual ~Button();

  void PeriodicUpdate(uint32_t periodMs);
  bool CheckStateChanged(void);
  enum BTN_STATES GetState(void);

private:
  enum BTN_STATES mState;
  uint32_t mButtonPressedCycles;
  GPIO_TypeDef* mpGPIOx;
  uint16_t mGPIO_Pin;
  bool mStateChanged;


};

#endif /* BUTTON_H_ */
