#ifndef CAN_H

#include "stm32f10x_can.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void Can1_Init(void);
void Can_SendData(CanTxMsg* TxM);
void Can_RXMessage(uint8_t CAN_ID,uint8_t Data);

#define CAN_H
#endif