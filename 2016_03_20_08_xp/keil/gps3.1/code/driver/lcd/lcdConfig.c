#ifndef LCDCONFIG_C
#define LCDCONFIG_C

#include "lcd.h"

unsigned short KeyReady=0;
unsigned short KeyValue=0;
unsigned short ChangK=0;
void UART3_init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	//使能USART3 GPIO时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
 	USART_DeInit(USART3);  //复位串口3

	//USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化PA9

		//USART3_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PA10

   //Usart3 NVIC 配置
   NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
   NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
		
	USART_Init(USART3, &USART_InitStructure); 
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART3, ENABLE);                    //使能串口 
}

void UART3_SendByte(uint8_t ch)
{
  USART_SendData(USART3, (uint8_t) ch);
  while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}



void USART3_IRQHandler(void)
{	
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
			KeyValue = USART_ReceiveData(USART3);
			KeyReady=1;
		
    }	
	
	if(KeyValue == 6)
	{
		ChangK=1;
	}
    else    
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);  
}
#endif
