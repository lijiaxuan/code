#ifndef GYROCONFIG_C
#define GYROCONFIG_C

#include "gyro.h"


#define USART1_DR_Address ((u32)0x40013804)     //USART1-DR 地址

volatile uint8_t USART1_RxBuffer[RxBufferSize];
volatile uint8_t USART3_TxBuffer[TxBufferSize];


uint8_t Gyro_Error_Flag;
volatile uint8_t USART1_RxBuffer[RxBufferSize];


void UART1_DMA_init(u32 baud)
{
    //GPIO端口设置
	uint32_t i;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	//下面这段程序非常重要！！！！
	/*陀螺仪为8个8位数据，同一帧数据之间的间隔非常小，两帧之间的间隔约1.25ms，大概3.4ms更新一次数据
	按正常方式初始化，即不添加如下代码，则不能保证在将uart使能瞬间，陀螺仪返回的数据处于何种状态：空闲状态or发送中，
	若在空闲状态则通过UART1_DMA_EN()函数能将数据同步
	若处于发送状态，则该帧数据肯定不能正确接收，将接收到错位的数据。
	理论分析，同帧数据间的间隔小会造成连锁反应，但两帧间时间间隔有1ms多，足够让38400比特率的8位数据处于空闲。
	但不知为何，在调用UART1_DMA_EN()函数时会出现很大概率的死机。
	将接收到的数据完全读出后发现，死机状态时uart接收到的数据凭空变成了9个数据。帧头仍然是0xDD，由于数据变成了9个数据，肯定就再也无法同步上了。
	我觉得应该是stm32内部的硬件uart存在某个小问题，当uart刚使能时有数据正在发送就会出现一些小问题。
	通过添加如下IO检测的代码之后，能保证UART使能时处于两帧之间的空闲态。重复很多很多次后，仍未出现问题。
	连续700次检测，通过示波器观察大概需要560us，没有进行计算
	*/
	
	
	i=0;
	while(i<=20)
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10)==0)
		{
			i++;
		}
	}
	i=0;
	while(i<700)
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10)==1)
		{
			i++;
			LED2_on;
		}else
		{
			i=0;
			LED2_off;
		}
	}
	//上面这段程序非常重要！！！！
	
		/* Enable the USART1 DMA Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//Usart1 DMA 配置
	/* USARTy RX DMA1 Channel --DMA1_Channel5(triggered by USART1 Rx event) Config */
	DMA_DeInit(DMA1_Channel5); 												// 
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Address;			//DMA通道1的地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_RxBuffer;		//DMA传送地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						//传送方向 USART是外设
	DMA_InitStructure.DMA_BufferSize = RxBufferSize;						//传送内存大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//传送源地址不递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//传送内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//源地址的数据长度是8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//传送的目的地址是8位宽度
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;							//传送模式循环
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;					//优先级设置
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;							//DMA通道没有设置为内存到内存传送 
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);							//

	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = baud;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	/* Enable USARTy DMA Rx request */
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	/* 允许DMA1通道1传输结束中断 */
	DMA_ITConfig(DMA1_Channel5,DMA_IT_TC, ENABLE);
	/* Enable USARTy RX DMA1 Channel */
//	DMA_Cmd(DMA1_Channel5, DISABLE);
   
    USART_Cmd(USART1, ENABLE);                    //使能串口 

} 

void DMA_check(void)
{
	if(USART1_RxBuffer[7] != 0xDD)
	{
		DMA_Cmd(DMA1_Channel5, DISABLE);       
    Gyro_Error_Flag = 1;
		LED4_on;
	}
}

void UART1_DMA_EN(void)
{
	uint8_t gyro_start,j;
	uint8_t gyro_temp;
	gyro_temp = 0;
	gyro_start=1;    //陀螺仪数据开头标志，此时将DMA通道使能
	j=0;
	
	while(gyro_start)
	{
		if(j==0)
		{
			if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
			{
				USART_ClearFlag(USART1, USART_FLAG_ORE);
				gyro_temp = USART_ReceiveData(USART1);
			}

			if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
			{
				USART_ClearITPendingBit(USART1, USART_IT_RXNE);
				gyro_temp = USART_ReceiveData(USART1);
			}
			
			if(gyro_temp == 0xDD)
			{
				j=1;
			}
		}
		else
		{
			if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
			{
				USART_ClearFlag(USART1, USART_FLAG_ORE);
				gyro_temp = USART_ReceiveData(USART1);
				j++;
			}

			if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
			{
				USART_ClearITPendingBit(USART1, USART_IT_RXNE);
				gyro_temp = USART_ReceiveData(USART1);
				j++;
			}

			if(j==9)
			{
				if(gyro_temp==0xDD)
				{
					gyro_start = 0;				//连续相隔7字节的两个0xDD则认为这是陀螺仪的帧头并开启DMA通道
					DMA_Cmd(DMA1_Channel5, ENABLE);
					LED4_off;
				}
				else
				{
					j=0;
				}
			}else if(j>9)
			{
				j=0;
			}
		}
	}
}

void UART1_SendByte(uint8_t ch)
{
  USART_SendData(USART1, (uint8_t) ch);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

#endif 

