/*
* @Author: obsidian
* @Date:   2016-03-19 14:28:15
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-19 16:56:54
*/
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

void gpio_init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}
void tim_init()
{
    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    TIM_ICInitTypeDef       TIM_ICInitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    TIM_InitStruct.TIM_ClockDivision = 0;
    TIM_InitStruct.TIM_Period = 0xffff;
    TIM_InitStruct.TIM_Prescaler = 0;
    TIM_TimeBaseInit(TIM4, &TIM_InitStruct);

    TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;
    TIM_ICInit(TIM4, &TIM_ICInitStruct);
    TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);

    NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
    NVIC_Init(&NVIC_InitStruct);
    TIM_Cmd(TIM4, ENABLE);
}

void TIM4_IRQ_Handler()
{
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
}

int main()
{
    gpio_init();
    tim_init();
    while(1)
    {

    }
    return 0;
}