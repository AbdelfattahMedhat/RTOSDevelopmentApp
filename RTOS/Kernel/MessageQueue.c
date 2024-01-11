/*
 * MessageQueue.c
 *
 *  Created on: DEC 19, 2023
 *      Author: Abdelfattah Med
 */


#include "MessageQueue.h"


#if ENABLE_MESSAGEQUEUE == 1

extern TCB* 		readyList;
extern TCB* 		waitingList;
extern TCB* 		currentTCB;
extern TCB* 		highestRdyTCB;
extern uint8_t		OSStarted;

uint8_t			MQStorage[MAX_MESSAGEQUEUE_STORAGE];
uint32_t		mq_last_free_storage_ind = 0;

MessageQueue	MQList[MAX_MESSAGEQUEUE_NO];
uint32_t		mq_last_free_ind = 0;

static void msgDataCpy(uint8_t* source, uint8_t* dest, uint32_t size);



MessageQueue MQCreate(uint32_t msgQueueSize, uint32_t msgSize){
	MessageQueue 	msgQueue 		= &MQList[mq_last_free_ind++];
	uint32_t		totalMsgSize	= (msgSize + sizeof(void*));
	uint32_t		totalQueueSize	= totalMsgSize*msgQueueSize;
	Message*		newMsg = NULL;
	uint32_t		ind = 0;

	msgQueue->freeList 			= NULL;
	msgQueue->freeListLast 		= NULL;
	msgQueue->freeListPreLast 	= NULL;
	msgQueue->mqWaitingList 	= NULL;
	msgQueue->msgList 			= NULL;
	msgQueue->msgListLast 		= NULL;
	msgQueue->msgListPreLast 	= NULL;
	msgQueue->msgNum 			= 0;

	msgQueue->msgSize			= msgSize;
	msgQueue->queueMAXSize		= msgQueueSize;

	// adapt freeList
	newMsg = (Message*)(&MQStorage[mq_last_free_storage_ind]);
	insertEnd(&msgQueue->freeList, newMsg, &msgQueue->freeListLast, &msgQueue->freeListPreLast);
	for(ind = 0; ind < (msgQueueSize - 1); ind++){
		newMsg = (Message*)((uint32_t)newMsg + totalMsgSize);
		insertEnd(&msgQueue->freeList, newMsg, &msgQueue->freeListLast, &msgQueue->freeListPreLast);
	}
	mq_last_free_storage_ind += totalQueueSize;
	return msgQueue;
}
Bool MQPut(MessageQueue msgQueue, Message* msg){
	TCB* pTCB = NULL;
	Bool status = FALSE;
	Message* msgPtr = getListHead(&msgQueue->freeList, &msgQueue->freeListLast, &msgQueue->freeListPreLast);
	if(msgPtr != NULL){
		msgDataCpy(msg, (((uint8_t*)msgPtr)+sizeof(void*)), msgQueue->msgSize);
		//msgDataCpy(msg, (msgPtr+1), msgQueue->msgSize);
		insertEnd(&msgQueue->msgList, msgPtr, &msgQueue->msgListLast, &msgQueue->msgListPreLast);
		msgQueue->msgNum++;
		disableInterrupts();

		pTCB = E_extractHead(&msgQueue->mqWaitingList);
		if(pTCB != NULL){
			deleteTCB(&waitingList, pTCB);
			insertTCB(&readyList, pTCB, OT_PRI, OD_DESC);
			pTCB->msgQueue = NULL;

			if(pTCB->taskPri > currentTCB->taskPri){
				highestRdyTCB = extractHead(&readyList);
				insertTCB(&readyList, currentTCB, OT_PRI, OD_DESC);
				OSTaskSwitch();
			}
		}

		enableInterrupts();
		status = TRUE;
	}
	return status;
}
Bool MQGet(MessageQueue msgQueue, Message* msg, uint32_t timeout){
	Bool status = FALSE;
	Message* msgPtr = NULL;
	if(msgQueue->msgNum == 0){
		if(timeout > 0){
			disableInterrupts();
			currentTCB->waitTicks		= timeout;
			currentTCB->msgQueue 		= msgQueue;
			insertTCB(&waitingList, currentTCB, OT_TICKS, OD_ASC);
			E_insertTCB(&msgQueue->mqWaitingList, currentTCB, OT_PRI, OD_DESC);
			highestRdyTCB = extractHead(&readyList);
			OSTaskSwitch();///// -> waiting
			enableInterrupts();
		}
	}
	if(msgQueue->msgNum > 0){
		msgPtr = getListHead(&msgQueue->msgList, &msgQueue->msgListLast, &msgQueue->msgListPreLast);
		msgDataCpy((((uint8_t*)msgPtr)+sizeof(void*)), msg, msgQueue->msgSize);
		insertEnd(&msgQueue->freeList, msgPtr, &msgQueue->freeListLast, &msgQueue->freeListPreLast);
		msgQueue->msgNum--;
		status = TRUE;
		currentTCB->waitTicks = 0;
	}
	return status;
}

static void msgDataCpy(uint8_t* source, uint8_t* dest, uint32_t size){
	uint32_t ind = 0;
	for (int ind = 0; ind < size; ++ind) {
		dest[ind] = source[ind];
	}
}
#endif
