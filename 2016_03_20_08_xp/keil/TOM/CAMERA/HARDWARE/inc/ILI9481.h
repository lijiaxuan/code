#ifndef __LCD_H
#define __LCD_H		
//#include "sys.h"	 
#include "stdlib.h"
#include "stm32f4xx.h"
//////////////////////////////////////////////////////////////////////////////////	 
//3.5'' ILI9481	  
//date:2013/08/27
//author:xxn59
////////////////////////////////////////////////////////////////////////////////// 
 
#define LCD_DATA    ((u32)0x6C000002)    	//disp Data ADDR     RS  FSMC A[16]
#define LCD_CMD     ((u32)0x6C000000)	 		//disp Reg ADDR
#define DCMI_TO_ExtSRAM

#ifdef DCMI_TO_ExtSRAM
#define GUI_ReadBit16Point ExtSRAM_ReadPixel
#else 
#define GUI_ReadBit16Point LCD _ReadPixel
#endif
#define WHITE        0xFFFF
#define BLACK        0x0000	  
#define BLUE         0x001F  
#define BRED         0XF81F
#define GRED 			   0XFFE0
#define GBLUE			   0X07FF
#define RED          0xF800
#define MAGENTA      0xF81F
#define GREEN        0x07E0
#define CYAN         0x7FFF
#define YELLOW       0xFFE0
#define BROWN 			 0XBC40 //
#define BRRED 			 0XFC07 //
#define GRAY  			 0X8430 //
//GUI

#define DARKBLUE      	 0X01CF	//
#define LIGHTBLUE      	 0X7D7C	//  
#define GRAYBLUE       	 0X5458 //
//PANEL
 
#define LIGHTGREEN     	 0X841F //
#define LIGHTGRAY        0XEF5B //(PANNEL)
#define LGRAY 			 0XC618 //(PANNEL)

#define LGRAYBLUE        0XA651 //
#define LBBLUE           0X2B12 //

#define RST_HIGH()		GPIOB->BSRRL = GPIO_Pin_1
#define RST_LOW()			GPIOB->BSRRH = GPIO_Pin_1

////////////////////////////////////////////////////////////////////
//-----------------Var--------------- 
    															  
extern u16  BACK_COLOR,  DeviceCode; //
extern u8   USE_HORIZONTAL;  	//
extern u16  BACK_COLOR;         //


////////////////////////////////////////////////////////////////////
//-----------------API--------------- 
	    															  
extern u16 BACK_COLOR,  DeviceCode; //

void LCD_GPIOInit(void);
void ili9481_WriteRegister(u16 Command);
void ili9481_WriteData(u16 Data);
void Delay_ili9481(u32 nCount);
void ili9481_Reset(void);
void ili9481_Init(void);
// void LCD_Init(void);					//LCD
void LCD_GPIOInit(void);
// void LCD_DisplayOn(void);															   
// void LCD_DisplayOff(void);															   
void LCD_Clear_ili9481(u16 Color);	 														   
void LCD_SetCursor_ili9481(u16 Xpos, u16 Ypos);											   	    
void LCD_SetBox(u16 Xstart, u16 Ystart, u16 Xend, u16 Yend);
void LCD_DrawPoint_ili9481(u16 x,u16 y,s8 size,u16 color);                                    
void Draw_Circle_ili9481(u16 x0,u16 y0,u8 r,u8 Si,u16 cl);					          	       
void FillSolidRect(u16 x, u16 y, u16 Width, u16 Height,  u16 Color);
void LCD_DrawLine_ili9481(u16 x1,u16 y1,u16 x2,u16 y2,u16 cl);			               
void LCD_DrawRectangle_ili9481(u16 x1, u16 y1, u16 x2, u16 y2,u8 s,u16 c1,u16 c2,u16 c3,u16 c4);
void LCD_Fill_ili9481(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);						   
void LCD_ShowChar_ili9481(u16 x,u16 y,u8 num,u8 size,u16 color,u8 mode);                   
// void LCD_ShowsigNum(u16 x,u16 y,s32 num,u16 col,u8 mode);			        		   
void LCD_ShowString_ili9481(u16 x,u16 y,const u8 *p,u16 col,u8 mode);            
void LCD_WriteString16_ili9481(u16 x, u16 y, u16 CharColor,u16 backcolor, char* p);
void LCD_WriteString24(u16 x, u16 y, u16 CharColor,u16 backcolor, char* p);
unsigned short LCD_ReadPixel(unsigned short x,unsigned short y);
// void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);									    
// void LCD_SetHor(u8 hor);                        									
// void LCD_InvertOn(void);                                                               
// void LCD_InvertOff(void);                                                              
// void LCD_ShowFloat(u16 x,u16 y,float Da,u16 Col,u8 mode);                  			 
// u16 LCD_ReadReg(u8 LCD_Reg);														    
// u16 LCD_Just_Read(void);														
u16 LCD_BGR2RGB(u16 c);																        
u32 Pow(u8 m,u8 n);																	    //
unsigned short GUI_ReadBit16Point(unsigned short x,unsigned short y);

extern void LCDCopyToExternalSRAM(void);
extern void ExtSRAMCopyToLCD(void);
extern void ExtSRAMCopyToLCDQVGA(void);
									
																			 						  		 
#endif  
