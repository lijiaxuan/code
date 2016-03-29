/* 
* @Author: obsidian
* @Date:   2015-12-31 14:46:15
* @Last Modified by:   obsidian
* @Last Modified time: 2015-12-31 14:48:10
*/
#include "../inc/led.h"
void LED_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;        //¶¨ÒåGPIO³õÊ¼»¯½á¹¹Ìå

	//  ´ò¿ªGPIO A B C DµÄÊ±ÖÓ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	/* Configure PB.3,PB.4 as Output push-pull */ 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	/* Configure PC.13,PC.14 as Output push-pull */ 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 


}
