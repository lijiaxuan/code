/*
* @Author: obsidian
* @Date:   2016-02-29 21:28:20
* @Last Modified by:   obsidian
* @Last Modified time: 2016-02-29 21:43:54
*/

#include "proximity_driver.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"


void SA_Config()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin= GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12 ;       
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; 
    GPIO_Init(GPIOC,&GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin= GPIO_Pin_15 ;       
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; 
    GPIO_Init(GPIOA,&GPIO_InitStructure);
}
