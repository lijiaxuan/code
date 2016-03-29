/* 
* @Author: obsidian
* @Date:   2016-01-03 17:16:30
* @Last Modified by:   obsidian
* @Last Modified time: 2016-02-23 17:25:47
*/
#include "../inc/bluetooth.h"
#include "../inc/bluetooth_global.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
void GPIO_config(void);
void USART_Config(u32 rate);
void init()
{
	GPIO_config();
    USART_Config(115200);
}
void send(unsigned char * pdata , unsigned char len)
{
	unsigned char index = 0;
//     if(bluetooth.status == 0)
//         {return;}
// 	if(pdata == 0)
// 	{return;}
	for(index = 0;index < len;index ++)
	{
		USART_SendData(USART3,*(pdata + index));
	}
	//for(index = 0; index < 200 ;index ++);
}
void send_head()
{
    if(bluetooth.status == 0)
        {return;}
	USART_SendData(USART3,0xff);
	USART_SendData(USART3,0xff);
}


/*
USART
 */
void GPIO_config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO,ENABLE);
  GPIO_PinRemapConfig(GPIO_FullRemap_USART3,ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC,&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC,&GPIO_InitStructure);
}
void USART_Config(u32 rate)
{
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
        
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);
    USART_DeInit(USART3);
    
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//ÇÀÕ¼ÓÅÏÈ¼¶3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //×ÓÓÅÏÈ¼¶3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQÍ¨µÀÊ¹ÄÜ
  NVIC_Init(&NVIC_InitStructure);   //¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷
    
    USART_InitStructure.USART_BaudRate = rate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êý¾Ý¸ñÊ½
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
    USART_InitStructure.USART_Parity = USART_Parity_No;//ÎÞÆæÅ¼Ð£ÑéÎ»
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎÞÓ²¼þÊý¾ÝÁ÷¿ØÖÆ
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //ÊÕ·¢Ä£Ê½

    USART_Init(USART3, &USART_InitStructure); //³õÊ¼»¯´®¿Ú
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//¿ªÆôÖÐ¶Ï
    USART_ClearFlag(USART3,USART_FLAG_TC);//USART1¿ÉÄÜÐèÒª·¢ËÍ£¬µ«USART3Ö»ÐèÒª½ÓÊÜÊÖ±úÊý¾Ý£¬²»ÓÃ·¢ËÍ
    USART_Cmd(USART3, ENABLE);                    //Ê¹ÄÜ´®¿Ú 
}
