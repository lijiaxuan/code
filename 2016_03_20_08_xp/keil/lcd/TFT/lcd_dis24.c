/****************************************************************************
* Copyright (C), 2011 ·Ü¶·Ç¶ÈëÊ½¹¤×÷ÊÒ www.ourstm.net
*
* ±¾Àý³ÌÔÚ ·Ü¶·°æSTM32¿ª·¢°åV2,2.1,V3ÉÏµ÷ÊÔÍ¨¹ý           
* QQ: 9191274, ÍúÍú£ºsun68, Email: sun68@163.com 
* ÌÔ±¦µêÆÌ£ºourstm.taobao.com  
*
* ÎÄ¼þÃû: Fun.c
* ÄÚÈÝ¼òÊö:
*       ±¾Àý³ÌÌá¹©ÁË·Ü¶·°åÅä2.4´çÆÁÄ£¿éµÄÇý¶¯³ÌÐò
		
*
* ÎÄ¼þÀúÊ·:
* °æ±¾ºÅ  ÈÕÆÚ       ×÷Õß    ËµÃ÷
* v0.2    2011-07-21 sun68  ´´½¨¸ÃÎÄ¼þ
*
*/


#include "fsmc_sram.h"
#include "font.h"  
#include "demo.h" 
#include "stdlib.h"
 
#define Bank1_LCD_D    ((uint32_t)0x60020000)    //disp Data ADDR
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //disp Reg ADDR



void LCD_Init(void);
void LCD_WR_REG(unsigned int index);
void LCD_WR_CMD(unsigned int index,unsigned int val);

void LCD_WR_Data(unsigned int val);

void LCD_test(void);
void LCD_clear(unsigned int p);

void Delay(__IO uint32_t nCount);

unsigned short LCD_RD_data(void);
extern void lcd_rst(void);
extern void Delay(__IO uint32_t nCount);

void TFTSetXY(u16 x,u16 y);     
void ili9481_SetCursor(u16 x,u16 y);
void ili9481_SetPoint(u16 x,u16 y,u16 point);
void ili9481_Clear(u16 dat);
void ili9481_DrawLine_x(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 point);
void TFT_CLEAR(u8 x,u16 y,u8 len,u16 wid);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u16 color,u8 mode);
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 col,u8 mode);
void LCD_ShowChar_BIG(u16 x,u16 y,u8 num,u8 size,u16 color);
void LCD_ShowString_BIG(u16 x,u16 y,const u8 *p,u16 col);
void LCD_TEST_Picture3(unsigned char *picture1);
void ili9481_fill(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 color);
unsigned char picture1[];
u16 POINT_COLOR=GREEN;     				//Ä¬ÈÏºìÉ«  
u16 DeviceCode;
/****************************************************************************
* Ãû    ³Æ£ºLCD_WR_REG(unsigned int index)
* ¹¦    ÄÜ£ºFSMCÐ´ÏÔÊ¾Æ÷¼Ä´æÆ÷µØÖ·º¯Êý
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎÞ 
****************************************************************************/ 
void LCD_WR_REG(unsigned int index)
{
	*(__IO uint16_t *) (Bank1_LCD_C)= index;

}			 

/****************************************************************************
* Ãû    ³Æ£ºvoid LCD_WR_CMD(unsigned int index,unsigned int val)
* ¹¦    ÄÜ£ºFSMCÐ´ÏÔÊ¾Æ÷¼Ä´æÆ÷Êý¾Ýº¯Êý
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎÞ 
****************************************************************************/ 
void LCD_WR_CMD(unsigned int index,unsigned int val)
{	
	*(__IO uint16_t *) (Bank1_LCD_C)= index;	
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}


/****************************************************************************
* Ãû    ³Æ£ºunsigned short LCD_RD_data(void)
* ¹¦    ÄÜ£ºFSMC¶ÁÏÔÊ¾Çø16Î»Êý¾Ýº¯Êý
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎÞ 
****************************************************************************/ 
unsigned short LCD_RD_data(void){
	unsigned int a=0;
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //¿Õ²Ù×÷
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //¶Á³öµÄÊµ¼Ê16Î»ÏñËØÊý¾Ý	  
	return(a);	
}



/****************************************************************************
* Ãû    ³Æ£ºLCD_WR_Data(unsigned int val)
* ¹¦    ÄÜ£ºFSMCÐ´16Î»Êý¾Ýº¯Êý
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎÞ 
****************************************************************************/ 
void LCD_WR_Data(unsigned int val)
{   
	*(__IO uint16_t *) (Bank1_LCD_D)= val; 	
}

