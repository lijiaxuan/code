// /*
// * @Author: obsidian
// * @Date:   2016-03-01 09:18:59
// * @Last Modified by:   obsidian
// * @Last Modified time: 2016-03-01 21:16:26
// */
// #include "lcd_driver.h"

// unsigned int USART3_TxBuffer[12] = {0};

// uint8_t LCD_Table[LCD_LEN];
// uint8_t LCD_Index       =   0;
// unsigned char KeyReady  =   0;
// unsigned char ChangK   =   0;
// unsigned char keyvalue = 0;
// void RefreshDma(void);
// void UART3_SendByte(uint8_t ch);

// void UART3_SendByte(uint8_t ch)
// {}

// /**
//  * [LCD_PushByte description]
//  * @param data [description]
//  */
// void LCD_PushByte(uint8_t data)
// {
//     LCD_Table[LCD_Index]=data;
//     if(LCD_Index >= (LCD_LEN-1))
//         LCD_Index=0;
//     else
//         LCD_Index++;
// }

// void LCD_PushString(char *data)
// {
//     while(*data)
//     {
//         LCD_PushByte(*data++);
//     }
// }

// void LCD_SetXY(int x,int y)
// {
//     LCD_Index=x+y*20;
// }

// void LCD_WriteChar(char data)
// {
//     LCD_PushByte(data);
// }

// void LCD_WriteString(char *data)
// {
//     LCD_PushString(data);
// }

// void LCD_WriteInt(int data)//
// {
//     char answer[100];
//     int index=98;
//     int pose=0;//
//     if(data<0)
//     {
//         pose=1;
//         data=-data;
//     }
//     answer[99]='\0';//
//     while(data>9)//
//     {
//         answer[index--]=data%10+'0';
//         data=data/10;
//     }
//     answer[index]=data+'0';
//     if(pose)//
//     {
//         answer[--index]='-';
//     }
//     LCD_PushString(answer+index);//
// }

// void LCD_WriteDouble(float data,int m)//
// {
//     if(data<0&&data>-1)
//         LCD_WriteChar('-');//
//     LCD_WriteInt((int)data);//
//     if(m>0)
//     {
//         int i;
//         LCD_WriteChar('.');//
//         if(data<0)
//             data=-data;
//         data=data-(int)data;
//         for(i=0;i<m;i++)//
//         {
//             data=data*10;
//             LCD_WriteChar((int)data+'0');
//             data=data-(int)data;
//         }
        
//     }
    
// }


// void LCD_WriteNum(float data)//
// {
//     char str[5];
//     int flag=0,i;
//     if(data<0)
//     {
//         LCD_WriteChar('-');//
//         data=-data;
//     }
//     LCD_WriteInt((int)data);//
//     data=data-(int)data;
//     for(i=0;i<5;i++)//
//     {
//         data=data*10;
//         str[i]=(int)data+'0';
//         if(str[i]!='0')
//         {
//             flag=i+1;
//         }
//         data=data-(int)data;
        
//     }
//     if(flag!=0)
//         LCD_WriteChar('.');//
//     for(i=0;i<flag;i++)
//     {
//         LCD_WriteChar(str[i]);//
//     }   
//     for(i=flag;i<6;i++)
//     {
//         LCD_WriteChar(' ');//
//     }
// }

// void LCD_WriteCharXY(char data,int x,int y)
// {
//     LCD_SetXY(x,y);
//     LCD_PushByte(data);
// }

// void LCD_WriteStringXY(char *data,int x,int y)
// {
//     LCD_SetXY(x,y);
//     LCD_PushString(data);
// }

// void LCD_WriteIntXY(int data,int x,int y)
// {
//     LCD_SetXY(x,y);
//     LCD_WriteInt(data);
// }

// void LCD_WriteDoubleXY(float data,int m,int x,int y)
// {
//     LCD_SetXY(x,y);
//     LCD_WriteDouble(data, m);
// }

// void LCD_WriteNumXY(float data,int x,int y)
// {
//     LCD_SetXY(x,y);
//     LCD_WriteNum(data);
// }

