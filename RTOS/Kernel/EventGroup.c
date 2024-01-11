/*
 * EventGroup.c
 *
 *  Created on: DEC 15, 2023
 *      Author: Abdelfattah Med
 */


#include "EventGroup.h"

#if ENABLE_EVENTGROUPS == 1

EventGroup 	EGList[MAX_EVENTGROUP_NO];
uint32_t	EG_last_free_ind = 0;

extern TCB* 		readyList;
extern TCB* 		waitingList;
extern TCB* 		currentTCB;
extern TCB* 		highestRdyTCB;
extern uint8_t		OSStarted;

static Bool EGCheck(EventGroup* eventGroup, uint32_t bits, Bool allWaitBits);

static Bool EGCheck(EventGroup* eventGroup, uint32_t bits, Bool allWaitBits){
	Bool result = FALSE;
	if(allWaitBits == TRUE){
		if( (eventGroup->egBits&bits) ==  bits){
			result = TRUE;
		}
	}
	else{
		if(eventGroup->egBits&bits){
			result = TRUE;
		}
	}
	return result;
}
EventGroup*	EGCreate(void){
	EventGroup* eg = NULL;
	if(EG_last_free_ind < MAX_EVENTGROUP_NO){
		eg = &EGList[EG_last_free_ind];
		EG_last_free_ind++;
		eg->egBits 				= 0;
		eg->egWaitingList 		= NULL;
	}
	return eg;
}

uint32_t	EGClearBits(EventGroup* eventGroup, uint32_t bits){
	eventGroup->egBits &= ~bits;
	return eventGroup->egBits;
}

uint32_t	EGSetBits(EventGroup* eventGroup, uint32_t bits){
	TCB* pTCB = eventGroup->egWaitingList;
	TCB* pNext = NULL;

	disableInterrupts();
	eventGroup->egBits |= bits;

	while(pTCB != NULL){
		pNext = pTCB->enext;
		if(EGCheck(eventGroup, pTCB->egWaitBits, pTCB->egWaitAllBits)){
			// exit from egWaitingList and waitingList
			E_deleteTCB(&eventGroup->egWaitingList, pTCB);
			pTCB->eventGroup = NULL;
			deleteTCB(&waitingList, pTCB);
			pTCB->waitTicks = 0;
			insertTCB(&readyList, pTCB, OT_PRI, OD_DESC);
		}
		pTCB = pNext;
	}

	if(readyList != NULL && readyList->taskPri > currentTCB->taskPri){
		highestRdyTCB = extractHead(&readyList);
		insertTCB(&readyList, currentTCB, OT_PRI, OD_DESC);
		OSTaskSwitch();
	}
	enableInterrupts();
	return eventGroup->egBits;
}
uint32_t	EGWaitBits(EventGroup* eventGroup,
		uint32_t bits,
		Bool autoClear,
		Bool allBitsWait,
		uint32_t timeout){
	uint32_t returnBits = 0;
	if(EGCheck(eventGroup, bits, allBitsWait) == FALSE && timeout > 0){
		disableInterrupts();
		currentTCB->waitTicks		= timeout;
		currentTCB->eventGroup 		= eventGroup;
		currentTCB->egWaitAllBits 	= allBitsWait;
		currentTCB->egWaitBits 		= bits;
		insertTCB(&waitingList, currentTCB, OT_TICKS, OD_ASC);
		E_insertTCB(&eventGroup->egWaitingList, currentTCB, OT_PRI, OD_DESC);
		highestRdyTCB = extractHead(&readyList);
		OSTaskSwitch();///// -> waiting
		enableInterrupts();
	}
	returnBits = eventGroup->egBits;
	if(autoClear == TRUE){
		eventGroup->egBits &= ~bits;
	}
	return returnBits;
}

#endif
