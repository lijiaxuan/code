#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "../inc/pwm.h"
void TIM2_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_Initstructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2  , ENABLE);
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE );
	GPIO_Initstructure . GPIO_Pin =GPIO_Pin_2| GPIO_Pin_3 ;
	GPIO_Initstructure .GPIO_Mode =GPIO_Mode_AF_PP ;
	GPIO_Initstructure .GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init (GPIOA,&GPIO_Initstructure );
}

void TIM2_Mode_Config(int t1,int t2 )
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_TimeBaseStructure .TIM_Period =7999;
	TIM_TimeBaseStructure .TIM_Prescaler =179;
	TIM_TimeBaseStructure .TIM_ClockDivision =TIM_CKD_DIV1  ;
	TIM_TimeBaseStructure .TIM_CounterMode =TIM_CounterMode_Up ;
	TIM_TimeBaseInit (TIM2 ,&TIM_TimeBaseStructure );
	
	TIM_OCInitStructure .TIM_OCMode = TIM_OCMode_PWM1 ;
	TIM_OCInitStructure .TIM_OutputState = TIM_OutputState_Enable ;
	TIM_OCInitStructure .TIM_Pulse = 400 + 600 * t1 / 180 ;
	TIM_OCInitStructure .TIM_OCPolarity = TIM_OCPolarity_High ;
	TIM_OC3Init(TIM2 ,&TIM_OCInitStructure );
	TIM_OC3PreloadConfig (TIM2,TIM_OCPreload_Enable );
	
	TIM_OCInitStructure .TIM_OutputState = TIM_OutputState_Enable ;
	TIM_OCInitStructure .TIM_Pulse = 400 + 600 * t2 / 180 ;
	TIM_OC4Init(TIM2 ,&TIM_OCInitStructure );
	TIM_OC4PreloadConfig (TIM2,TIM_OCPreload_Enable );
	TIM_ARRPreloadConfig (TIM2,ENABLE );
	TIM2_GPIO_Config();
	TIM_Cmd(TIM2,ENABLE );
}

// void NVIC_Config()
// {
// 	NVIC_InitTypeDef NVIC_InitStructure;
//     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
// 	
// 	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; 
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
// 	NVIC_Init(&NVIC_InitStructure); 
// }
// void EXTI_Configuration(void)
// {

// 	EXTI_InitTypeDef EXTI_InitStructure;
// 	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);	
// 	EXTI_InitStructure.EXTI_Line=EXTI_Line12;
// 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
// 	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
// 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
// 	EXTI_Init(&EXTI_InitStructure);
// 	
// //  	 GPIO_SetBits(GPIOC ,GPIO_Pin_6);

// }

//  void EXTI15_10IRQHandler(void)  
// {      
//   
//   if(EXTI_GetITStatus(EXTI_Line12)==SET)  
//   {     
//      GPIO_WriteBit(GPIOA,GPIO_Pin_12,(BitAction)(Bit_SET-GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_12)));  
//      
//   }   
//   delay_ms(100);//ÏÂ½µÑØÏú¶¶  
//     
//   while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)!=Bit_SET);  
//     
//   CCR1_Val+=200;

//   Test_MOTO (CCR1_Val);
//   EXTI_ClearITPendingBit(EXTI_Line12);  
// } 

// void  Test_MOTO( double a1,double  a2)
// {
//   TIM_OCInitStructure.TIM_Pulse = 400+600*a1/180;
// 	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
// 	TIM_OCInitStructure.TIM_Pulse = 400+600*a2/180;
// 	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
// }
