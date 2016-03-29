/**********************************************************************************
 * 硬件连接：-----------------
 *          |   PB - LED10     |
 *          |   PB - LED11     |
 *          |        
 *           ----------------- 

**********************************************************************************/
#include "led.h"

/*
 * 函数名：LED_GPIO_Config
 * 描述  ：配置LED用到的I/O口
 * 输入  ：无
 * 输出  ：无
 */
void LED_GPIO_Config(void)
{		
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启GPIOC的外设时钟*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 

	/*选择要控制的GPIOC引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;	

	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*调用库函数，初始化GPIOC*/
  	GPIO_Init(GPIOB, &GPIO_InitStructure);		  

	/* 关闭所有led灯	*/
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);	 
}
void LED_Init(void)
{

		for(;;)
		{ 
			LED1(ON);
			Delay(1000000);
			LED1(OFF);
			Delay(1000000);
		}
}

void LED_Toggle(void)
{
    
		
    //LED_t *pLED_Struct = &LED_Structs[ledx-1];
    //GPIO_TypeDef *GPIOx = (GPIO_TypeDef *)(AHB1PERIPH_BASE + 0x0400*pLED_Struct->GPIOx);
    // u16 pin_bit = 1<<pLED_Struct->pinx;
    
    //  if(ledx>4 || ledx<1) return;
    
    if((GPIOB->ODR & GPIO_Pin_10) == 0)
        GPIO_SetBits(GPIOB, GPIO_Pin_10);
    else
        GPIO_ResetBits(GPIOB, GPIO_Pin_10);   
}



/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
