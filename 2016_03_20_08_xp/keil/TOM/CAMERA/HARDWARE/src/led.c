#include "led.h"
#include "stm32f4xx_it.h"

void LED_Init(void)
{
	//PE2 PE4
	
	
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOI, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
// 	GPIO_Init(GPIOI, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOE, GPIO_Pin_2);  //LED2,3
 	GPIO_SetBits(GPIOE, GPIO_Pin_4);  //LED4


}

