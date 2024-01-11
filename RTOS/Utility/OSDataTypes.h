/*
 * OSDataTypes.h
 *
 *  Created on: DEC 11, 2023
 *      Author: Abdelfattah Med
 */

#ifndef UTILITY_OSDATATYPES_H_
#define UTILITY_OSDATATYPES_H_

#include "stdint.h"
#include "stddef.h"
#include "OSConfig.h"

typedef enum{
	SEM_BINARY, SEM_MUTEX
}SemType;

typedef enum{
	FALSE = 0, TRUE = 1
}Bool;

#if ENABLE_SEMAPHORES == 1
typedef struct ossem{
	SemType 		semType;
	uint32_t		semValue;
	struct tcb*		semWaitingList;
	struct tcb*		semOwner;  			// for MUTEX only
}OSSem;
#endif

#if ENABLE_EVENTGROUPS == 1
typedef struct eventgroup{
	uint32_t		egBits;
	struct tcb*		egWaitingList;
}EventGroup;
#endif

#if ENABLE_MESSAGEQUEUE == 1
typedef struct message{
	struct message* nextMSG;
}Message;

typedef struct queue{
	Message*		freeList;
	Message*		freeListLast;
	Message*		freeListPreLast;

	Message*		msgList;
	Message*		msgListLast;
	Message*		msgListPreLast;

	uint32_t		msgNum;
	uint32_t		queueMAXSize;
	uint32_t		msgSize;
	struct tcb*		mqWaitingList;
}Queue;

typedef Queue* MessageQueue;
#endif

typedef struct tcb{
	uint32_t* 		pTaskStack;
	struct tcb* 	next;
	struct tcb* 	prev;
	// for additional linking to make the same TCB linked in two different lists
	struct tcb* 	enext;
	struct tcb* 	eprev;
	void (*pfTaskName)(void);
	uint32_t		taskPri;
	uint32_t		waitTicks;
#if ENABLE_SEMAPHORES == 1
	uint32_t		taskBasePri;
	OSSem*			sem;
#endif
#if ENABLE_EVENTGROUPS == 1
	EventGroup*		eventGroup;
	uint32_t		egWaitBits;
	Bool			egWaitAllBits;
#endif
#if ENABLE_MESSAGEQUEUE == 1
	MessageQueue msgQueue;
#endif
}TCB;

typedef enum{
	OT_PRI, OT_TICKS
}OrderType;

typedef enum{
	OD_ASC, OD_DESC
}OrderDir;

#endif /* UTILITY_OSDATATYPES_H_ */
