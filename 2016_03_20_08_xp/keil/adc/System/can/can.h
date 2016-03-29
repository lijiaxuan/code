#ifndef __CAN_H
#define __CAN_H
#include "stm32f10x.h"
#include "gpio.h"
#include "nvic.h"

void CAN_Config(void);
void CAN_Mode_Config(void);
void CAN_Filter_Config(void);
void CAN_SetMsg(void);

extern CanTxMsg TxMessage;
extern CanRxMsg RxMessage;
extern u32 flag;

#endif
