/**********************************************************************************
 * Ӳ�����ӣ�-----------------
 *          |   PB - LED10     |
 *          |   PB - LED11     |
 *          |        
 *           ----------------- 

**********************************************************************************/
#include "led.h"

/*
 * ��������LED_GPIO_Config
 * ����  ������LED�õ���I/O��
 * ����  ����
 * ���  ����
 */
void LED_GPIO_Config(void)
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*����GPIOC������ʱ��*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 

	/*ѡ��Ҫ���Ƶ�GPIOC����*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;	

	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*���ÿ⺯������ʼ��GPIOC*/
  	GPIO_Init(GPIOB, &GPIO_InitStructure);		  

	/* �ر�����led��	*/
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