void LCD_WR_Data_8(unsigned int val)
{
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}
/****************************************************************************
* Ãû    ³Æ£ºvoid Delay(__IO uint32_t nCount)
* ¹¦    ÄÜ£ºÑÓÊ±º¯Êý
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎÞ 
****************************************************************************/ 
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
//

/****************************************************************************
* Ãû    ³Æ£ºvoid LCD_Init(void)
* ¹¦    ÄÜ£º2.4 ´çTFT ³õÊ¼»¯º¯Êý
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎÞ 
****************************************************************************/ 
void LCD_Init(void)
{	
	unsigned int i;
	
	
// 	Delay(500000);
// 	LCD_WR_REG(0x00FF);
// 	LCD_WR_REG(0x00FF);
// 	Delay(500000);
// 	LCD_WR_REG(0x00FF);
// 	LCD_WR_REG(0x00FF);
// 	LCD_WR_REG(0x00FF);
// 	LCD_WR_REG(0x00FF);
// 	Delay(1000000);
// 	LCD_WR_CMD(0x00B0, 0x0000);
// 	LCD_WR_REG(0x0011);
// 	Delay(15000000);
// 	LCD_WR_CMD(0x00B3, 0x0002);
// 	LCD_WR_CMD(0x00B3, 0x0000);
// 	LCD_WR_CMD(0x00B3, 0x0000);
// 	LCD_WR_CMD(0x00B3, 0x0000);
// 	
// 	LCD_WR_CMD(0x00C0, 0x0010);//13
// 	LCD_WR_CMD(0x00C0, 0x003B);//480
// 	LCD_WR_CMD(0x00C0, 0x0000);
// 	LCD_WR_CMD(0x00C0, 0x0002);
// 	LCD_WR_CMD(0x00C0, 0x0000);
// 	LCD_WR_CMD(0x00C0, 0x0001);
// 	LCD_WR_CMD(0x00C0, 0x0000);//NW
// 	LCD_WR_CMD(0x00C0, 0x0043);
// 	
// 	LCD_WR_CMD(0x00C1, 0x0010);
// 	LCD_WR_CMD(0x00C1, 0x0010);
// 	LCD_WR_CMD(0x00C1, 0x0008);
// 	LCD_WR_CMD(0x00C1, 0x0008);

// 	LCD_WR_CMD(0x00C4, 0x0011);
// 	LCD_WR_CMD(0x00C4, 0x0007);
// 	LCD_WR_CMD(0x00C4, 0x0003);
// 	LCD_WR_CMD(0x00C4, 0x0003);
// 	
// 	LCD_WR_CMD(0x00C6, 0x0000);
// 	
// 	LCD_WR_CMD(0x00C8, 0x0003);
// 	LCD_WR_CMD(0x00C8, 0x0003);
// 	LCD_WR_CMD(0x00C8, 0x0013);
// 	LCD_WR_CMD(0x00C8, 0x005C);
// 	LCD_WR_CMD(0x00C8, 0x0003);
// 	LCD_WR_CMD(0x00C8, 0x0007);
// 	LCD_WR_CMD(0x00C8, 0x0014);
// 	LCD_WR_CMD(0x00C8, 0x0008);
// 	LCD_WR_CMD(0x00C8, 0x0000);
// 	LCD_WR_CMD(0x00C8, 0x0021);
// 	LCD_WR_CMD(0x00C8, 0x0008);
// 	LCD_WR_CMD(0x00C8, 0x0014);
// 	LCD_WR_CMD(0x00C8, 0x0007);
// 	LCD_WR_CMD(0x00C8, 0x0053);
// 	LCD_WR_CMD(0x00C8, 0x000C);
// 	LCD_WR_CMD(0x00C8, 0x0013);
// 	LCD_WR_CMD(0x00C8, 0x0003);
// 	LCD_WR_CMD(0x00C8, 0x0003);
// 	LCD_WR_CMD(0x00C8, 0x0021);
// 	LCD_WR_CMD(0x00C8, 0x0000);
// 	
// 	LCD_WR_CMD(0x0035, 0x0000);
// 	
// 	LCD_WR_CMD(0x0036, 0x0008);
// 	
// 	LCD_WR_CMD(0x003A, 0x0055);
// 	
// 	LCD_WR_CMD(0x0044, 0x0000);
// 	LCD_WR_CMD(0x0044, 0x0001);
// 	
// 	LCD_WR_CMD(0x00D0, 0x0007);
// 	LCD_WR_CMD(0x00D0, 0x0007);
// 	LCD_WR_CMD(0x00D0, 0x001D);
// 	
// 	LCD_WR_CMD(0x00D1, 0x0000);
// 	LCD_WR_CMD(0x00D1, 0x0003);
// 	LCD_WR_CMD(0x00D1, 0x0000);
// 	
// 	LCD_WR_CMD(0x00D2, 0x0003);
// 	LCD_WR_CMD(0x00D2, 0x0014);
// 	LCD_WR_CMD(0x00D2, 0x0004);
// 	
// 	LCD_WR_REG(0x0029);
// 	Delay(3000000);
// 	LCD_WR_CMD(0x002A, 0x0000);
// 	LCD_WR_CMD(0x002A, 0x0000);
// 	LCD_WR_CMD(0x002A, 0x0001);
// 	LCD_WR_CMD(0x002A, 0x003F);
// 	
// 	LCD_WR_CMD(0x002B, 0x0000);
// 	LCD_WR_CMD(0x002B, 0x0000);
// 	LCD_WR_CMD(0x002B, 0x0001);
// 	LCD_WR_CMD(0x002B, 0x00DF);
// 	
// 	LCD_WR_CMD(0x00B4, 0x0000);
// 	Delay(10000000);
	
	
	
	
	
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
void ili9481_SetCursor(u16 x,u16 y)//ÉèÖÃ¹â±ê
{
	LCD_WR_REG(0x002A);
  
	LCD_WR_Data((y)>>8);
	LCD_WR_Data(0x00FF&(y));

//  	LCD_WR_Data((320-y)>>8);
//  	LCD_WR_Data(0x00FF&(320-y));
	
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

void ili9481_DrawLine_x(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 point)
{
	u32  n;
	ili9481_SetCursor(x_start,y_start);

    LCD_WR_REG(0x002C);

	for(n=x_start;n<=x_end;n++)
	{		
    	LCD_WR_Data(0XFFFF-point);
  	}
  
}

void ili9481_fill(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 color)//Ìî³ä¾ØÐÎÇøÓòÑÕÉ«
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

void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 col,u8 mode)
{     
	y = 320-y;
    while(*p!='\0')
    {       
        if(x>479&&y>26){x=0;y-=16;}
        if(y<16){y=16;x=0;ili9481_Clear(WHITE);}
        LCD_ShowChar(x,y,*p,12,col,mode);
        x+=12;
        p++;
    }  
}

void LCD_ShowString_BIG(u16 x,u16 y,const u8 *p,u16 col)
{    
		y = 320-y;
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

	for(n=0;n<262144;n++)
	{
		LCD_WR_Data(0XFFFF-dat);					 	
	}

}

   void LCD_Clear(void)
	 { 
		 ili9481_fill(0, 0, 480, 320, 1);
// 		 u16 color=MAGENTA;
// 		 ili9481_Clear(WHITE);
// 		 LCD_ShowString_BIG(352,0,  "|~~~~~~|",color);
// 	   LCD_ShowString_BIG(352,32, "| XJTU |",color);
// 	   LCD_ShowString_BIG(352,64, "| ROBOT|",color);
// 	   LCD_ShowString_BIG(352,96, "| TEAM |",color);
// 	   LCD_ShowString_BIG(352,128,"|------|",color);
// 	   LCD_ShowString_BIG(352,160,"| ^-^  |",color);
// 	   LCD_ShowString_BIG(352,192,"|  TO  |",color);
// 	   LCD_ShowString_BIG(352,224,"|  BE  |",color);
// 	   LCD_ShowString_BIG(352,256,"| NO.1 |",color);
// 	   LCD_ShowString_BIG(352,288,"|______|",color);
   }
	 
void LCD_TEST_Picture3(unsigned char *picture1)
{
//     unsigned	char i,j;
//
//		ili9481_SetCursor(0,0);
//  
//    	LCD_WR_REG(0x002C);
//       	 
//			for (j=0;j<260000;j++)
//			{
//				
//              LCD_WR_Data(picture1[j]);                  
//			}
		 

}




/****************************************************************************
* Ãû    ³Æ£ºvoid TFT_CLEAR(u8 x,u16 y,u8 len,u16 wid)
* ¹¦    ÄÜ£º2.4 ´çTFT ÇåÆÁº¯Êý
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎÞ 
****************************************************************************/ 
void TFT_CLEAR(u8 x,u16 y,u8 len,u16 wid)
{                    
    u32 n;	
	LCD_WR_CMD(0x0050, x); 				//´°¿ÚµÄÆðÊ¼X
	LCD_WR_CMD(0x0051, x+len-1); 		//´°¿ÚµÄ½áÊøX
	LCD_WR_CMD(0x0052, y);              //´°¿ÚµÄÆðÊ¼Y
	LCD_WR_CMD(0x0053, y+wid-1); 		//´°¿ÚµÄ½áÊøY
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




///****************************************************************************
//* Ãû    ³Æ£ºvoid TFT_DrawPoint(u8 x,u16 y)
//* ¹¦    ÄÜ£º2.4 ´çTFT »­µãº¯Êý
//* Èë¿Ú²ÎÊý£ºÎÞ
//* ³ö¿Ú²ÎÊý£ºÎÞ
//* Ëµ    Ã÷£º
//* µ÷ÓÃ·½·¨£ºÎÞ 
//****************************************************************************/ 
//void TFT_DrawPoint(u8 x,u16 y)
//{
//    
//	LCD_WR_CMD(32, x);					//ÆðÊ¼×ø±êX
//    LCD_WR_CMD(33, y);					//ÆðÊ¼×ø±êY
//	LCD_WR_REG(34);				   
//	LCD_WR_Data(POINT_COLOR); 	   	
//} 
///****************************************************************************
//* Ãû    ³Æ£ºu32 abs(s32 res)
//* ¹¦    ÄÜ£ºÈ¡¾ø¶ÔÖµº¯Êý
//* Èë¿Ú²ÎÊý£ºÎÞ
//* ³ö¿Ú²ÎÊý£ºÎÞ
//* Ëµ    Ã÷£º
//* µ÷ÓÃ·½·¨£ºÎÞ 
//****************************************************************************/ 
////u32 abs(s32 res)
////{
////	if(res<0)return -res;
////	else return res;
////}  
//
///****************************************************************************
//* Ãû    ³Æ£ºvoid TFT_ShowChar(u8 x,u16 y,u8 num)
//* ¹¦    ÄÜ£ºÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾Ò»¸ö×Ö·û
//* Èë¿Ú²ÎÊý£ºÎÞ
//* ³ö¿Ú²ÎÊý£ºÎÞ
//* Ëµ    Ã÷£º
//* µ÷ÓÃ·½·¨£ºÎÞ 
//****************************************************************************/ 
//void TFT_ShowChar(u8 x,u16 y,u8 num)
//{       
//	#define MAX_CHAR_POSX 232
//	#define MAX_CHAR_POSY 304 
//    u8 temp;
//    u8 pos,t;      
//    if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;
//    //Éè¶¨Ò»¸ö×Ö·ûËùÕ¼µÄ´°¿Ú´óÐ¡ 
//	LCD_WR_CMD(0x0050, x); 				//´°¿ÚµÄÆðÊ¼X
//	LCD_WR_CMD(0x0051, x+5); 			//´°¿ÚµÄ½áÊøX
//	LCD_WR_CMD(0x0052, y);              //´°¿ÚµÄÆðÊ¼Y
//	LCD_WR_CMD(0x0053, y+11); 			//´°¿ÚµÄ½áÊøY	 
//	LCD_WR_CMD(32, x);
//    LCD_WR_CMD(33, y);
//	LCD_WR_REG(34);					   
//	num=num-' ';						//µÃµ½Æ«ÒÆºóµÄÖµ
//	for(pos=0;pos<12;pos++)	{
//	    temp=asc2_1206[num][pos];		//»ñµÃ×ÖÄ£Êý×éµÄÖµ
//	    for(t=0;t<6;t++)
//	    {                 
//	        if(temp&0x01)LCD_WR_Data(POINT_COLOR);	  //Î»Îª1 ÓÃÖ¸¶¨ÑÕÉ«Ð´Èëµ½ÏñËØ
//	        else LCD_WR_Data(0xffff);                 //Î»Îª0 ÓÃ°×É«Ð´Èëµ½ÏñËØ    
//	        temp>>=1; 
//	    }
//	}
//}  	 
///****************************************************************************
//* Ãû    ³Æ£ºu32 mn(u8 m,u8 n)
//* ¹¦    ÄÜ£º
//* Èë¿Ú²ÎÊý£ºÎÞ
//* ³ö¿Ú²ÎÊý£ºÎÞ
//* Ëµ    Ã÷£º
//* µ÷ÓÃ·½·¨£ºÎÞ 
//****************************************************************************/ 
//u32 mn(u8 m,u8 n)
//{
//	u32 result=1;	 
//	while(n--)result*=m;    
//	return result;
//}
///****************************************************************************
//* Ãû    ³Æ£ºvoid TFT_ShowNum(u8 x,u16 y,u32 num)
//* ¹¦    ÄÜ£ºÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾Ò»¸öÊý×Ö0-65536
//* Èë¿Ú²ÎÊý£ºÎÞ
//* ³ö¿Ú²ÎÊý£ºÎÞ
//* Ëµ    Ã÷£º
//* µ÷ÓÃ·½·¨£ºÎÞ 
//****************************************************************************/ 
//void TFT_ShowNum(u8 x,u16 y,u32 num)
//{      
//	u32 res;   	   
//	u8 t=0,t1=0;   
//	res=num;
//	if(!num)TFT_ShowChar(x,y,'0');		//¼ÓÈëÊý×ÖÎª0 ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾0
//	while(res)  						//µÃµ½Êý×Ö³¤¶È
//	{
//		res/=10;
//		t++;
//	}
//	t1=t;
//	while(t)							//ÏÔÊ¾Êý×Ö
//	{
//		res=mn(10,t-1); 	 
//	    TFT_ShowChar(x+(t1-t)*6,y,(num/res)%10+'0');	 //ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾Êý×Ö
//		t--;
//	}				     
//} 
///****************************************************************************
//* Ãû    ³Æ£ºvoid TFT_ShowString(u8 x,u16 y,const u8 *p)
//* ¹¦    ÄÜ£ºÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾×Ö·û´®
//* Èë¿Ú²ÎÊý£ºÎÞ
//* ³ö¿Ú²ÎÊý£ºÎÞ
//* Ëµ    Ã÷£º
//* µ÷ÓÃ·½·¨£ºÎÞ 
//****************************************************************************/ 
//void TFT_ShowString(u8 x,u16 y,const u8 *p)
//{         
//    while(*p!='\0')
//    {       
//        if(x>MAX_CHAR_POSX){x=0;y+=12;}
//        if(y>MAX_CHAR_POSY){y=x=0;TFT_CLEAR(0,0,240,320);}
//        TFT_ShowChar(x,y,*p);
//        x+=6;
//        p++;
//    }  
//}
///****************************************************************************
//* Ãû    ³Æ£ºvoid TFT_ShowBmp(u8 x,u16 y,u8 lenth,u16 wide,const u8 *p)
//* ¹¦    ÄÜ£ºÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾Ò»¸±Í¼Æ¬
//* Èë¿Ú²ÎÊý£ºÎÞ
//* ³ö¿Ú²ÎÊý£ºÎÞ
//* Ëµ    Ã÷£º
//* µ÷ÓÃ·½·¨£ºÎÞ 
//****************************************************************************/ 
//void TFT_ShowBmp(u8 x,u16 y,u8 lenth,u16 wide,const u8 *p)
//{      
//    u32 size,temp; 
//    //Éè¶¨Ò»¸öÍ¼Æ¬ËùÕ¼µÄ´°¿Ú´óÐ¡ 	
//	LCD_WR_CMD(0x0050, x); 				//´°¿ÚµÄÆðÊ¼X
//	LCD_WR_CMD(0x0051, (u16)x+lenth-1); //´°¿ÚµÄ½áÊøX
//	LCD_WR_CMD(0x0052, y);              //´°¿ÚµÄÆðÊ¼Y
//	LCD_WR_CMD(0x0053, y+wide-1); 		//´°¿ÚµÄ½áÊøY	 
//	LCD_WR_CMD(32, x);
//    LCD_WR_CMD(33, y);
//	LCD_WR_REG(34);					   
//
//	temp=(u32)lenth*wide*2;
//	for(size=0;size<temp;size++)LCD_WR_Data_8(p[size]); 
//}         
//
///******************* (C) COPYRIGHT 2011 ·Ü¶·STM32 *****END OF FILE****/
