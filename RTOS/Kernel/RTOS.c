/*
 * Task.c
 *
 *  Created on: DEC 11, 2023
 *      Author: Abdelfattah Med
 */

#include "RTOS.h"

TCB 		TCBList[MAX_TASKS_NO];
uint32_t	last_free_tcb_ind = 0;

uint32_t	idle_stack[IDLE_STACK_SIZE];

TCB* 		readyList 		= NULL;
TCB* 		waitingList 	= NULL;

TCB* 		currentTCB 		= NULL;
TCB* 		highestRdyTCB 	= NULL;

uint8_t		OSStarted		= 0;

static TCB* OSTaskInit(void (*pfTask)(void), uint32_t task_pri,
		uint32_t* task_stack_arr, uint32_t task_stack_size){
	// allocate TCB and Stack
	// Initialize TCB and Stack
	TCB* pTCB = &TCBList[last_free_tcb_ind];
	last_free_tcb_ind++;

	pTCB->next 				= NULL;
	pTCB->prev 				= NULL;
	pTCB->enext 			= NULL;
	pTCB->eprev 			= NULL;
#if ENABLE_SEMAPHORES == 1
	pTCB->taskBasePri		= 0;
	pTCB->sem 				= NULL;
#endif
#if ENABLE_EVENTGROUPS == 1
	pTCB->eventGroup 	= NULL;
	pTCB->egWaitBits	= 0;
	pTCB->egWaitAllBits = FALSE;
#endif
#if ENABLE_MESSAGEQUEUE == 1
	pTCB->msgQueue 		= NULL;
#endif
	pTCB->waitTicks 		= 0;
	pTCB->taskPri			= task_pri;
	pTCB->pfTaskName		= pfTask;
	pTCB->pTaskStack		= ((uint32_t)(task_stack_arr + task_stack_size))&0xFFFFFFFC;

	// psr, pc, lr, r12,r3-0
	// exc_ret, r11-4
	pTCB->pTaskStack--;
	*(pTCB->pTaskStack) = 0x01000000;

	pTCB->pTaskStack--;
	*(pTCB->pTaskStack) = pfTask;

	pTCB->pTaskStack--;
	*(pTCB->pTaskStack) = user_error;

	pTCB->pTaskStack -= 5;

	pTCB->pTaskStack--;
	*(pTCB->pTaskStack) = 0xFFFFFFFD;

	pTCB->pTaskStack -= 8;

	return pTCB;
}
void OSCreateTask(void (*pfTask)(void), uint32_t task_pri,
		uint32_t* task_stack_arr, uint32_t task_stack_size){

	TCB* pTCB =	OSTaskInit(pfTask, task_pri, task_stack_arr, task_stack_size);
	insertTCB(&readyList, pTCB, OT_PRI, OD_DESC);
	if(OSStarted == 1 && pTCB->taskPri > currentTCB->taskPri){
		insertTCB(&readyList, currentTCB, OT_PRI, OD_DESC);
		highestRdyTCB = extractHead(&readyList);
		OSTaskSwitch();
	}
}
void OSDelay(uint32_t waitTicks){
	disableInterrupts();
	currentTCB->waitTicks = waitTicks;
	insertTCB(&waitingList, currentTCB, OT_TICKS, OD_ASC);
	highestRdyTCB = extractHead(&readyList);
	OSTaskSwitch();
	enableInterrupts();
}
void OSStart(void){
	OSCreateTask(T_IDLE, 0, idle_stack, IDLE_STACK_SIZE);
	disableInterrupts();
	SCB->SHP[10] = 0xFF;// PendSV
	SCB->SHP[11] = 0xFF;// SysTick
	OSSysTickSetup();
	currentTCB = extractHead(&readyList);
	OSStarted = 1;
	OSStartFirstTask();
}

void user_error(void){
	while(1){
	}
}

void T_IDLE(void){
	while(1){

	}
}
