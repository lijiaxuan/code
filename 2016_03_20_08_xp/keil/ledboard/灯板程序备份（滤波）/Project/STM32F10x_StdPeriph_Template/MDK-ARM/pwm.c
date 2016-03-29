#include "stm32f10x.h"
#include"all.h"

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
uint16_t TimerPeriod = 0;
uint16_t Channel1Pulse = 0;

void pwmint(void)
	{
	 GPIO_InitTypeDef GPIO_InitStructure;
	//RCC CONFIG
		/* TIM1, GPIOA, GPIOB, GPIOE and AFIO clocks enable */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 |RCC_APB2Periph_GPIOA |RCC_APB2Periph_AFIO, ENABLE);

	//gpio config
		 GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);
	 /* GPIOA Configuration: Channel 1, 2 and 3 as alternate function push-pull */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

		/* GPIOB Configuration: Channel 1N, 2N and 3N as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		//∂® ±∆˜1…Ë÷√ 
		 /* Compute the value to be set in ARR regiter to generate signal frequency at 2.5 Khz */
		TimerPeriod = (SystemCoreClock / 2500 ) - 1;
		/* Compute CCR1 value to generate a duty cycle at 50% for channel 1 and 1N */
		Channel1Pulse = (uint16_t) (((uint32_t)5* (TimerPeriod - 1)) / 10);

		/* Time Base configuration */
		TIM_TimeBaseStructure.TIM_Prescaler = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

		/* Channel 1 */
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
		TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

		TIM_OC1Init(TIM1, &TIM_OCInitStructure);

		/* TIM1 counter enable */
		TIM_Cmd(TIM1, ENABLE);

		/* TIM1 Main Output Enable */
		TIM_CtrlPWMOutputs(TIM1, ENABLE);
	}
