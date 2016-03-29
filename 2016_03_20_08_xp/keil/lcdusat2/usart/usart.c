#include "sys.h"
#include "usart.h"
#include "demo.h"
#include "module/lcd/lcd_global.h"
#include "module/lcd/numstr.h"
#define BUFFER_LEN 6

extern void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 col,u8 mode);
extern void ili9481_fill(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 color);
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 col,u8 mode);
void ili9481_fill(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 color);


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
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //Usart1 NVIC ÅäÖÃ

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾ÝNVIC_InitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèNVIC¼Ä´æÆ÷USART1
  
   //USART ³õÊ¼»¯ÉèÖÃ
   
	USART_InitStructure.USART_BaudRate = bound;//Ò»°ãÉèÖÃÎª9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
   

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//¿ªÆôÖÐ¶Ï
   
    USART_Cmd(USART1, ENABLE);                    //Ê¹ÄÜ´®¿Ú 

}

unsigned char rxbuffer[20];
static unsigned char txbuffer[20];
static unsigned char rx_count = 0; 
char cache[250] = {0};
short cindex = 0;
char recv_flag = 0;
char dealed = 0;

void USART1_IRQHandler(void)                	//´®¿Ú1ÖÐ¶Ï·þÎñ³ÌÐò
{
    static unsigned char key = 0;
    static unsigned char find_start = 0;
    static unsigned char last_recv = 0;	
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
            key = USART_ReceiveData(USART1);
			      //cache[cindex] = key;
			      //cindex ++;
						if(key == 0x01)
						{
							key =0x01;
							}
            if(!find_start)
            {
                if(last_recv == 0xff && key == 0xff)
                {
                    find_start = 1;
                }
                last_recv = key;
            }
            else
            {
                rxbuffer[rx_count] = key;
                rx_count ++;
                last_recv = key;
                if(rx_count >= BUFFER_LEN)
                {
                    rx_count = 0;
                    find_start = 0;
									//recv_flag = 1;
									//dealed ++;
                    parse();
                }
            }
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    }   
		
}

void sendselect(char key)
{
    txbuffer[0] = SELECT;
    txbuffer[1] = lcd.p_status->p_current->node_id;
    txbuffer[2] = lcd.p_status->p_current->page_index;
    unsigned char i = 0;
    USART_SendData(USART1, 0xff);
    USART_SendData(USART1, 0xff);
    for(i = 0;i < BUFFER_LEN;i ++)
    {
        USART_SendData(USART1, txbuffer[i]);
        txbuffer[i] = 0;
    }
}

void sendall()
{
    txbuffer[0] = SAVE;
    unsigned char i = 0;
    USART_SendData(USART1, 0xff);
    USART_SendData(USART1, 0xff);
    for(i = 0;i < BUFFER_LEN;i ++)
    {
        USART_SendData(USART1, txbuffer[i]);
        txbuffer[i] = 0;
    }
}


void sendsave(char index)
{
	unsigned char i = 0;
    txbuffer[0] = CHANGE;
    txbuffer[1] = index + lcd.p_status->p_current->page_index * PAGE_SIZE;
    if(lcd.p_status->p_current->p_item[index].flag == FILE)
    {
        if(lcd.p_status->p_current->p_item[index].is_int == 0)
        {
            *(float *)(&txbuffer[2]) = *(lcd.p_status->p_current->p_item[index].p_float);
        }
        else
        {
            *(unsigned short *)(&txbuffer[2]) = *(lcd.p_status->p_current->p_item[index].p_short);               
        }
    }
    USART_SendData(USART1, 0xff);
    USART_SendData(USART1, 0xff);
    for(i = 0;i < BUFFER_LEN;i ++)
    {
        USART_SendData(USART1, txbuffer[i]);
        txbuffer[i] = 0;
    }
}

// void LCD_Clear(void)
// { 
//     u16 color=MAGENTA;
//     ili9481_fill(0, 0, 480, 320, BLACK);

//     LCD_ShowString(180, 33, "For Car B", WHITE,1);
// }

void update()
{
    txbuffer[0] = UPDATE;
    unsigned char i = 0;
    USART_SendData(USART1, 0xff);
    USART_SendData(USART1, 0xff);
    for(i = 0;i < BUFFER_LEN;i ++)
    {
        USART_SendData(USART1, txbuffer[i]);
        txbuffer[i] = 0;
    }
}

void parse()
{
    struct Node *p_tmp = lcd.p_status->p_current;
	struct Item *p_tmp_item = lcd.p_status->p_current->p_item;
    unsigned char tmpid = 0;
    tmpid = (p_tmp->p_item[rxbuffer[1]].id - 1) % PAGE_SIZE;
    dealed ++;
	short k = 0;
    if(rxbuffer[0] == GETVALUE && lcd.p_status->p_current->node_id != 0)
    {
        if(p_tmp_item[rxbuffer[1]].flag == FILE)
        {
            if(p_tmp_item[rxbuffer[1]].is_int == 0)
            {
                *(p_tmp_item[rxbuffer[1]].p_float) = *(float *)(&rxbuffer[2]);
                num.floatvalue = *(float *)(&rxbuffer[2]);
                num.float2str_3();
            }
            else
            {
                *(p_tmp_item[rxbuffer[1]].p_short) = *(unsigned short *)(&rxbuffer[2]);
                num.intvalue = *(unsigned short *)(&rxbuffer[2]);
                num.int2str();                
            }
            //LCD_ShowString(80 + 220 * ((tmpid) * 2 / PAGE_SIZE) + lcd.p_status->p_current->p_item[rxbuffer[1]].ctx_len * 12 , 70 + LINE_DIS * (((tmpid)) % (PAGE_SIZE / 2)), "        ", WHITE,1);
            LCD_ShowString(80 + 220 * ((tmpid) * 2 / PAGE_SIZE) + p_tmp_item[rxbuffer[1]].ctx_len * 12 , 70 + LINE_DIS * (((tmpid)) % (PAGE_SIZE / 2)), num.numstr, WHITE,1);
            num.init(); 
        }
    }
// 	num.intvalue = dealed;
// 	num.int2str();
// 	LCD_ShowString(80,80,num.numstr,WHITE,1);
	for(char i = 0;i < BUFFER_LEN; i++)
	{
		rxbuffer[i] = 0;
	}
}

void sendstart()
{
    txbuffer[0] = START;
    unsigned char i = 0;
    USART_SendData(USART1, 0xff);
    USART_SendData(USART1, 0xff);
    for(i = 0;i < BUFFER_LEN;i ++)
    {
        USART_SendData(USART1, txbuffer[i]);
        txbuffer[i] = 0;
    }
}
void sendstop()
{
    txbuffer[0] = STOP;
    unsigned char i = 0;
    USART_SendData(USART1, 0xff);
    USART_SendData(USART1, 0xff);
    for(i = 0;i < BUFFER_LEN;i ++)
    {
        USART_SendData(USART1, txbuffer[i]);
        txbuffer[i] = 0;
    }
}
