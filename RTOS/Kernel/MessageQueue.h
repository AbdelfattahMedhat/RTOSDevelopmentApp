/*
 * MessageQueue.h
 *
 *  Created on: DEC 19, 2023
 *      Author: Abdelfattah Med
 */

#ifndef KERNEL_MESSAGEQUEUE_H_
#define KERNEL_MESSAGEQUEUE_H_

#include "List.h"
#include "OSDataTypes.h"
#include "OSConfig.h"
#include "OSPort.h"

#if ENABLE_MESSAGEQUEUE == 1

MessageQueue MQCreate(uint32_t msgQueueSize, uint32_t msgSize);
Bool MQPut(MessageQueue msgQueue, Message* msg);
Bool MQGet(MessageQueue msgQueue, Message* msg, uint32_t timeout);

#endif

#endif /* KERNEL_MESSAGEQUEUE_H_ */
