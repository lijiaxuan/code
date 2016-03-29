#ifndef __USART_H
#define __USART_H
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/5/27
//�汾��V1.3
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
////////////////////////////////////////////////////////////////////////////////// 
#define CHANGE      0xf2
#define SAVE        0xf3
#define SELECT      0xf0
#define UPDATE      0xf1
#define GETVALUE    0xe0
#define START       0xf4
#define STOP        0xf5


extern u8 USART_RX_BUF[11];     //���ջ���,���63���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8 USART_RX_STA;         //����״̬���	
extern unsigned char rxbuffer[20];
extern char recv_flag;
extern char dealed;
extern short cindex;
void parse();
void uart_init(u32 bound);
void update();
void sendselect(char key);
void sendsave(char index);
void sendall();
void sendstart();
void sendstop();

#endif
