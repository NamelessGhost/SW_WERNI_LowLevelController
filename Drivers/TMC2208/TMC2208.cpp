/*
 * TMC2208.cpp
 *
 *  Created on: Dec 18, 2023
 *      Author: krumm
 */

#include <TMC2208.h>
#include <string.h>
#include "usart.h"

TMC2208::TMC2208()
{

  //Default values for GCONF register
  mGconfSr.gconf.I_scale_analog = 1;
  mGconfSr.gconf.internal_Rsense = 0;
  mGconfSr.gconf.en_spreadcycle = 1;
  mGconfSr.gconf.shaft = 0;
  mGconfSr.gconf.index_otpw = 0;
  mGconfSr.gconf.index_step = 0;
  mGconfSr.gconf.pdn_disable = 1;
  mGconfSr.gconf.mstep_reg_select = 0;    //Microstep select using MS1,MS2
  mGconfSr.gconf.multistep_filt = 1;
  mGconfSr.gconf.test_mode = 0;

  mChopconfSr.chopconf.diss2vs = 0;
  mChopconfSr.chopconf.diss2g = 0;
  mChopconfSr.chopconf.dedge = 0;
  mChopconfSr.chopconf.intpol = 0;   //Default is 1
  mChopconfSr.chopconf.MRES = 1;  //Halfstepping
  mChopconfSr.chopconf.reserved1 = 0;
  mChopconfSr.chopconf.vsense = 1;   //Check!
  mChopconfSr.chopconf.TBL = 0;
  mChopconfSr.chopconf.reserved2 = 0;
  mChopconfSr.chopconf.HEND = 0;
  mChopconfSr.chopconf.HSTRT = 0;    //Check!
  mChopconfSr.chopconf.TOFF = 3;

}

TMC2208::~TMC2208()
{
  // TODO Auto-generated destructor stub
}

void TMC2208::ReadConfig()
{
  ReadRegister(REG_ADDR_GCONF, &mGconfSr);
  ReadRegister(REG_ADDR_CHOPCONF, &mChopconfSr);
}

void TMC2208::WriteConfig()
{
  WriteRegister(REG_ADDR_GCONF, &mGconfSr);
  WriteRegister(REG_ADDR_CHOPCONF, &mChopconfSr);
}

void TMC2208::ReadRegister(uint8_t regAddr, tmc2208_reg_data_t* pRegData)
{
  tmc2208_read_msg_t lReadMsg;
  tmc2208_write_msg_t lReadResponse;

  lReadMsg.header = 0x05;
  lReadMsg.address = 0x00;
  lReadMsg.writeBit = 0x01;
  lReadMsg.regAddress = regAddr;

  lReadMsg.crc = CalculateCRC((uint8_t*)&lReadMsg, 3);

  UartWrite((uint8_t*)&lReadMsg, 4);             //Send read request to TMC2208

  UartRead((uint8_t*)&lReadResponse, 8);    //Receive response from TMC2208

  //Byteswap32((uint8_t*)&lReadResponse.regData);

  memcpy(pRegData, &lReadResponse.regData, sizeof(tmc2208_reg_data_t));
}

void TMC2208::WriteRegister(uint8_t regAddr, const tmc2208_reg_data_t* pRegData)
{
  tmc2208_write_msg_t lWriteMsg;

  lWriteMsg.header = 0x05;
  lWriteMsg.address = 0x00;
  lWriteMsg.writeBit = 0x01;
  lWriteMsg.regAddress = regAddr;
  lWriteMsg.regData = SwapBytes(pRegData);

  //Byteswap32((uint8_t*)&lWriteMsg.regData);

  lWriteMsg.crc = CalculateCRC((uint8_t*)&lWriteMsg, sizeof(lWriteMsg)-1);

  UartWrite((uint8_t*)&lWriteMsg, 8);
}

uint8_t TMC2208::CalculateCRC(uint8_t data[], uint8_t len)
{
  uint8_t crc = 0;
  for (uint8_t i = 0; i < len; i++) {
    uint8_t currentByte = data[i];
    for (uint8_t j = 0; j < 8; j++) {
      if ((crc >> 7) ^ (currentByte & 0x01)) {
        crc = (crc << 1) ^ 0x07;
      } else {
        crc = (crc << 1);
      }
      crc &= 0xff;
      currentByte = currentByte >> 1;
    }
  }
  return crc;
}

//Swap the bytes in the data register so that byte 3 lies first in memory
tmc2208_reg_data_t TMC2208::SwapBytes(const tmc2208_reg_data_t* pReg)
{
  uint8_t buff[4];
  buff[0] = *((uint8_t*)pReg + 3);
  buff[1] = *((uint8_t*)pReg + 2);
  buff[2] = *((uint8_t*)pReg + 1);
  buff[3] = *((uint8_t*)pReg + 0);

  return *((tmc2208_reg_data_t*)buff);

}

void TMC2208::UartWrite(const uint8_t* pData, uint32_t len)
{
  HAL_UART_Transmit(&huart5, pData, len, 10);
}

void TMC2208::UartRead( uint8_t* pData, uint32_t len)
{
  HAL_UART_Receive(&huart5, pData, len, 10);
}
