
#include "fsmc_sram.h"
#include "font.h"  
#include "demo.h" 
#include "stdlib.h"
#include "module/lcd/lcd_driver.h"

#define Bank1_LCD_D    ((uint32_t)0x60020000)    //disp Data ADDR
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //disp Reg ADDR
void ili9481_Clear(u16 dat);

unsigned char picture1[];
u16 POINT_COLOR=GREEN;     				//默认红色  
u16 DeviceCode;
void LCD_WR_REG(unsigned int index)
{
	*(__IO uint16_t *) (Bank1_LCD_C)= index;

}			 

void LCD_WR_CMD(unsigned int index,unsigned int val)
{	
	*(__IO uint16_t *) (Bank1_LCD_C)= index;	
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}


unsigned short LCD_RD_data(void){
	unsigned int a=0;
	a=*(__IO uint16_t *) (Bank1_LCD_D);   
	a=*(__IO uint16_t *) (Bank1_LCD_D); 	  
	return(a);	
}



void LCD_WR_Data(unsigned int val)
{   
	*(__IO uint16_t *) (Bank1_LCD_D)= val; 	
}

void LCD_WR_Data_8(unsigned int val)
{
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
//
void LCD_Init(void)
{	
	unsigned int i;
	
	LCD_WR_REG(0x0011); 
	Delay(500000);
	LCD_WR_REG(0x0013); 
	LCD_WR_CMD(0x00D0, 0x0007); 
	LCD_WR_CMD(0x00D0, 0x0040); 
	LCD_WR_CMD(0x00D0, 0x001c); 

	LCD_WR_CMD(0x00D1, 0x0000); 

	LCD_WR_CMD(0x00D1, 0x0018); 
	LCD_WR_CMD(0x00D1, 0x001d); 
	LCD_WR_CMD(0x00D2, 0x0001);
	LCD_WR_CMD(0x00D2, 0x0011); 

	LCD_WR_CMD(0x00C0, 0x0000);
	LCD_WR_CMD(0x00C0, 0x003B); 
	LCD_WR_CMD(0x00C0, 0x0000); 
	LCD_WR_CMD(0x00C0, 0x0002); 

	LCD_WR_CMD(0x00C0, 0x0011); 
	LCD_WR_CMD(0x00C1, 0x0010); 
	LCD_WR_CMD(0x00C1, 0x000B); 
	LCD_WR_CMD(0x00C1, 0x0088); 

	LCD_WR_CMD(0x00C5, 0x0001); 
	LCD_WR_CMD(0x00C8, 0x0000);

	LCD_WR_CMD(0x00C8, 0x0030); 

	LCD_WR_CMD(0x00C8, 0x0036); 
	LCD_WR_CMD(0x00C8, 0x0045); 
	LCD_WR_CMD(0x00C8, 0x0004); 

	LCD_WR_CMD(0x00C8, 0x0016); 
	LCD_WR_CMD(0x00C8, 0x0037); 

	LCD_WR_CMD(0x00C8, 0x0075);
	LCD_WR_CMD(0x00C8, 0x0077);
	LCD_WR_CMD(0x00C8, 0x0054);
	LCD_WR_CMD(0x00C8, 0x000f);
	LCD_WR_CMD(0x00C8, 0x0000);
	LCD_WR_CMD(0x00E4, 0x00A0);
	LCD_WR_CMD(0x00F0, 0x0001);
	LCD_WR_CMD(0x00F3, 0x0040);
	LCD_WR_CMD(0x00F3, 0x000A);
	LCD_WR_CMD(0x00F7, 0x0080);
	LCD_WR_CMD(0x0036, 0x000a);
	LCD_WR_CMD(0x003A, 0x0055);

	
	///////////////////////
	LCD_WR_CMD(0x002A, 0x0000); 
	LCD_WR_CMD(0x002A, 0x0000); 
	LCD_WR_CMD(0x002A, 0x0001); 
	LCD_WR_CMD(0x002A, 0x003F); 
	LCD_WR_CMD(0x002B, 0x0000); 
	LCD_WR_CMD(0x002B, 0x0000); 
	LCD_WR_CMD(0x002B, 0x0001); 
	LCD_WR_CMD(0x002B, 0x00df);
	
	
	Delay(500000);
	LCD_WR_REG(0x0029);

	LCD_WR_REG(0x002C);

	for(i=0;i<262144;i++)
	{
	  LCD_WR_Data(0x0000);					 
	}  
}
void ili9481_SetCursor(u16 x,u16 y)
{
	LCD_WR_REG(0x002A);
  
	LCD_WR_Data((y)>>8);
	LCD_WR_Data(0x00FF&(y));
	
	LCD_WR_Data(0x0001);
	LCD_WR_Data(0x003F);
	
	LCD_WR_REG(0x002B);
	
	LCD_WR_Data((480-x)>>8);
	LCD_WR_Data(0x00FF&(480-x));
		
	LCD_WR_Data(0x0001);
	LCD_WR_Data(0x00df);
}

void TFTSetXY(u16 x,u16 y)
{
    ili9481_SetCursor(x,y);
    LCD_WR_REG(0x002C);    
}

void ili9481_SetPoint(u16 x,u16 y,u16 point)
{
	ili9481_SetCursor(x,y);
	
	LCD_WR_REG(0x002C);
	LCD_WR_Data(point);
    
}
void ili9481_DrawLine(unsigned short x1,unsigned short y1, unsigned short x2, unsigned short y2,unsigned short color,unsigned char len)
{
    if(x2 < x1) {return;}

    if(x1 == x2)
    {
        if(y1 > y2)
        {for(short i = 0;i < y1 - y2; i ++)
				 {
					 for(char k = 0;k < len; k ++)
					 {
						 ili9481_SetPoint(x1 + k ,y2 + i,color);
					 }
				 }
			 }
       else
       {
			for(short i = 0;i < y2 - y1; i ++)
			{
				for(char k = 0;k < len; k ++)
				{
					ili9481_SetPoint(x1 + k ,y1 + i,color);
                    //ili9481_SetPoint(x1 + 1 ,y1 + i,color);
				}
			}
		}
				return;
	}
    float k = 0;
    unsigned short dx = 0;
    unsigned short y = 0;
    k = (float)(y2 - y1) / (float)(x2 - x1);
    for(dx = 0; dx < x2 - x1; dx ++)
    {
        y =(unsigned short)(k * dx + y1);
        for(char k = 0;k < len; k ++)
        {
            ili9481_SetPoint(x1 + dx ,y + k,color);
            //ili9481_SetPoint(x1 + 1 ,y1 + i,color);
        }
    }

}
// void ili9481_DrawLine_x(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 point)
// {
// 	u32  n;
// 	ili9481_SetCursor(x_start,y_start);

//     LCD_WR_REG(0x002C);

// 	for(n=x_start;n<=x_end;n++)
// 	{		
//     	LCD_WR_Data(point);
//   	}
  
// }

void ili9481_fill(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 color)//填充矩形区域颜色
{
	u32  n,j;
	ili9481_SetCursor(x_start,y_start);

    LCD_WR_REG(0x002C);
	for(n=x_start;n<=x_end;n++)
	{
		//for(j=320-y_start;j<=320-y_end;j++)
		for(j=y_start;j<=y_end;j++)
		{
	    	LCD_WR_Data(color);
		}
		ili9481_SetCursor(n+1,y_start);
		LCD_WR_REG(0x002C);
	}
}

void ili9481_drawrect(unsigned short x1, unsigned char y1, unsigned short x2, unsigned short y2, unsigned short color,unsigned char width)
{
    ili9481_DrawLine(x1, y1, x1, y2, color,width);
    ili9481_DrawLine(x1, y1, x2, y1, color,width);
    ili9481_DrawLine(x2, y1, x2, y2, color,width);
    ili9481_DrawLine(x1, y2, x2, y2, color,width);
}

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u16 color,u8 mode)
{  
    u8 temp;
    u8 pos,t;
	num=num-' ';
    if(x>479||y<16)return;          
    signed char dx = 0;
    signed char dy = 0;            
	for(dy = 0;dy < 24;dy ++)
    {  
		for(dx = 0;dx < 12;dx ++)
        {
           pos = dy / 8 * 12 + dx;
           if(ascii24[num][pos] & (1 << (7 - (dy % 8))))
           {ili9481_SetPoint(x+dx,y-dy,color); }
					 else{ili9481_SetPoint(x+dx,y-dy,BLACK);}
        }
	}           
}

