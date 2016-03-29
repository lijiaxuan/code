#ifndef __KEY_H
#define __KEY_H
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"


#define KEY_OUT_UP  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键0
#define KEY_OUT_FALL  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)//读取按键1
#define KEY_OUT_LEFT  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//读取按键2
#define KEY_OUT_RIGHT  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)//读取按键3
#define KEY_OUT_CONFIRM  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9)//读取按键4
#define KEY_OUT_CANCEL  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10)//读取按键5


#define KEY_UP 											1
#define KEY_FALL  									2
#define KEY_LEFT										3
#define KEY_RIGHT 									4
#define KEY_CONFIRM 								5
#define KEY_CANCEL 									6

void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8);  	//按键扫描函数	


#endif
