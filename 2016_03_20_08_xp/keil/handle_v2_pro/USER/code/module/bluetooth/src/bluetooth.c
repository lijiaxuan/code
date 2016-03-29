/* 
* @Author: obsidian
* @Date:   2016-01-03 17:16:30
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-24 16:57:48
*/
#include "../inc/bluetooth.h"
#include "../inc/bluetooth_global.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
void bluetooth_uart_init(u32 bound);
void init()
{
	bluetooth_uart_init(115200);
}
void send(unsigned char * pdata , unsigned char len)
{
	unsigned char index = 0;
	if(pdata == 0)
	{return;}
	for(index = 0;index < len;index ++)
	{
		USART_SendData(USART3,*(pdata + index));
	}
	for(index = 0; index < 200 ;index ++);
}
void send_head()
{
	USART_SendData(USART3,0xff);
	USART_SendData(USART3,0xff);
}


/*
USART
 */
void bluetooth_uart_init(u32 bound)
{

    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//Ê¹ÄÜUSART1£¬GPIOAÊ±ÖÓ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	//USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
		//USART3_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure); 


   	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
   	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
   	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_Init(&NVIC_InitStructure);


	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	USART_Init(USART3, &USART_InitStructure); 
		
	USART_Cmd(USART3, ENABLE);     
         
}
