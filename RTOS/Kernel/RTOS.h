/*
 * Task.h
 *
 *  Created on: DEC 11, 2023
 *      Author: Abdelfattah Med
 */

#ifndef KERNEL_RTOS_H_
#define KERNEL_RTOS_H_

#include "stm32f4xx.h"
#include "List.h"
#include "OSDataTypes.h"
#include "OSConfig.h"
#include "OSPort.h"

void user_error(void);
void T_IDLE(void);

void OSCreateTask(void (*pfTask)(void), uint32_t task_pri,
		uint32_t* task_stack_arr, uint32_t task_stack_size);
void OSDelay(uint32_t waitTicks);
void OSStart(void);

#endif /* KERNEL_RTOS_H_ */
