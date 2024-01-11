/*
 * OSPort.h
 *
 *  Created on: DEC 11, 2023
 *      Author: Abdelfattah Med
 */

#ifndef PORT_OSPORT_H_
#define PORT_OSPORT_H_

#include "stm32f4xx.h"
#include "OSConfig.h"
#include "List.h"
#include "OSDataTypes.h"

#define OSTaskSwitch()							\
{												\
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;		\
}

#define OSStartFirstTask()						\
{												\
	asm(" svc 0 ");								\
}

#define disableInterrupts()				__set_BASEPRI(0x50)
#define enableInterrupts()				__set_BASEPRI(0x00)

void OSSysTickSetup(void);

void SVC_Handler(void) __attribute__((naked));
void PendSV_Handler(void) __attribute__((naked));
void SysTick_Handler(void);

#endif /* PORT_OSPORT_H_ */
