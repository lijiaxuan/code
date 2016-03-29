
#ifndef LED_H
#define LED_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "stm32f10x_rcc.h"

void LED_Configuration(void);

#define LED1_on GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define LED2_on GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define LED3_on GPIO_ResetBits(GPIOC,GPIO_Pin_13)
#define LED4_on GPIO_ResetBits(GPIOC,GPIO_Pin_14)

#define LED1_off GPIO_SetBits(GPIOB,GPIO_Pin_3)
#define LED2_off GPIO_SetBits(GPIOB,GPIO_Pin_4)
#define LED3_off GPIO_SetBits(GPIOC,GPIO_Pin_13)
#define LED4_off GPIO_SetBits(GPIOC,GPIO_Pin_14)



#endif

