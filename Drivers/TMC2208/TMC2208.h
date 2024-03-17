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
#define REG_ADDR_CHOPCONF   0x6C

#pragma pack(1)

typedef union {
  uint32_t value;
  struct {
    uint8_t I_scale_analog   :1;   //BIT0
    uint8_t internal_Rsense  :1;
    uint8_t en_spreadcycle   :1;
    uint8_t shaft            :1;
    uint8_t index_otpw       :1;
    uint8_t index_step       :1;
    uint8_t pdn_disable      :1;
    uint8_t mstep_reg_select :1;
    uint8_t multistep_filt   :1;
    uint8_t test_mode        :1;
    uint8_t reserved0        :8;
    uint8_t reserved1        :8;
    uint8_t reserved2        :6;    //BIT31
  };
} tmc2208_gconf_reg_t;

typedef union {
  uint32_t value;
  struct {
    uint8_t TOFF             :4;   //BIT0
    uint8_t HSTRT            :3;
    uint8_t HEND             :4;
    uint8_t reserved2        :4;
    uint8_t TBL              :2;
    uint8_t vsense           :1;
    uint8_t reserved1        :6;
    uint8_t MRES             :4;
    uint8_t intpol           :1;
    uint8_t dedge            :1;
    uint8_t diss2g           :1;
    uint8_t diss2vs          :1;   //BIT31
  };
} tmc2208_chopconf_reg_t;

typedef union {
  tmc2208_gconf_reg_t gconf;
  tmc2208_chopconf_reg_t chopconf;
} tmc2208_reg_data_t;

typedef struct {
  uint8_t header    :8;
  uint8_t address   :8;
  uint8_t regAddress:7;
  uint8_t writeBit  :1;
  tmc2208_reg_data_t regData;
  uint8_t crc       :8;
} tmc2208_write_msg_t;

typedef struct {
  uint8_t header    :8;
  uint8_t address   :8;
  uint8_t regAddress:7;
  uint8_t writeBit  :1;
  uint8_t crc       :8;
} tmc2208_read_msg_t;

#pragma pack()

class TMC2208
{
public:
  TMC2208* Instance();
  void InitDriver();
  void WriteConfig();
  void ReadConfig();
  void SetDriverStepFactor(float stepFactor);

private:
  static TMC2208* mspThis;
  static bool msDriverInitialized;
  //Shadow registers
  tmc2208_reg_data_t mGconfSr;
  tmc2208_reg_data_t mChopconfSr;

  //Private functions
  TMC2208();
  virtual ~TMC2208();
  void ReadRegister(uint8_t regAddr, tmc2208_reg_data_t* pRegData);
  void WriteRegister(uint8_t regAddr, const tmc2208_reg_data_t* pRegData);
  uint8_t CalculateCRC(uint8_t data[], uint8_t len);
  tmc2208_reg_data_t SwapBytes(const tmc2208_reg_data_t* pReg);
  void UartWrite(const uint8_t* pData, uint32_t len);
  void UartRead( uint8_t* pData, uint32_t len);
  void UartClearRxFifo();
};

#endif /* TMC2208_TMC2208_H_ */
