#ifndef GPS_DRIVER_H

#define     COUNTER_PER_ROTATION            512 * 4

extern unsigned char USART1_RxBuffer[8];

#include "stm32f10x_dma.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "math.h"



void Encoder_Timer_Config();

void UART1_DMA_init();

#define GPS_DRIVER_H
#endif