/*
 * Semaphore.h
 *
 *  Created on: DEC 13, 2023
 *      Author: Abdelfattah Med
 */

#ifndef KERNEL_SEMAPHORE_H_
#define KERNEL_SEMAPHORE_H_

#include "List.h"
#include "OSDataTypes.h"
#include "OSConfig.h"
#include "OSPort.h"
#if ENABLE_SEMAPHORES == 1
	OSSem* 	OSSemCreate(SemType semType, uint32_t initValue);
	Bool 	OSSemTake(OSSem* sem, uint32_t timeout);
	void 	OSSemGive(OSSem* sem);
#endif
#endif /* KERNEL_SEMAPHORE_H_ */
