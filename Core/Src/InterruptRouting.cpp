/*
 * Filename: InterruptRouting.cpp
 * Author: André Krummenacher
 * Date: 12.11.2023
 * Description: Interface that routes HAL callback functions to registered classes.
 */


#include <stdio.h>
#include "InterruptRouting.h"
#include "main.h"

std::vector<Iinterruptable*> Iinterruptable::outputCompareIntReceivers;

Iinterruptable::Iinterruptable()
{
  outputCompareIntReceivers.push_back(this);
}

Iinterruptable::~Iinterruptable(){}

void Iinterruptable::OutputCompareIntCb(TIM_HandleTypeDef* htim)
{
  //Implement in derived class to use.
}


//************ C-Functions below ***************

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef* htim)
{
  if(htim->Instance == TIM1)
  {
    for(Iinterruptable* receiver : Iinterruptable::outputCompareIntReceivers)
    {
      receiver->OutputCompareIntCb(htim);
    }
  }
}
