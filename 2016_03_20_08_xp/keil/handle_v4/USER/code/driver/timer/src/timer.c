#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "../inc/timer.h"
#include "../../../module/GPS/inc/GPS_global.h"

// void Encoder_Timer_Config()
// {
// 	NVIC_InitTypeDef NVIC_InitStructure;
// 	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
// 	TIM_ICInitTypeDef TIM_ICInitStructure;
// 	GPIO_InitTypeDef GPIO_InitStructure;
// 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
// 	
// 	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
// 	NVIC_Init(&NVIC_InitStructure); 
// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
// 	
// 	//config GPIO
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);
// 	/* configue timer */	
// 	TIM_DeInit(TIM3);		
// 	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
// 	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 
// 	TIM_TimeBaseStructure.TIM_Period = encoder_property[0].count_per_rotation - 1;
// 	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
// 	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
// 	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
// 	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

// 	TIM_ICStructInit(&TIM_ICInitStructure);
// 	TIM_ICInitStructure.TIM_ICFilter = (u8)0;
// 	
// 	TIM_ICInit(TIM3, &TIM_ICInitStructure);

// 	TIM_ClearFlag(TIM3, TIM_FLAG_Update);	
// 	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
// 	
// 	/* reset the timer3 counter */
// 	TIM3->CNT = (u16)0;
// 	/* enable the timer3 */
// 	TIM_Cmd(TIM3, ENABLE);
// 	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2;
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);

// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
// 	
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);

// 	TIM_DeInit(TIM4);		
// 	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
// 	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 
// 	TIM_TimeBaseStructure.TIM_Period = encoder_property[1].count_per_rotation - 1; 			
// 	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
// 	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
// 	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
// 	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
// 	
// 	
// 	TIM_ICStructInit(&TIM_ICInitStructure);
// 	TIM_ICInitStructure.TIM_ICFilter = (u8)0;
// 	
// 	TIM_ICInit(TIM4, &TIM_ICInitStructure);

// 	TIM_ClearFlag(TIM4, TIM_FLAG_Update);	
// 	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
// 	
// 	/* reset the timer3 counter */
// 	TIM4->CNT = (u16)0;
// 	/* enable the timer3 */
// 	TIM_Cmd(TIM4, ENABLE);

// 	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2;
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);
// }

void Driver_Timer_Config()
{

}

// void TIM3_IRQHandler(void)
// {
// 	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
//     if(!(TIM3->CR1&0x0010))
//     {/* count up overflow interrupt */
//         gps.encoder[0].num_of_rotation++;
//     }
//     else
//     {/* count down overflow interrupt */
//         gps.encoder[0].num_of_rotation--;
//     }
// }
// void TIM4_IRQHandler(void)
// {
// 	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
//     if(!(TIM4->CR1&0x0010))
//     {/* count up overflow interrupt */
//         gps.encoder[1].num_of_rotation++;
//     }
//     else
//     {/* count down overflow interrupt */
//         gps.encoder[1].num_of_rotation--;
//     }
// }