void LCD_ShowChar_BIG(u16 x,u16 y,u8 num,u8 size,u16 color)
{  
    u8 temp;
    u8 pos,t;
    y = 320-y;
    if(x>479||y<32)return; 
	//ili9481_fill(x,y-32,x+16,y,WHITE);         
    num=num-' ';                                        
    return;
        for(pos=0;pos<size;pos++)
        {
            //if(size==16)
			     //temp=asc2_1608[num][pos];       
            //else if(size==12)    
               //temp=asc2_1206[num][pos];  
            //else return;            
            for(t=0;t<size/2;t++)
            {                 
                if(temp&0x01)
								{
									ili9481_SetPoint(x+2*t,y-2*pos,color);
									ili9481_SetPoint(x+2*t+1,y-2*pos,color);
									ili9481_SetPoint(x+2*t,y-2*pos-1,color);
									ili9481_SetPoint(x+2*t+1,y-2*pos-1,color);
								}
                temp>>=1; 
            }
        }   
                   
}

void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 col,u8 mode)
{     
	y = 320-y;
    while(*p!='\0')
    {       
        if(x>479&&y>26){x=0;y-=16;}
        if(y<16){y=16;x=0;ili9481_Clear(WHITE);}
        LCD_ShowChar(x,y,*p,16,col,1);
        x+=12;
        p++;
    }  
}

void LCD_ShowString_BIG(u16 x,u16 y,const u8 *p,u16 col)
{    
	//y = 320-y;
    while(*p!='\0')
    {       
        if(x>470&&y>=52){x=0;y-=32;}
        if(y<32)
				{
					y=32;
					x=0;
					ili9481_Clear(WHITE);
				} 
				LCD_ShowChar_BIG(x,y,*p,16,col);
        x+=16;
        p++;
		
    }  
}

void ili9481_Clear(u16 dat)
{
	u32  n;
	
	ili9481_SetCursor(0,0);
  
    LCD_WR_REG(0x002C);

	for(n=0;n<480 * 320;n++)
	{
		LCD_WR_Data(dat);					 	
	}

}


void ili9481_DrawLine_x()
{}
void LCD_TEST_Picture3(unsigned char *picture1)
{
}



void TFT_CLEAR(u8 x,u16 y,u8 len,u16 wid)
{                    
    u32 n;	
	LCD_WR_CMD(0x0050, x); 		
	LCD_WR_CMD(0x0051, x+len-1); 
	LCD_WR_CMD(0x0052, y);        
	LCD_WR_CMD(0x0053, y+wid-1); 
	ili9481_SetCursor(20,40);
	GPIO_ResetBits(GPIOD,GPIO_Pin_7);
    LCD_WR_REG(0x002C);
	
	for(n=0;n<50000;n++)
	{
		LCD_WR_Data(0x0000);					 
	}
	
	for(n=0;n<50000;n++)
	{
		LCD_WR_Data(0XFFFF-0xF800);					 
	}
	GPIO_SetBits(GPIOD,GPIO_Pin_7);
}
