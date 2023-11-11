/*
 * MsgPool.h
 *
 *  Created on: Apr 17, 2019
 *      Author: axel
 */

#ifndef MSGPOOL_H_
#define MSGPOOL_H_

#include "Message.h"
#include "Mutex.h"

class MsgPool
{
public:
  virtual ~MsgPool();
  static MsgPool* instance();

  static Message* resMsg();
  static void freeMsg(Message* p_msg);

private:
  MsgPool();

  struct msg_enty_struct
  {
    Message* mMsg;
    bool     mUsed;
  };

  msg_enty_struct mPool[MSG_POOL_SIZE];
  Mutex mMutex;

  static MsgPool* mspThis;
};

#endif /* MSGPOOL_H_ */
