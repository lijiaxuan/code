#ifndef _ILI9327_H_
#define _ILI9327_H_

#define   VGA_BLACK   0x0000
#define   VGA_NAVY    0x0010
#define   VGA_DGREEN  0x0400
#define   VGA_DCYAN   0x0410
#define   VGA_MAROON  0x8000
#define   VGA_PURPLE  0x8010
#define   VGA_OLIVE   0x8400
#define   VGA_LGRAY   0xC618
#define   VGA_DGRAY   0x8410
#define   VGA_BLUE    0x001F
#define   VGA_GREEN   0x07E0
#define   VGA_CYAN    0x07FF
#define   VGA_RED     0xF800
#define   VGA_MAGENTA 0xF81F
#define   VGA_YELLOW  0xFFE0
#define   VGA_WHITE   0xFFFF

void LCD_Init(void);
void LCD_Clear(uint16_t color);
void LCD_Point(uint16_t x,uint16_t y,uint16_t color);
void LCD_Bar(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);
void LCD_VLine(uint16_t x,uint16_t sy,uint16_t ey,uint16_t color);
void LCD_HLine(uint16_t sx,uint16_t y,uint16_t ex,uint16_t color);
void LCD_Line(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);
void LCD_Rect(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);
void LCD_SetArea(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t * pgram);
void LCD_GetArea(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t * pgram);

#endif
