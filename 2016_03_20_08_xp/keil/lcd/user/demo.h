#ifdef GLOBALS 
#define EXT
#else
#define EXT extern 
#endif

#include "stm32f10x.h"

#define Led_ON()   GPIO_SetBits(GPIOB, GPIO_Pin_5);  	      //LED1亮
#define Led_OFF()  GPIO_ResetBits(GPIOB, GPIO_Pin_5); 	      //LED2灭

#define TP_CS()  GPIO_ResetBits(GPIOB,GPIO_Pin_7)	  		  //触摸SPI 片选允许
#define TP_DCS() GPIO_SetBits(GPIOB,GPIO_Pin_7)				  //触摸SPI 片选禁止
#define PEN  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)          //触摸检测  

  
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 (~0xF800)
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)



void TFT_CLEAR(u8 x,u16 y,u8 len,u16 wid);
void ili9481_DrawLine_x(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 point);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u16 color,u8 mode);
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 col,u8 mode);
extern void ili9481_fill(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 color);
