/*
 * EventGroup.h
 *
 *  Created on: DEC 15, 2023
 *      Author: Abdelfattah Med
 */

#ifndef KERNEL_EVENTGROUP_H_
#define KERNEL_EVENTGROUP_H_

#include "List.h"
#include "OSDataTypes.h"
#include "OSConfig.h"
#include "OSPort.h"

#if ENABLE_EVENTGROUPS == 1

EventGroup*	EGCreate(void);
uint32_t	EGSetBits(EventGroup* eventGroup, uint32_t bits);
uint32_t	EGClearBits(EventGroup* eventGroup, uint32_t bits);
uint32_t	EGWaitBits(EventGroup* eventGroup,
		uint32_t bits,
		Bool autoClear,
		Bool allBitsWait,
		uint32_t timeout);


#endif

#endif /* KERNEL_EVENTGROUP_H_ */
