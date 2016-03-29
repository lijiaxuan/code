#ifndef LCD_DRIVER_H
/*
    include
 */
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "misc.h"

/*
    define
 */

#define LCD_LEN  160
#define INPUT    1

/*
    global
 */

extern unsigned int     USART3_TxBuffer[12];
extern unsigned char    keyvalue;
extern unsigned char    KeyReady;
extern unsigned char    ChangK ;

/*
    function
 */
// void uart_init(u32 bound);

// extern uint8_t LCD_Table[160];
// extern uint8_t LCD_Index;

// void LCD_PushByte(uint8_t data);
// void LCD_PushString(char *data);    
// void LCD_SetXY(int x,int y);
// void LCD_WriteChar(char data);
// void LCD_WriteString(char *data);
// void LCD_WriteDouble(float data,int m);
// void LCD_WriteNum(float data);
// void LCD_WriteInt(int data);
// void LCD_WriteCharXY(char data,int x,int y);
// void LCD_WriteStringXY(char *data,int x,int y);
// void LCD_WriteIntXY(int data,int x,int y);
// void LCD_WriteDoubleXY(float data,int m,int x,int y);
// void LCD_WriteNumXY(float data,int x,int y);
void LCD_Clear( void );
void ili9481_DrawLine(unsigned short x1,unsigned short y1, unsigned short x2, unsigned short y2,unsigned short color,unsigned char len);
void ili9481_fill(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 color);
void ili9481_drawrect(unsigned short x1, unsigned char y1, unsigned short x2, unsigned short y2, unsigned short color,unsigned char width);
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 col,u8 mode);
// void LCD_Clear_Line(u8 C_line);
// void LCD_Clear_Forward(u8 L_count);


// void WriteString(char *data);
// void WriteDouble(float data,int m);
// void WriteInt(int data);

// void refresh(void);
// void refresh_slow(void);
// void RefreshDma(void);
    
#define LCD_DRIVER_H
#endif
