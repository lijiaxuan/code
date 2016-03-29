#include "usart.h"
#include <stdio.h>

void COM1Init(u32 BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);  
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	USART_InitStructure.USART_BaudRate = BaudRate;//≤®Ãÿ¬ …Ë÷√
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
	USART_Init(USART1, &USART_InitStructure);
	USART_ClockStructInit(&USART_ClockInitStruct);  
	USART_ClockInit(USART1, &USART_ClockInitStruct);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TC|USART_FLAG_RXNE);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;    //must be set
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void Com1_Puts(char * str)
{
	while (*str)
	{
		USART_SendData(USART1, *str++);

		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
	}
}
	char cmd;
void USART1_IRQHandler(void)
{

	extern int FlgTransmit;
	
	if (USART_GetITStatus(USART1, USART_IT_TXE) == SET)
		USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		cmd=USART_ReceiveData(USART1);
		FlgTransmit=1;
 
	}
}

