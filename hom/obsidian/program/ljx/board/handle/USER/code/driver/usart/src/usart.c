#include "../inc/usart.h"
#include "module/handle/inc/handle_global_variable.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
int _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
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
 
// #if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  

//��ʼ��IO ����1 
//bound:������
u8 rec_buf[8] = {'0','0','0','0','0','0','0','0'};
u8 rec_buffer;
u8 flag = 0;
int receive = 0;
char div = '1';
int buffer_view =0;
u16 uart_receive_count = 0;
u16 receive_u8_count = 0;
u16 uart_send_count = 0;
u8 start_num = 0;
union u8_to_u16
{
	u8 temp[2];
	u16 rec_data;
}trans_lunzi,trans_fan;
int rec_flag = 1;
void uart_init(u32 bound){
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART1��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
 	USART_DeInit(USART1);  //��λ����1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
	//USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10
		//USART3_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PA10

   //Usart1 NVIC ����

   NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
   NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
  //Usart3 NVIC ����
	 NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	 NVIC_Init(&NVIC_InitStructure);
	
   //USART ��ʼ������

		USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
		//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		USART_ClearFlag(USART1,USART_FLAG_TC);
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 
		
		USART_Init(USART3, &USART_InitStructure); 
		//USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
//		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
		
    USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 
}



void USART1_IRQHandler(void)
{

		receive_u8_count++;
		uart_receive_count=receive_u8_count/4;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//		USART_ClearFlag(USART1,USART_FLAG_RXNE);
// 		USART_ClearFlag(USART3,USART_FLAG_TC);
		if(start_num==0&&0xc8==USART_ReceiveData(USART1))
		{
	  
			
// 			USART_SendData(USART3,0xc8);
			//while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);   
//  			USART_ClearFlag(USART3,USART_FLAG_TC);
			start_num=1;
			rec_flag = 0;
		}
	
		if(start_num)
		{
	
			switch(rec_flag)
			{
				case 0:
					rec_flag = 1;
				break;
				case 1:
					rec_buf[0]=USART_ReceiveData(USART1);    
					rec_flag=2;
				  
//  					USART_SendData(USART3,rec_buf[1]);
// 					while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); 
				
					break;
				case 2:
					rec_buf[1]=USART_ReceiveData(USART1); 
					rec_flag = 3;
				
//  					USART_SendData(USART3,rec_buf[2]);
// 					while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);  
				
					break;
				case 3:
					rec_buf[2]=USART_ReceiveData(USART1);    
					rec_flag=4;
					  
//  					USART_SendData(USART3,rec_buf[3]);
// 					while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); 
		
					break;
				case 4:
					rec_buf[3]=USART_ReceiveData(USART1); 
					rec_flag = 5;
					
//  					USART_SendData(USART3,rec_buf[4]);
// 					while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);   
				
					break;
				case 5:
					rec_buf[4]=USART_ReceiveData(USART1);    
					rec_flag=6;
					 
//  					USART_SendData(USART3,rec_buf[5]);
// 					while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);  
			
					break;
				case 6:
					rec_buf[5]=USART_ReceiveData(USART1); 
					rec_flag = 7;
					 
//  					USART_SendData(USART3,rec_buf[6]);
// 					while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);  
		
					break;
				case 7:
					rec_buf[6]=USART_ReceiveData(USART1); 
					rec_flag = 8;
					 
//  					USART_SendData(USART3,rec_buf[7]);
// 					while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);  
		
					break;
				case 8:
					rec_buf[7] = USART_ReceiveData(USART1);
					
						 
//  						USART_SendData(USART3,rec_buf[7]);
// 						while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
// 				
						rec_flag = 9;
						handle.steering_angle_control_data = *((unsigned short*)(&rec_buf[3]));						
						handle.steering_angle_control_data = *((unsigned short*)(&rec_buf[5]));
						//trans_lunzi.temp[1] = rec_buf[3];
						//trans_lunzi.temp[0] = rec_buf[4];
						//trans_fan.temp[1] = rec_buf[5];
						//trans_fan.temp[0] = rec_buf[6];
						
// 						uart_send_count = trans.rec_data;
// 						time2 = time - uart_send_count;
					start_num = 0;
					break;
				default:
					break;
			
		}
	}

}




#ifdef DELAY_TIME

#endif	