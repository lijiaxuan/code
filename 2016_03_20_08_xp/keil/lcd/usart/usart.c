#include "sys.h"
#include "usart.h"
#include "demo.h"

//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌÐòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßÐí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ALIENTEK Mini STM32¿ª·¢°å
//´®¿Ú1³õÊ¼»¯		   
//ÕýµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.com
//ÐÞ¸ÄÈÕÆÚ:2010/5/27
//°æ±¾£ºV1.3
//°æÈ¨ËùÓÐ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ÕýµãÔ­×Ó 2009-2019
//All rights reserved
//********************************************************************************
//V1.3ÐÞ¸ÄËµÃ÷ 
//Ö§³ÖÊÊÓ¦²»Í¬ÆµÂÊÏÂµÄ´®¿Ú²¨ÌØÂÊÉèÖÃ.
//¼ÓÈëÁË¶ÔprintfµÄÖ§³Ö
//Ôö¼ÓÁË´®¿Ú½ÓÊÕÃüÁî¹¦ÄÜ.
//ÐÞÕýÁËprintfµÚÒ»¸ö×Ö·û¶ªÊ§µÄbug
////////////////////////////////////////////////////////////////////////////////// 	  


//¼ÓÈëÒÔÏÂ´úÂë,Ö§³Öprintfº¯Êý,¶ø²»ÐèÒªÑ¡Ôñuse MicroLIB
#if 1
#pragma import(__use_no_semihosting)             
//±ê×¼¿âÐèÒªµÄÖ§³Öº¯Êý                 
struct __FILE 
{ 
	int handle;
	
}; 

FILE __stdout;       
//¶¨Òå_sys_exit()ÒÔ±ÜÃâÊ¹ÓÃ°ëÖ÷»úÄ£Ê½    
int _sys_exit(int x) 
{ 
	x = x; 
} 
//ÖØ¶¨Òåfputcº¯Êý 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

short dealed_index = 0;
short recv_index = 0;
unsigned char * pdeal = cmd_buff;
/*Ê¹ÓÃmicroLibµÄ·½·¨*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
u8 cmd_buff[1024];
unsigned char recv_flag = 0;     
char USART_position;
u8 flag_position=0;
u8 flag_COMPUTER=0;
u8 flag_data=0;
u8 flag_position_main=0;
u8 USART_RX_STA=0;       
static u8 count=0;
u8 count_pos=0;
u8 flag_tx=0;
void uart_init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
     
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX   PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //Usart1 NVIC ÅäÖÃ

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;      //

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQÍ¨µÀÊ¹ÄÜ
    NVIC_Init(&NVIC_InitStructure); //¸ù¾ÝNVIC_InitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèNVIC¼Ä´æÆ÷USART1
  
   //USART ³õÊ¼»¯ÉèÖÃ
   
    USART_InitStructure.USART_BaudRate = bound;//Ò»°ãÉèÖÃÎª9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
   

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//¿ªÆôÖÐ¶Ï
   
    USART_Cmd(USART1, ENABLE);     
}

static unsigned char rxbuffer[14] = {0};

void strcopy(unsigned char* src,char len)
{
    unsigned char k = 0;
    for(k = 0;k < len;k ++)
    {
        *pdeal = src[k];
        pdeal ++;
    }
}

static char cach[255] = {0};
static short index = 0;
void USART1_IRQHandler(void)                	//´®¿Ú1ÖÐ¶Ï·þÎñ³ÌÐò
{	
    static char recvstart = 0;
    unsigned char data = 0;
    
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
    data = USART_ReceiveData(USART1);
	cach[index] = data;
	index ++;
	  if(index > 255)
		{index = 0;}
    if(recvstart == 0 && data == 0xff)
    {
        recvstart = 1;
    }
    else if(recvstart == 1)
    {
        rxbuffer[recv_index] = data;
        recv_index ++;
        if(recv_index >= FRAME_LEN)
        {
            strcopy(rxbuffer, FRAME_LEN);
            dealed_index += FRAME_LEN;
			recv_index = 0;
            pdeal += FRAME_LEN;
            recvstart = 0;
					recv_flag = 1;
        }
        
    }
    }
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}
void parse()
{
    unsigned char *tmp = cmd_buff;
    unsigned char posx = 0;
    unsigned char posy = 0;
    for(;tmp != pdeal;tmp += FRAME_LEN)
    {
        if(tmp[0] == DISPLAY)
        {
            posx = tmp[1];
            posy = tmp[2];
            LCD_ShowString(posx * 12, posy * 24, (unsigned char *)&tmp[3], WHITE, 1);
        }
        if(tmp[0] == CLEAR)
        {
            ili9481_fill(0, 0, 480, 320, 1);
        }
    }
    pdeal = cmd_buff;
}
