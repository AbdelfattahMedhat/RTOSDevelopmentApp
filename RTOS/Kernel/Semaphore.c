/*
 * Semaphore.c
 *
 *  Created on: DEC 13, 2023
 *      Author: Abdelfattah Med
 */
#include "Semaphore.h"

#if ENABLE_SEMAPHORES == 1

OSSem 			semList[MAX_SEM_NO];
uint32_t 		last_free_sem_ind = 0;

extern TCB* 		readyList;
extern TCB* 		waitingList;

extern TCB* 		currentTCB;
extern TCB* 		highestRdyTCB;

extern uint8_t		OSStarted;

OSSem* 	OSSemCreate(SemType semType, uint32_t initValue){
	OSSem* 	sem = &semList[last_free_sem_ind];
	last_free_sem_ind++;
	sem->semType = semType;
	sem->semValue = initValue;
	sem->semOwner = NULL;
	sem->semWaitingList = NULL;
	return sem;
}
Bool 	OSSemTake(OSSem* sem, uint32_t timeout){
	Bool result = FALSE;
	if(sem != NULL){
		disableInterrupts();
		if(sem->semValue == 0 && timeout > 0){

			if(sem->semType == SEM_MUTEX &&
					sem->semOwner != NULL && currentTCB->taskPri > sem->semOwner->taskPri){

				if(sem->semOwner->taskBasePri == 0)
				{
					sem->semOwner->taskBasePri = sem->semOwner->taskPri;
				}
				sem->semOwner->taskPri = currentTCB->taskPri;
				if(sem->semOwner->waitTicks == 0){
					deleteTCB(&readyList, sem->semOwner);
					insertTCB(&readyList, sem->semOwner,OT_PRI, OD_DESC);
				}
			}
			// wait
			currentTCB->waitTicks = timeout;
			currentTCB->sem = sem;
			insertTCB(&waitingList, currentTCB, OT_TICKS, OD_ASC);
			E_insertTCB(&sem->semWaitingList, currentTCB, OT_PRI, OD_DESC);
			highestRdyTCB = extractHead(&readyList);
			OSTaskSwitch();///// -> waiting
		}
		///////////////////////////////////////////
		if(sem->semValue > 0){

			if(sem->semType == SEM_MUTEX){
				sem->semOwner = currentTCB;
			}

			sem->semValue--;
			result = TRUE;
		}

		enableInterrupts();
	}
	return result;
}
void 	OSSemGive(OSSem* sem){
	TCB* pTCB = NULL;
	disableInterrupts();
	if(sem != NULL){
		if(sem->semType == SEM_BINARY || (sem->semType == SEM_MUTEX && currentTCB == sem->semOwner)){

			if(sem->semType == SEM_MUTEX){
				if(currentTCB->taskBasePri > 0){
					currentTCB->taskPri = currentTCB->taskBasePri;
					currentTCB->taskBasePri = 0;
				}
				sem->semOwner = NULL;
			}

			sem->semValue = 1;
			pTCB = E_extractHead(&sem->semWaitingList);
			if(pTCB != NULL){
				deleteTCB(&waitingList, pTCB);
				insertTCB(&readyList, pTCB, OT_PRI, OD_DESC);
				pTCB->sem = NULL;

				if(pTCB->taskPri > currentTCB->taskPri){
					highestRdyTCB = extractHead(&readyList);
					insertTCB(&readyList, currentTCB, OT_PRI, OD_DESC);
					OSTaskSwitch();
				}
			}
		}
	}
	enableInterrupts();
}






#endif
