/*
* @Author: obsidian
* @Date:   2016-02-29 21:27:41
* @Last Modified by:   obsidian
* @Last Modified time: 2016-02-29 22:20:14
*/
#include "gps_driver.h"


unsigned char USART1_RxBuffer[8];


void Encoder_Timer_Config()
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
    
    //config GPIO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* configue timer */    
    TIM_DeInit(TIM3);       
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 
    TIM_TimeBaseStructure.TIM_Period = COUNTER_PER_ROTATION - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = (u8)0;
    
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    TIM_ClearFlag(TIM3, TIM_FLAG_Update);   
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    
    /* reset the timer3 counter */
    TIM3->CNT = (u16)0;
    /* enable the timer3 */
    TIM_Cmd(TIM3, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
        {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
    
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        
        TIM_DeInit(TIM2);       
        TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
        TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 
        TIM_TimeBaseStructure.TIM_Period = COUNTER_PER_ROTATION - 1;            
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
        TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
        
        
        TIM_ICStructInit(&TIM_ICInitStructure);
        TIM_ICInitStructure.TIM_ICFilter = (u8)0;
    
        TIM_ICInit(TIM2, &TIM_ICInitStructure);

        TIM_ClearFlag(TIM2, TIM_FLAG_Update);   
        TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
        
        /* reset the timer3 counter */
        TIM2->CNT = (u16)0;
        /* enable the timer3 */
        TIM_Cmd(TIM2, ENABLE);

        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    
    }
}

void UART1_DMA_init()
{
    unsigned int baud = 38400;
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
   
    //USART1_RX   PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* USARTy RX DMA1 Channel --DMA1_Channel5(triggered by USART1 Rx event) Config */
    DMA_DeInit(DMA1_Channel5);                                              
    DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned int)&USART1->DR;       
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_RxBuffer;   
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                  
    DMA_InitStructure.DMA_BufferSize = 8;                   
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;    
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;             
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;     
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                     
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;             
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                        
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);                            



    USART_InitStructure.USART_BaudRate = 38400;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

    DMA_ITConfig(DMA1_Channel5,DMA_IT_TC, ENABLE);

    DMA_Cmd(DMA1_Channel5, DISABLE);
   
    USART_Cmd(USART1, ENABLE);  

} 
