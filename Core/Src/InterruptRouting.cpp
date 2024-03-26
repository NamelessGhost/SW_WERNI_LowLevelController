/*
 * Filename: InterruptRouting.cpp
 * Author: André Krummenacher
 * Date: 12.11.2023
 * Description: Interface that routes HAL callback functions to registered classes.
 */


#include <stdio.h>
#include "InterruptRouting.h"
#include "main.h"

std::vector<Iinterruptable*> Iinterruptable::sInterruptSubscriberVector;

Iinterruptable::Iinterruptable()
{
  sInterruptSubscriberVector.push_back(this);
}

Iinterruptable::~Iinterruptable(){}

void Iinterruptable::OutputCompareIntCb(TIM_HandleTypeDef* htim)
{
  //Implement in derived class to use.
}

void Iinterruptable::UartTxCompleteCb(UART_HandleTypeDef* huart)
{
  //Implement in derived class to use.
}

void Iinterruptable::UartRxDataAvailableCb(UART_HandleTypeDef* huart)
{
  //Implement in derived class to use.
}

void Iinterruptable::UartRxRtoCallback(UART_HandleTypeDef* huart)
{
  //Implement in derived class to use.
}

void Iinterruptable::AdcConvCompleteCb(ADC_HandleTypeDef *hadc)
{
  //Implement in derived class to use.
}

void Iinterruptable::TimPeriodElapsedCb(TIM_HandleTypeDef *htim)
{
  //Implement in derived class to use.
}

//************ C-Functions below ***************

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef* htim)
{
  for(Iinterruptable* receiver : Iinterruptable::sInterruptSubscriberVector)
  {
    receiver->OutputCompareIntCb(htim);
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart)
{
  for(Iinterruptable* receiver : Iinterruptable::sInterruptSubscriberVector)
  {
    receiver->UartTxCompleteCb(huart);
  }
}

void UART_RxFtCallback(UART_HandleTypeDef* huart)
{
  for(Iinterruptable* receiver : Iinterruptable::sInterruptSubscriberVector)
  {
    receiver->UartRxDataAvailableCb(huart);
  }
}

void UART_RxRtoCallback(UART_HandleTypeDef* huart)
{
  for(Iinterruptable* receiver : Iinterruptable::sInterruptSubscriberVector)
  {
    receiver->UartRxRtoCallback(huart);
  }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  for(Iinterruptable* receiver : Iinterruptable::sInterruptSubscriberVector)
  {
    receiver->AdcConvCompleteCb(hadc);
  }
}

//This interrupt is forwarded from main.c to here
void HAL_TIM_PeriodElapsedCallback_FWD(TIM_HandleTypeDef *htim)
{
  for(Iinterruptable* receiver : Iinterruptable::sInterruptSubscriberVector)
  {
    receiver->TimPeriodElapsedCb(htim);
  }
}
