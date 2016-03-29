#ifndef __USART_H
#define __USART_H
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/5/27
//版本：V1.3
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
////////////////////////////////////////////////////////////////////////////////// 
#define CHANGE      0xf2
#define SAVE        0xf3
#define SELECT      0xf0
#define UPDATE      0xf1
#define GETVALUE    0xe0
#define START       0xf4
#define STOP        0xf5


extern u8 USART_RX_BUF[11];     //接收缓冲,最大63个字节.末字节为换行符 
extern u8 USART_RX_STA;         //接收状态标记	
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
