/*
 * TMC2208.cpp
 *
 *  Created on: Dec 18, 2023
 *      Author: krumm
 */

#include <TMC2208.h>
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

}

TMC2208::~TMC2208()
{
  // TODO Auto-generated destructor stub
}

void TMC2208::WriteConfig()
{
  tmc2208_write_msg_t lWriteMsg;
  lWriteMsg.header = 0x05;
  lWriteMsg.address = 0x00;
  lWriteMsg.writeBit = 0x01;
  lWriteMsg.regAddress = REG_ADDR_GCONF;
  lWriteMsg.regData = mGconfSr;
  Byteswap32((uint8_t*)&lWriteMsg.regData);
  *((uint8_t*)&lWriteMsg.regData + 3) = 0b11000000;
  lWriteMsg.crc = CalculateCRC((uint8_t*)&lWriteMsg, sizeof(lWriteMsg)-1);
  UartWrite((uint8_t*)&lWriteMsg, sizeof(lWriteMsg));
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

void TMC2208::Byteswap32(uint8_t* ptr)
{
  uint8_t buf[4];
  buf[3] = *(ptr + 0);
  buf[2] = *(ptr + 1);
  buf[1] = *(ptr + 2);
  buf[0] = *(ptr + 3);

  *(ptr + 0) = buf[0];
  *(ptr + 1) = buf[1];
  *(ptr + 2) = buf[2];
  *(ptr + 3) = buf[3];
}

void TMC2208::UartWrite(const uint8_t* pData, uint32_t len)
{
  HAL_UART_Transmit(&huart5, pData, len, 10);
}
