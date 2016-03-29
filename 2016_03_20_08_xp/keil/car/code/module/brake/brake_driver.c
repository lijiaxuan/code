/*
* @Author: obsidian
* @Date:   2016-02-29 21:25:53
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-01 09:01:29
*/
#include "brake_driver.h"
#include "brake_global.h"
void TIM43_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_Initstructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4  , ENABLE);
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE );
    GPIO_Initstructure . GPIO_Pin           =GPIO_Pin_8| GPIO_Pin_9 ;
    GPIO_Initstructure .GPIO_Mode           =GPIO_Mode_AF_PP ;
    GPIO_Initstructure .GPIO_Speed          =GPIO_Speed_50MHz;
    GPIO_Init (GPIOB,&GPIO_Initstructure );
}

void TIM43_Mode_Config(void)
{
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    TIM_Cmd(TIM4,DISABLE );
    TIM_OCInitStructure .TIM_OCMode         =   TIM_OCMode_PWM1 ;
    TIM_OCInitStructure .TIM_OCPolarity     =   TIM_OCPolarity_High ;
    TIM_OCInitStructure .TIM_OutputState    =   TIM_OutputState_Enable ;
    TIM_OCInitStructure .TIM_Pulse          =   brake.midvalue;
    TIM_OC3Init(TIM4 ,&TIM_OCInitStructure );
    TIM_OC3PreloadConfig (TIM4,TIM_OCPreload_Enable );
    TIM_ARRPreloadConfig (TIM4,ENABLE );
    TIM_CtrlPWMOutputs(TIM4, ENABLE);
    TIM_Cmd(TIM4,ENABLE );
}
