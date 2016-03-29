#ifndef GYRO_H
#define GYRO_H

#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "led.h"
#include "systick.h"
#include "math.h"

#define RxBufferSize 8
#define TxBufferSize 8

struct Gyro_Struct
{
	int32_t  value;
	int32_t  count;
};
	
//file gyro.c
extern double Gyro_Angle_Total;
extern uint8_t Gyro_Error_Flag;
extern double Gyro_Convert1;    //�����泵��תϵ��
extern double Gyro_Convert2;    //�����泵��תϵ��

void Gyro_CheckFloat(void);
void Gyro_Clear(void);

//file gyroConfig.c
void UART1_DMA_init(u32 baud);
void UART1_DMA_EN(void);


#endif

