/*
 * OSConfig.h
 *
 *  Created on: DEC 11, 2023
 *      Author: Abdelfattah Med
 */

#ifndef CONFIG_OSCONFIG_H_
#define CONFIG_OSCONFIG_H_

#include "stm32f4xx.h"

extern uint32_t SystemCoreClock;

#define		MAX_TASKS_NO		(10)				// includes IDLE Task
#define 	TICK_RATE			(1000)				// 1000 HZ = 1 ms, 100 HZ = 10 ms
#define 	IDLE_STACK_SIZE		(100) 				//
#define 	CPU_CLOCK			(SystemCoreClock)

#define		TICKS_IN_SEC		(TICK_RATE)

#define		ENABLE_SEMAPHORES	(1)
#define		MAX_SEM_NO			(5)

#define		ENABLE_EVENTGROUPS	(1)
#define		MAX_EVENTGROUP_NO	(5)

#define		ENABLE_MESSAGEQUEUE			(1)
#define		MAX_MESSAGEQUEUE_NO			(5)
#define		MAX_MESSAGEQUEUE_STORAGE	(500)


#endif /* CONFIG_OSCONFIG_H_ */
