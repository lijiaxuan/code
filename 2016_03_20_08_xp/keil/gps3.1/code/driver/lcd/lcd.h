#ifndef _LCD_H
#define _LCD_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "systick.h"


enum M_KeyNumTpye
{
    key1 = 1,
    key2,
    key3,
    key4,
    keyback,
    emergency,
    
    key5,
    key6,
    key7,
    key8,
    empty1,
    empty2,
    
    key9,
    key0,
    point,
    sign,
    keydelete,
    keyOK
};



#define LCD_LEN  160
extern uint8_t LCD_Table[160];
extern uint8_t LCD_Index;

void LCD_PushByte(uint8_t data);
void LCD_PushString(char *data);	
void LCD_SetXY(int x,int y);
void LCD_WriteChar(char data);
void LCD_WriteString(char *data);
void LCD_WriteDouble(double data,int m);
void LCD_WriteNum(double data);
void LCD_WriteInt(int data);
void LCD_WriteCharXY(char data,int x,int y);
void LCD_WriteStringXY(char *data,int x,int y);
void LCD_WriteIntXY(int data,int x,int y);
void LCD_WriteDoubleXY(double data,int m,int x,int y);
void LCD_WriteNumXY(double data,int x,int y);
void LCD_Clear( void );
void LCD_Clear_Line(u8 C_line);
void LCD_Clear_Forward(u8 L_count);

void refresh_slow(void);
void refresh(void);


void Input_IntValue(int *address,char *name);
void Input_DoubleValue(double * address,char *name);
void Input_FloatValue(float * address,char *name);//浮点输入

void UART3_init(u32 bound);


extern unsigned short KeyReady;
extern unsigned short KeyValue;
extern unsigned short ChangK;

#endif
