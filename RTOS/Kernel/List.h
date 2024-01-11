/*
 * List.h
 *
 *  Created on: DEC 11, 2023
 *      Author: Abdelfattah Med
 */

#ifndef KERNEL_LIST_H_
#define KERNEL_LIST_H_

#include "OSDataTypes.h"

void insertTCB(TCB** pList, TCB* pTCB,
		OrderType orderType, OrderDir orderDir);
TCB* extractHead(TCB** pList);
void deleteTCB(TCB** pList, TCB* pTCB);


void E_insertTCB(TCB** pList, TCB* pTCB,
		OrderType orderType, OrderDir orderDir);
TCB* E_extractHead(TCB** pList);
void E_deleteTCB(TCB** pList, TCB* pTCB);

void insertEnd(Message** list,Message* msg,Message** last,Message** preLast);
Message* getListHead(Message** list,Message** last,Message** preLast);

#endif /* KERNEL_LIST_H_ */
