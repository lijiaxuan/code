/* 
* @Author: obsidian
* @Date:   2015-12-15 19:59:08
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-30 08:40:48
*/

#include "../inc/steer.h"
#include "../inc/steer_global.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
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
	TIM_OCInitStructure .TIM_Pulse = MIDVALUE;// + 600 * t2 / 180 ;
	TIM_OC4Init(TIM4 ,&TIM_OCInitStructure );
	TIM_OC4PreloadConfig (TIM4,TIM_OCPreload_Enable );
	TIM_ARRPreloadConfig (TIM4,ENABLE );
	TIM_CtrlPWMOutputs(TIM4, ENABLE);
	TIM_Cmd(TIM4,ENABLE );
}

void steer_set_angle(float angle)
{
	if(steer.status == OFF)
		{return;}
	steer.steer_angle = angle;
	unsigned short pwm = 0;
	pwm = MIDVALUE + (int)(3.5 * angle);
	if(pwm > MIDVALUE + 155)
		{pwm = MIDVALUE + 155;}
	else if(pwm < MIDVALUE - 167)
		{pwm = MIDVALUE - 167;}
	*steer.CCRx = pwm;				//PWM4 
}
