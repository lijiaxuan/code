
#include "stm32f4xx.h"


void System_Clock_Init(void)
{
	u32 		PLL_M;
	u32		PLL_N;
	u32		PLL_P;
	u32		PLL_Q;
	
	RCC_DeInit();      /*初始化*/
	RCC_HSEConfig(RCC_HSE_ON);      /*选择外部时钟*/
	while(RCC_WaitForHSEStartUp() != SUCCESS);
	
	PLL_M = 8;          //PLL_VCO input clock   1MHz  //
	PLL_N = 336;		//PLL_VCO output clock  336MHz//
	PLL_P = 2;			//PLLCLK                168MHz//
	PLL_Q = 7;			//SD USB Clock          48MHz //
	
	RCC_PLLConfig(RCC_PLLSource_HSE, PLL_M, PLL_N, PLL_P, PLL_Q);
	RCC_PLLCmd(ENABLE);
	
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);  //SystemClock = 168MHz    //
	
	RCC_HCLKConfig(RCC_SYSCLK_Div1);            //AHB(HCLK) Clock = SYSCLK//
	
	RCC_PCLK1Config(RCC_HCLK_Div4);				//APB1 clock = HCLK/4     //
	
	RCC_PCLK2Config(RCC_HCLK_Div2);				//APB2 clock = HCLK/2     //
		
}