// void LCD_Clear(void)//
// {
//     //LCD_WriteChar(12);//
//     LCD_WriteStringXY("                                                                                ",0,0); 
//     LCD_WriteStringXY("                                                                                ",0,4); 

//     LCD_SetXY(0,0);
// }

// void LCD_Clear_Line(u8 C_line)
// {
//     LCD_WriteStringXY("                    ",0,C_line); 
// }

// void LCD_Clear_Forward(u8 L_count)
// {
//     char i=0;
//     for(i=0;i<L_count;i++)
//     {
//         LCD_Clear_Line(i);
//     }
// }

// void WriteChar(char data)
// {
//     UART3_SendByte(data);
// }

// void WriteString(char *data)        //
// {
//     while(*data)
//     {
//         WriteChar(*data++);
//     }
// }

// void WriteInt(int data)//
// {
//     char answer[100];
//     int index=98;
//     int pose=0;//
//     if(data<0)
//     {
//         pose=1;
//         data=-data;
//     }
//     answer[99]='\0';//
//     while(data>9)//
//     {
//         answer[index--]=data%10+'0';
//         data=data/10;
//     }
//     answer[index]=data+'0';
//     if(pose)//
//     {
//         answer[--index]='-';
//     }
//     WriteString(answer+index);//
// }
// /**
//  * [WriteDouble description] float
//  * @param data [description]
//  * @param m    [description]
//  */
// void WriteDouble(float data,int m)
// {
//         unsigned char* pdata;
//         unsigned char i = 0;
//         float w_data;
//         w_data=(float)data;
//         pdata = (unsigned char *)&w_data;
//         for(i = 0;i < 4;i ++)
//         {
//             USART_SendData(USART3, * (pdata + i));
//             while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);    
//         }
    
// }

// void refresh(void)
// {
//     {
//         RefreshDma();
//     }
// }

// void refresh_slow(void)
// {
//     static  char send_flag=0;
    
//     if(send_flag%10==0)
//     {
//         RefreshDma(); 
//     }
//     send_flag++;
// }

// void RefreshDma(void)
// {
//     #ifdef LCD_DEBUG
//     static  int m=0;
    
//     UART3_SendByte(0xff);
//     while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);    
//     UART3_SendByte(30*(m/2)+(m%2)*10);
//     while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);    

//     DMA_Cmd(DMA1_Channel2, DISABLE );  //??USART1 TX DMA1 ??????   
//     DMA1_Channel2->CMAR = (uint32_t)&LCD_Table[10*m];
//     DMA_SetCurrDataCounter(DMA1_Channel2,10);//DMA???DMA?????
//     DMA_Cmd(DMA1_Channel2, ENABLE);  //??USART1 TX DMA1 ?????? 
    
//     if(m>=15)
//         m= 0;
//     else
//         m++;
//     #endif
// }

// void uart_init(u32 bound)
// {
//     //GPIO????
//     GPIO_InitTypeDef GPIO_InitStructure;
// 	USART_InitTypeDef USART_InitStructure;
// 	NVIC_InitTypeDef NVIC_InitStructure;
	 
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	//??USART3 GPIO??
// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
//  	USART_DeInit(USART3);  //????3

// 	//USART3_TX   PB.10
//     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//??????
//     GPIO_Init(GPIOB, &GPIO_InitStructure); //???PA9

// 		//USART3_RX	  PB.11
//     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//????
//     GPIO_Init(GPIOB, &GPIO_InitStructure);  //???PA10

//    //Usart3 NVIC ??
//    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//?????3
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//????3
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
//    NVIC_Init(&NVIC_InitStructure);	//??????????VIC???
	
	
//    //USART ?????
// 	USART_InitStructure.USART_BaudRate = bound;//?????9600;
// 	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//???8?????
// 	USART_InitStructure.USART_StopBits = USART_StopBits_1;//?????
// 	USART_InitStructure.USART_Parity = USART_Parity_No;//??????
// 	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????????
// 	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//????
		
// 	USART_Init(USART3, &USART_InitStructure); 
// 	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//????
// 	USART_Cmd(USART3, ENABLE);                    //???? 
// }
