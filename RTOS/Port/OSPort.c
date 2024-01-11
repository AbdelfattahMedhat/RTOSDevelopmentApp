/*
 * OSPort.c
 *
 *  Created on: DEC 11, 2023
 *      Author: Abdelfattah Med
 */

#include "OSPort.h"

extern TCB* 		readyList;
extern TCB* 		waitingList;

extern TCB* 		currentTCB;
extern TCB* 		highestRdyTCB;

extern uint8_t		OSStarted;

void OSSysTickSetup(void){
	SysTick->LOAD = (CPU_CLOCK/TICK_RATE) - 1;
	SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk|
			SysTick_CTRL_ENABLE_Msk|
			SysTick_CTRL_TICKINT_Msk);
}

void SVC_Handler(void){
	asm("	LDR r0,=currentTCB			"); // &&TCB
	asm("	LDR r0,[r0]					"); // &TCB
	asm("	LDR r0,[r0]					"); // r0 = taskSP

	asm("	ldmia r0!, {r4-r11, r14}	");
	asm("	MSR	psp,r0					");

	asm("	mov r1,#0					");
	asm("	MSR	basepri,r1				");

	asm("	bx	r14						");
}
void PendSV_Handler(void){
	asm("	MRS r0,psp					"); // currentTCB

	asm("	TST	r14,#0x10				");
	asm("	IT	eq						");
	asm("	vstmdbeq r0!, {s16-s31}		");
	asm("								");
	asm("	stmdb r0!, {r4-r11, r14}	");
	asm("								");
	asm("	LDR r3,=currentTCB			"); // &&TCB
	asm("	LDR r2,[r3]					"); // &TCB
	asm("								");
	asm("	STR	r0,[r2]					");
	asm("								");
	asm("	LDR r0,=highestRdyTCB		"); // &&TCB
	asm("	LDR r0,[r0]					"); // &TCB
	asm("	STR	r0,[r3]					");// currentTCB = highestRdyTCB

	asm("	LDR r0,[r0]					"); // r0 = taskSP

	asm("	ldmia r0!, {r4-r11, r14}	");

	asm("	TST	r14,#0x10				");
	asm("	IT	eq						");
	asm("	vldmiaeq r0!, {s16-s31}		");

	asm("	MSR	psp,r0					");

	asm("	bx	r14						");
}
void SysTick_Handler(void){
	TCB* pTCB = waitingList;
	disableInterrupts();
	while(pTCB != NULL){
		pTCB->waitTicks--;
		if(pTCB->waitTicks == 0){
			pTCB = extractHead(&waitingList);
			insertTCB(&readyList, pTCB, OT_PRI, OD_DESC);
#if ENABLE_SEMAPHORES == 1
			if(pTCB->sem != NULL){
				E_deleteTCB(&pTCB->sem->semWaitingList, pTCB);
				pTCB->sem = NULL;
			}
#endif
#if ENABLE_EVENTGROUPS == 1
			if(pTCB->eventGroup != NULL){
				E_deleteTCB(&pTCB->eventGroup->egWaitingList, pTCB);
				pTCB->eventGroup = NULL;
			}
#endif
#if ENABLE_MESSAGEQUEUE == 1
			if(pTCB->msgQueue != NULL){
				E_deleteTCB(&pTCB->msgQueue->mqWaitingList, pTCB);
				pTCB->msgQueue = NULL;
			}
#endif
			pTCB = waitingList;
		}
		else{
			pTCB = pTCB->next;
		}
	}
	if(readyList != NULL && readyList->taskPri > currentTCB->taskPri){
		highestRdyTCB = extractHead(&readyList);
		insertTCB(&readyList, currentTCB, OT_PRI, OD_DESC);
		OSTaskSwitch();
	}
	enableInterrupts();
}
