#include "stm32f10x.h"
#include "platform_config.h"
#include"all.h"
#include <stdio.h>
#define BaudRate 230400 //230400   115200
USART_InitTypeDef USART_InitStructure;

void Usartint(void)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		/* Configure the NVIC Preemption Priority Bits */  
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
		
		/* Enable the USARTy Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	//RCC CONFIG
	#ifndef USE_STM3210C_EVAL
		/* Enable USARTy Clock */
		RCC_APB2PeriphClockCmd(USARTy_CLK, ENABLE); 
	#else
		/* Enable USARTy Clock */
		RCC_APB1PeriphClockCmd(USARTy_CLK, ENABLE); 
	#endif
	 
			 /* Enable GPIO clock */
		RCC_APB2PeriphClockCmd(USARTy_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE); 

	//GPIO CONFIG

		/* Configure USARTy Rx as input floating */
		GPIO_InitStructure.GPIO_Pin = USARTy_RxPin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);
		
		/* Configure USARTy Tx as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = USARTy_TxPin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);		

		USART_InitStructure.USART_BaudRate = BaudRate;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		
		/* Configure USARTy */
		USART_Init(USARTy, &USART_InitStructure);
		USART_ITConfig(USARTy,USART_IT_RXNE ,ENABLE); 
		/* Configure USARTz */
	 
		/* Enable the USARTy */
		USART_Cmd(USARTy, ENABLE);


	}

void Usend(uint16_t TxBuffer)
 {
   	TxBuffer=(uint16_t)((double)TxBuffer/(double)0xfff*3300);
    tempq=TxBuffer/1000;
	  tempq=tempq+0x30;
    TxBuffer%=1000;
	  tempb=	TxBuffer/100;
	  tempb=tempb+0x30;
		TxBuffer%=100;
	  temps=	TxBuffer/10;
	  temps=temps+0x30;
	  TxBuffer%=10;
	  tempg=	TxBuffer/1;
	  tempg=tempg+0x30;
    USART_SendData(USARTy,tempq );
	 
	  while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
     {
     }
		 
		for(i=0;i<500;i++)
			{;}
				
		 USART_SendData(USARTy,tempb);
				
		 while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
			{
			}
			
		for(i=0;i<500;i++)
			{;}
				
		 USART_SendData(USARTy,temps );
				
	  while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
      {
      }
			
		for(i=0;i<500;i++)
			{;}
				
		 USART_SendData(USARTy,tempg );
				
	 while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
			{
			}
			
		for(i=0;i<500;i++)
			{;}
				
	 USART_SendData(USARTy, ',');
				
	 while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
			{
			}
			
		for(i=0;i<500;i++)
		{;}	
}


int fputc(int ch, FILE *f)

		{

				USART_SendData(USART1, (unsigned char) ch);// USART1 可以换成 USART2 等

				while (!(USART1->SR & USART_FLAG_TXE));

				return (ch);

		}
