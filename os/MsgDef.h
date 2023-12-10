/*
 * MsgDef.h
 *
 *  Created on: Apr 17, 2019
 *      Author: axel
 */

#ifndef MSGDEF_H_
#define MSGDEF_H_

#include <stdint.h>
#include <stddef.h>

#define MSG_POOL_SIZE               25
#define MSG_QUEUE_SIZE              10

#define MSG_ID_START                1
#define MSG_ID_TIMEOUT              2
#define MSG_ID_TEMPLATETASK_EVENT   3
#define MSG_ID_WERNI_MESSAGE        4

enum MsgType
{
  MsgNone,
  MsgData,
  MsgMem,
  MsgIsr
};

#pragma pack(1)

struct SharedMem
{
  size_t length;
  void*  memory;
};

union DataUnion
{
  uint8_t   bytes[4];
  uint32_t  longword;
};

union MsgUnion
{
  struct MsgTypeData
  {
    uint8_t       Id;
    uint8_t       Type;
    uint8_t       DestAddr;
    uint8_t       SrcAddr;
    DataUnion     Data;
  }DataMsg;

  struct MsgTypeNone
  {
    uint8_t       Id;
    uint8_t       Type;
    uint8_t       DestAddr;
    uint8_t       SrcAddr;
  }NoneMsg;

  struct MsgTypeMemory
  {
    uint8_t       Id;
    uint8_t       Type;
    uint8_t       DestAddr;
    uint8_t       SrcAddr;
    SharedMem*    Memory;
  }MemMsg;

  struct MsgTypeIsr
  {
    uint8_t       Id;
    uint8_t       Type;
    uint8_t       DestAddr;
    uint32_t      Value;
  }IsrMsg;
};

#pragma pack()

#endif /* MSGDEF_H_ */
