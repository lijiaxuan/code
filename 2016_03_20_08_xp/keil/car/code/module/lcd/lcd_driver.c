#include "lcd_driver.h"
#include "lcd_global.h"
#include "module/GPS/GPS_global.h"

void UART3_init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
     
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);   //使能USART3 GPIO时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    USART_DeInit(USART3);  //复位串口3

    //USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化PA9

        //USART3_RX   PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PA10

   //Usart3 NVIC 配置
   NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;       //子优先级3
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          //IRQ通道使能
   NVIC_Init(&NVIC_InitStructure);  //根据指定的参数初始化VIC寄存器
    
    
   //USART 初始化设置
    USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
        
    USART_Init(USART3, &USART_InitStructure); 
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART3, ENABLE);                    //使能串口 
}
static unsigned char txbuffer[FRAME_LEN] = {0};
void TFT_Clear( void )
{
    unsigned char k = 0;
    USART_SendData(USART3, 0xff);
    txbuffer[0] = CLEAR;
    for(k = 0;k < FRAME_LEN;k ++)
    {
        USART_SendData(USART3, txbuffer[k]);
        txbuffer[k] = 0;
    }
}



void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 col,u8 mode)
{
    unsigned char k = 0;
    USART_SendData(USART3, 0xff);

    txbuffer[0] = DISPLAY;
    txbuffer[1] = x;
    txbuffer[2] = y;
    for(k = 0;k < FRAME_LEN && *p != 0;k ++,p ++)
    {
        txbuffer[3 + k] = *p;
    }
    for(k = 0;k < FRAME_LEN;k ++)
    {
        USART_SendData(USART3, txbuffer[k]);
        txbuffer[k] = 0;
    }
}
void USART3_IRQHandler(void)
{
    unsigned char key = 0;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        key = USART_ReceiveData(USART3);
        lcd.process(key);
    }
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);

}

void lcd_timer_cfg(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 , ENABLE);
     TIM_DeInit(TIM6);
    TIM_TimeBaseStructure.TIM_Prescaler = 720-1; 
    TIM_TimeBaseStructure.TIM_Period = 30000; // 5000 - 5ms
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
     TIM_ClearFlag(TIM6,TIM_IT_Update);

}

void TIM6_IRQHandler(void)
{
    TIM_ClearFlag(TIM6,TIM_IT_Update);
    lcd.update_flag = 1;
}
