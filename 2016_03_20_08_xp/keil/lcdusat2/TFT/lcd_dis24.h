#ifndef LCD_DIS_H
#include "stm32f10x.h"

void LCD_Init(void);
void LCD_WR_REG(unsigned int index);
void LCD_WR_CMD(unsigned int index,unsigned int val);

void LCD_WR_Data(unsigned int val);

void LCD_test(void);

void Delay(__IO uint32_t nCount);

unsigned short LCD_RD_data(void);
extern void lcd_rst(void);
extern void Delay(__IO uint32_t nCount);

void TFTSetXY(u16 x,u16 y);     
void ili9481_SetCursor(u16 x,u16 y);
void ili9481_SetPoint(u16 x,u16 y,u16 point);
void ili9481_Clear(u16 dat);
//void ili9481_DrawLine_x(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 point);
void TFT_CLEAR(u8 x,u16 y,u8 len,u16 wid);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u16 color,u8 mode);
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 col);
void LCD_ShowChar_BIG(u16 x,u16 y,u8 num,u8 size,u16 color);
void LCD_ShowString_BIG(u16 x,u16 y,const u8 *p,u16 col);
void LCD_TEST_Picture3(unsigned char *picture1);
void ili9481_fill(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 color);
void ili9481_DrawLine(unsigned short x1,unsigned short y1, unsigned short x2, unsigned short y2,unsigned short color,unsigned char len);
void ili9481_drawrect(unsigned short x1, unsigned char y1, unsigned short x2, unsigned short y2, unsigned short color,unsigned char width);

#define LCD_DIS_H
#endif