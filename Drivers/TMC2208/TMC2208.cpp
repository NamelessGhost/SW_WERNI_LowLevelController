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
  CalculateAndAppendCRC((uint8_t*)&lWriteMsg, sizeof(lWriteMsg));
  UartWrite((uint8_t*)&lWriteMsg, sizeof(lWriteMsg));
}

void TMC2208::CalculateAndAppendCRC(uint8_t* pData, uint32_t len)
{
  int32_t i,j;
  uint8_t* crc = pData + (len-1); // CRC located in last byte of message
  uint8_t currentByte;

  crc = 0;
  for (i=0; i<(len-1); i++)
  { // Execute for all bytes of a message
    currentByte = pData[i]; // Retrieve a byte to be sent from Array
    for (j=0; j<8; j++)
    {
      if ((*crc >> 7) ^ (currentByte&0x01)) // update CRC based result of XOR operation
      {
        *crc = (*crc << 1) ^ 0x07;
      }
      else
      {
        *crc = (*crc << 1);
      }
      currentByte = currentByte >> 1;
    } // for CRC bit
  } // for message byte
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
