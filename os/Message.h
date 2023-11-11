/*
 * Message.h
 *
 *  Created on: Apr 17, 2019
 *      Author: axel
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "MsgDef.h"

class MsgPool;
class Message
{
public:
  Message();
  static void init(MsgPool* pPool);

  static Message* reserve(uint8_t id, uint8_t destination);
  static Message* reserve(uint8_t id, uint8_t destination, size_t shared_memory_size);
  static Message* reserve(uint8_t id, uint8_t destination, DataUnion data);
  static Message* reserveIsr(uint8_t id, uint8_t destination, uint32_t isr_value);

  void sendMsg();
  void freeMsg();

  uint8_t id();
  uint8_t src();
  uint8_t dst();
  uint8_t type();
  DataUnion  data();
  SharedMem* mem();
  uint32_t value();
  void setValue(uint32_t value);

private:
  MsgUnion mMsg;
  static MsgPool* mspPool;
};

#endif /* MESSAGE_H_ */
