#include "usart.h"

void USART_Config(u32 rate)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);
	USART_DeInit(USART3);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
  NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	USART_InitStructure.USART_BaudRate = rate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART3, &USART_InitStructure); //初始化串口
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
	USART_ClearFlag(USART3,USART_FLAG_TC);//USART1可能需要发送，但USART3只需要接受手柄数据，不用发送
	USART_Cmd(USART3, ENABLE);                    //使能串口 
}

int fputc(int ch, FILE *f)
{
	USART_SendData(USART3,(unsigned char) ch);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
	return (ch);
}
