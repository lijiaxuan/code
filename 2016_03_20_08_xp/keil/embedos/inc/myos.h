/* 
* @Author: obsidian
* @Date:   2015-12-30 17:26:28
* @Last Modified by:   obsidian
* @Last Modified time: 2015-12-30 17:39:43
*/
#ifndef MYOS_H
#include "stm32f10x.h"

#define TASK_TIME_SLICE		5
typedef unsigned int 		OS_STK;



//assembling functions
void OS_ENTER_CRITICAL(void);
void OS_EXIT_CRITICAL(void);
void OSCtxSw(void);
void OSStart(void);

OS_STK*	OSTaskStkInit(void 			(*task)(void *p_arg),
						void 		*p_arg,
						OS_STK 		*p_tos);

//cpu
#define OS_MAX_TASKS 		16
#define TASK_STATE_CREATING 0
#define TASK_STATE_RUNNING  1
#define TASK_STATE_PAUSING  2
#define TASK_STACK_SIZE		64
#define LED1TURN() (GPIOA->ODR ^= 1<<8)  // reverse the voltage of LED1 !!!HARDWARE RELATED
#define LED2TURN() (GPIOD->ODR ^= 1<<2)  // reverse the voltage of LED2 !!!HARDWARE RELATED
struct OS_TCB
{
	OS_STK*			OSTCBStkPtr;	//stack pointer
	unsigned char	OSTCBStat;		//OS Task Status

};

void OSInit(void);
void LedInit(void);

void OS_TaskIdle(void *p_arg);
void OSInitTaskIdle(void);
void OSTaskCreate(	void 	(*task)(void *p_arg),
					void*  	p_arg,
					OS_STK 	*ptos);
void OSTCBSet(struct OS_TCB 	*p_tcb,
			OS_STK 			*p_tos,
			unsigned char 	task_state);
void SysTickInit(unsigned char Nms);
void SysTick_Handler(void);

unsigned int GetTime(void);
void delayMs();

#define MYOS_H
#endif
