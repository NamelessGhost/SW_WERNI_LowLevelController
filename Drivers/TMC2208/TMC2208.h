/*
 * TMC2208.h
 *
 *  Created on: Dec 18, 2023
 *      Author: krumm
 */

#ifndef TMC2208_TMC2208_H_
#define TMC2208_TMC2208_H_

#include <stdint.h>

#define REG_ADDR_GCONF      0x00

#pragma pack(1)

typedef union {
  uint32_t value;
  struct {
    uint32_t I_scale_analog   :1;
    uint32_t internal_Rsense  :1;
    uint32_t en_spreadcycle   :1;
    uint32_t shaft            :1;
    uint32_t index_otpw       :1;
    uint32_t index_step       :1;
    uint32_t pdn_disable      :1;
    uint32_t mstep_reg_select :1;
    uint32_t multistep_filt   :1;
    uint32_t test_mode        :1;
    uint32_t reserved         :22;
  };
} tmc2208_gconf_reg_t;


typedef union {
  tmc2208_gconf_reg_t gconf;
} tmc2208_reg_data_t;

typedef struct {
  uint8_t header    :8;
  uint8_t address   :8;
  uint8_t regAddress:7;
  uint8_t writeBit  :1;
  tmc2208_reg_data_t regData;
  uint8_t crc       :8;
} tmc2208_write_msg_t;

#pragma pack()

class TMC2208
{
public:
  TMC2208();
  virtual ~TMC2208();
  void WriteConfig();

private:
  tmc2208_reg_data_t mGconfSr;   //GCONF shadow register
  void CalculateAndAppendCRC(uint8_t* pData, uint32_t len);
  void Byteswap32(uint8_t* ptr);
  void UartWrite(const uint8_t* pData, uint32_t len);
};

#endif /* TMC2208_TMC2208_H_ */
