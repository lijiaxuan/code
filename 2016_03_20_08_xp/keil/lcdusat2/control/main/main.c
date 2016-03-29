/*
* @Author: obsidian
* @Date:   2016-03-01 20:39:05
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-12 17:15:28
*/
#include "stm32f10x.h"
#include "module/lcd/lcd_global.h"
#include "driver/delay/inc/delay.h"
#include "driver/sd/file.h"
#include "module/lcd/lcd_driver.h"
#include "module/lcd/menu/menu_pid.h"
#include "module/steer/steer_global.h"
#include "module/brake/brake_global.h"
#include "module/GPS/GPS_global.h"
#include "module/pid/pid_global.h"
#include "usart.h"

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

static unsigned short step;

void begin()
{
    TIM_TimeBaseInitTypeDef TimerInitStruct;
    NVIC_InitTypeDef        NVICInitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD |RCC_APB2Periph_GPIOE | RCC_APB1Periph_TIM4, ENABLE);
    TimerInitStruct.TIM_ClockDivision = 0;
    TimerInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TimerInitStruct.TIM_Period = 10000;
    TimerInitStruct.TIM_Prescaler = 720 - 1;
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    TIM_TimeBaseInit(TIM4, &TimerInitStruct);
    TIM_Cmd(TIM4, ENABLE);
    NVICInitStruct.NVIC_IRQChannel = TIM4_IRQn;
    NVICInitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 3;
    NVICInitStruct.NVIC_IRQChannelSubPriority = 4;
    NVIC_Init(&NVICInitStruct);
    NVICInitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVICInitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 3;
    NVICInitStruct.NVIC_IRQChannelSubPriority = 4;
    NVIC_Init(&NVICInitStruct);
}

int main()
{
    GPIO_InitTypeDef GPIOtypeInitStruct;
    GPIOtypeInitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIOtypeInitStruct.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
    GPIOtypeInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIOtypeInitStruct);
    EXTI_InitTypeDef EXTItypeStruct;
    EXTItypeStruct.EXTI_Line = EXTI_Line14;
    EXTItypeStruct.EXTI_LineCmd = ENABLE;
    EXTItypeStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTItypeStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTItypeStruct);
    while(1)
    {
        if(!GPIOD->IDR & (1 << 14))
        {
            step += 5;
        }
        if(!GPIOD->IDR & (1 << 15))
        {
            if(step > 5)
                {step -= 5;}
        }
    }
}


static unsigned short count = 0;

void next()
{
    static char ledindex = 0;
    unsigned char tmp;
    ledindex ++;
    if(ledindex > 15)
        {ledindex = 0;}
    if(ledindex >= 7)
    {
        tmp = ledindex;
    }
    else
    {
        tmp = 16 - ledindex;
    }
    GPIOE->ODR &= (1 << tmp);
}

void Timer4_IRQHandler()
{
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    count ++;
    if(count >= step)
    {
        count = 0;
        next();
    }

}
