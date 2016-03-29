/* 
* @Author: obsidian
* @Date:   2015-12-15 19:59:08
* @Last Modified by:   obsidian
* @Last Modified time: 2016-02-25 15:45:53
*/

#include "../inc/brake.h"
#include "../inc/brake_global.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#define BRAKE_MIDVALUE 0x560

void TIM43_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_Initstructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4  , ENABLE);
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE );
	GPIO_Initstructure . GPIO_Pin =GPIO_Pin_8| GPIO_Pin_9 ;
	GPIO_Initstructure .GPIO_Mode =GPIO_Mode_AF_PP ;
	GPIO_Initstructure .GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init (GPIOB,&GPIO_Initstructure );
}

void TIM43_Mode_Config(void)
{
	//TIM4_GPIO_Config();
	//TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	// TIM_TimeBaseStructure .TIM_Period =1000000 / 50;
	// TIM_TimeBaseStructure .TIM_Prescaler =72 - 1;
	// TIM_TimeBaseStructure .TIM_ClockDivision =TIM_CKD_DIV1  ;
	// TIM_TimeBaseStructure .TIM_CounterMode =TIM_CounterMode_Up ;
	// TIM_TimeBaseInit (TIM4 ,&TIM_TimeBaseStructure );
	TIM_Cmd(TIM4,DISABLE );
	TIM_OCInitStructure .TIM_OCMode = TIM_OCMode_PWM1 ;
	TIM_OCInitStructure .TIM_OCPolarity = TIM_OCPolarity_High ;
	TIM_OCInitStructure .TIM_OutputState = TIM_OutputState_Enable ;
	TIM_OCInitStructure .TIM_Pulse = BRAKE_MIDVALUE;// + 600 * t2 / 180 ;
	TIM_OC3Init(TIM4 ,&TIM_OCInitStructure );
	TIM_OC3PreloadConfig (TIM4,TIM_OCPreload_Enable );
	TIM_ARRPreloadConfig (TIM4,ENABLE );
	TIM_CtrlPWMOutputs(TIM4, ENABLE);
	TIM_Cmd(TIM4,ENABLE );
}

void brake_set_angle(float angle)
{
	if(brake.status == OFF)
		{return;}
	brake.brake_angle = angle;
// 	unsigned short pwm = 0;
// 	pwm = MIDVALUE + (int)(eff * angle);
// 	if(pwm > MIDVALUE + SCALE)
// 		{pwm = MIDVALUE + SCALE;}
// 	else if(pwm < MIDVALUE - SCALE)
// 		{pwm = MIDVALUE - SCALE;}
//	*brake.CCRx = pwm;				//PWM4 
}

void brake_timer_cfg(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 , ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 1440 - 1 ; 
	TIM_TimeBaseStructure.TIM_Period = 50000; // 5000 - 5ms
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM6,TIM_IT_Update);
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	
	//TIM_Cmd(TIM6,ENABLE);

}

static char count = 0;
void TIM6_IRQHandler(void)
{
	TIM_ClearFlag(TIM6,TIM_IT_Update);
	count ++;
	if(count != brake.sec)
	{
		return;
	}
	TIM4->CCR3 = 0x560;
	brake.stopflag = 0;
	TIM_Cmd(TIM6,DISABLE);
}
