/*
* @Author: obsidian
* @Date:   2016-02-29 21:30:35
* @Last Modified by:   obsidian
* @Last Modified time: 2016-02-29 22:41:50
*/
#include "steer_driver.h"
#include "steer_global.h"

void TIM4_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_Initstructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4  , ENABLE);
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE );
    GPIO_Initstructure . GPIO_Pin =GPIO_Pin_8| GPIO_Pin_9 ;
    GPIO_Initstructure .GPIO_Mode =GPIO_Mode_AF_PP ;
    GPIO_Initstructure .GPIO_Speed =GPIO_Speed_50MHz;
    GPIO_Init (GPIOB,&GPIO_Initstructure );
}

void TIM4_Mode_Config(void)
{
    TIM4_GPIO_Config();
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_TimeBaseStructure .TIM_Period =1000000 / 50;
    TIM_TimeBaseStructure .TIM_Prescaler =72 - 1;
    TIM_TimeBaseStructure .TIM_ClockDivision =TIM_CKD_DIV1  ;
    TIM_TimeBaseStructure .TIM_CounterMode =TIM_CounterMode_Up ;
    TIM_TimeBaseInit (TIM4 ,&TIM_TimeBaseStructure );
    
    TIM_OCInitStructure .TIM_OCMode = TIM_OCMode_PWM1 ;
    TIM_OCInitStructure .TIM_OCPolarity = TIM_OCPolarity_High ;
    TIM_OCInitStructure .TIM_OutputState = TIM_OutputState_Enable ;
    TIM_OCInitStructure .TIM_Pulse = steer.midvalue;// + 600 * t2 / 180 ;
    TIM_OC4Init(TIM4 ,&TIM_OCInitStructure );
    TIM_OC4PreloadConfig (TIM4,TIM_OCPreload_Enable );
    TIM_ARRPreloadConfig (TIM4,ENABLE );
    TIM_CtrlPWMOutputs(TIM4, ENABLE);
    TIM_Cmd(TIM4,ENABLE );
}
