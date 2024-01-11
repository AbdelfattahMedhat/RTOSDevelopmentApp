/*
 * List.c
 *
 *  Created on: DEC 11, 2023
 *      Author: Abdelfattah Med
 */

#include "List.h"

void insertTCB(TCB** pList, TCB* pTCB,
		OrderType orderType, OrderDir orderDir){
	TCB* currPtr = (*pList);

	if((*pList) == NULL){
		(*pList) = pTCB;
		pTCB->next = NULL;
		pTCB->prev = NULL;
	}
	else{
		if(orderType == OT_PRI && orderDir == OD_DESC){
			if(pTCB->taskPri > (*pList)->taskPri){
				pTCB->next = (*pList);
				(*pList)->prev = pTCB;
				(*pList) = pTCB;
				pTCB->prev = NULL;
			}
			else{
				// search for a node that next has pr < pTCB pr
				while(currPtr->next != NULL && currPtr->next->taskPri > pTCB->taskPri){
					currPtr = currPtr->next;
				}
				pTCB->next = currPtr->next;
				pTCB->prev = currPtr;
				currPtr->next = pTCB;
				if(pTCB->next != NULL)
				{
					pTCB->next->prev = pTCB;
				}
			}

		}else if(orderType == OT_TICKS && orderDir == OD_ASC){
			if(pTCB->waitTicks < (*pList)->waitTicks){
				pTCB->next = (*pList);
				(*pList)->prev = pTCB;
				(*pList) = pTCB;
				pTCB->prev = NULL;
			}
			else{
				// search for a node that next has pr < pTCB pr
				while(currPtr->next != NULL && currPtr->next->waitTicks < pTCB->waitTicks){
					currPtr = currPtr->next;
				}
				pTCB->next = currPtr->next;
				pTCB->prev = currPtr;
				currPtr->next = pTCB;
				if(pTCB->next != NULL)
				{
					pTCB->next->prev = pTCB;
				}
			}
		}
	}

}
TCB* extractHead(TCB** pList){
	TCB* head = (*pList);
	if((*pList) != NULL){
		(*pList) 		= (*pList)->next;
		(*pList)->prev 	= NULL;
		head->next 		= NULL;
		head->prev 		= NULL;
	}
	return head;
}
void deleteTCB(TCB** pList, TCB* pTCB){
	if(pList != NULL && (*pList) != NULL && pTCB != NULL){
		// single TCB
		if(pTCB->next == NULL && pTCB->prev == NULL){
			(*pList) = NULL;
		}
		// not first nor last TCB
		else if(pTCB->next != NULL && pTCB->prev != NULL){
			pTCB->prev->next = pTCB->next;
			pTCB->next->prev = pTCB->prev;
			pTCB->next = NULL;
			pTCB->prev = NULL;
		}
		//  first
		else if(pTCB->next != NULL && pTCB->prev == NULL){
			(*pList) = pTCB->next;
			pTCB->next->prev = NULL;
			pTCB->next = NULL;
		}
		// last
		else{
			pTCB->prev->next = NULL;
			pTCB->prev = NULL;
		}
	}
}
void E_insertTCB(TCB** pList, TCB* pTCB,
		OrderType orderType, OrderDir orderDir){
	TCB* currPtr = (*pList);

	if((*pList) == NULL){
		(*pList) = pTCB;
		pTCB->enext = NULL;
		pTCB->eprev = NULL;
	}
	else{
		if(orderType == OT_PRI && orderDir == OD_DESC){
			if(pTCB->taskPri > (*pList)->taskPri){
				pTCB->enext = (*pList);
				(*pList)->eprev = pTCB;
				(*pList) = pTCB;
				pTCB->eprev = NULL;
			}
			else{
				// search for a node that next has pr < pTCB pr
				while(currPtr->enext != NULL && currPtr->enext->taskPri > pTCB->taskPri){
					currPtr = currPtr->enext;
				}
				pTCB->enext = currPtr->enext;
				pTCB->eprev = currPtr;
				currPtr->enext = pTCB;
				if(pTCB->enext != NULL)
				{
					pTCB->enext->eprev = pTCB;
				}
			}

		}else if(orderType == OT_TICKS && orderDir == OD_ASC){
			if(pTCB->waitTicks < (*pList)->waitTicks){
				pTCB->enext = (*pList);
				(*pList)->eprev = pTCB;
				(*pList) = pTCB;
				pTCB->eprev = NULL;
			}
			else{
				// search for a node that next has pr < pTCB pr
				while(currPtr->enext != NULL && currPtr->next->waitTicks < pTCB->waitTicks){
					currPtr = currPtr->enext;
				}
				pTCB->enext = currPtr->enext;
				pTCB->eprev = currPtr;
				currPtr->enext = pTCB;
				if(pTCB->enext != NULL)
				{
					pTCB->enext->eprev = pTCB;
				}
			}
		}
	}

}
TCB* E_extractHead(TCB** pList){
	TCB* head = (*pList);
	if((*pList) != NULL){
		(*pList) 			= (*pList)->enext;
		(*pList)->eprev 	= NULL;
		head->enext 		= NULL;
		head->eprev 		= NULL;
	}
	return head;
}
void E_deleteTCB(TCB** pList, TCB* pTCB){
	if(pList != NULL && (*pList) != NULL && pTCB != NULL){
		// single TCB
		if(pTCB->enext == NULL && pTCB->eprev == NULL){
			(*pList) = NULL;
		}
		// not first nor last TCB
		else if(pTCB->enext != NULL && pTCB->eprev != NULL){
			pTCB->eprev->enext = pTCB->enext;
			pTCB->enext->eprev = pTCB->eprev;
			pTCB->enext = NULL;
			pTCB->eprev = NULL;
		}
		//  first
		else if(pTCB->enext != NULL && pTCB->eprev == NULL){
			(*pList) = pTCB->enext;
			pTCB->enext->eprev = NULL;
			pTCB->enext = NULL;
		}
		// last
		else{
			pTCB->eprev->enext = NULL;
			pTCB->eprev = NULL;
		}
	}
}
void insertEnd(Message** list,Message* msg,Message** last,Message** preLast){
	if ((*list) == NULL) {
		(*list) 	= msg;
		(*list)->nextMSG 	= NULL;
		(*last) 	= msg;
		(*last)->nextMSG 	= NULL;
		(*preLast) 	= msg;
		(*preLast)->nextMSG 	= NULL;
	}
	else {
		(*preLast) 	= (*last);
		(*last) 	= msg;
		(*preLast)->nextMSG 	= (*last);
		(*last)->nextMSG = NULL;
	}
}
Message* getListHead(Message** list,Message** last,Message** preLast){
	Message* head = (*list);
	if((*list) != NULL){
		if ((*list) == (*last)) {
			(*list) 	= NULL;
			(*last) 	= NULL;
			(*preLast) 	= NULL;
		} else if ((*list) == (*preLast)){
			(*list) 	= (*last);
			(*preLast) 	= (*last);
		}
		else{
			(*list) = (*list)->nextMSG;
		}
	}
	return head;
}
