/* Includes ------------------------------------------------------------------*/
#include "ili9327.h"
#include "stm32f10x_conf.h"
#include "gpio_bb.h"

#define USING_ASM

#define LCD_CS_SET() (GPIOD->BSRR=GPIO_Pin_3)
#define LCD_CS_CLR() (GPIOD->BRR=GPIO_Pin_3)

#define LCD_RS_SET()  (GPIOD->BSRR=GPIO_Pin_0)
#define LCD_RS_CLR()  (GPIOD->BRR=GPIO_Pin_0)

#define LCD_WR_SET()  (GPIOD->BSRR=GPIO_Pin_1)
#define LCD_WR_CLR()  (GPIOD->BRR=GPIO_Pin_1)


#define LCD_RD_SET()  (GPIOD->BSRR=GPIO_Pin_2)
#define LCD_RD_CLR()  (GPIOD->BRR=GPIO_Pin_2)

#define LCD_DATA_OUT()  (GPIOE->CRL=0x33333333)
#define LCD_DATA_IN()   (GPIOE->CRL=0x44444444)
#define LCD_DATA_GET()  (GPIOE->IDR&0xFF)

/*****************************************************************************/
void LCD_GPIO_INIT()
{
  GPIO_InitTypeDef gpio_t;
  /*PE0~PE7  --> D0~D7*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
  gpio_t.GPIO_Mode=GPIO_Mode_Out_PP;
  gpio_t.GPIO_Pin=0x00FF;
  gpio_t.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOE,&gpio_t);
  /*PD0 --> LCD_RS*/
  /*PD1 --> LCD_WR*/
  /*PD2 --> LCD_RD*/
  /*PD3 --> LCD_CS*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
  gpio_t.GPIO_Pin=0x000F;
  GPIO_Init(GPIOD,&gpio_t);
  //LCD CS长期有效
  LCD_CS_CLR();
  LCD_WR_SET();
  LCD_RD_SET();
}



__inline void LCD_WriteCommand(uint8_t cmd)
{
  PUT_PEL(cmd);
  LCD_RS_CLR();
  //LCD_RS_CLR();
  LCD_WR_CLR();
  LCD_WR_SET();
}

__inline void LCD_WriteByte(uint8_t dat)
{
  PUT_PEL(dat);
  LCD_RS_SET();
  //LCD_RS_SET();
  LCD_WR_CLR();
  LCD_WR_SET();
}

__inline uint16_t LCD_ReadData(void)
{
  uint16_t tmp;
  LCD_DATA_IN();
  LCD_RS_SET();
  LCD_RD_CLR();
  tmp=LCD_DATA_GET();
  LCD_RD_SET();
  tmp<<=8;
  LCD_RD_CLR();
  tmp|=LCD_DATA_GET();
  LCD_RD_SET();
  LCD_DATA_OUT();
  return tmp;
}

#ifdef USING_ASM

/*0000hhll*/
/*FFFFHHLL*/
__asm void LCD_WriteData(uint16_t data)
{
  mvn   r1,   r0
  bfi   r0,   r1,   #16,  #16
  lsr   r1,   r0,   #8
  and   r0,   r0,   #0x00FF00FF
  and   r1,   r1,   #0x00FF00FF
  
  ldr   r2,   =__cpp(GPIOD_BASE+0x10)
  str   r1,   [r2,#0x400]
  movs  r1,   #1
  str   r1,   [r2]
  
  lsl   r1,   r1,   #1
  str   r1,   [r2,#0x004]
  str   r1,   [r2,#0x000]
  
  str   r0,   [r2,#0x400]
  str   r1,   [r2,#0x004]
  str   r1,   [r2,#0x000]
  
  bx    lr
}

__asm void LCD_WriteData_C(uint16_t data,uint32_t size)
{
  cbz   r1,   write_data_c_exit
  mvn   r3,   r0
  bfi   r0,   r3,   #16,  #16
  lsr   r3,   r0,   #8
  and   r0,   r0,   #0x00FF00FF
  and   r3,   r3,   #0x00FF00FF
  
  ldr   r2,   =__cpp(GPIOD_BASE+0x10)
  
  mov   r12,  #1
  str   r12,  [r2]
  lsl   r12,  r12,   #1
write_data_c_loop  
  
  str   r3,   [r2,#0x400]
  str   r12,  [r2,#0x004]
  str   r12,  [r2,#0x000]
  
  str   r0,   [r2,#0x400]
  str   r12,  [r2,#0x004]
  str   r12,  [r2,#0x000]
  
  subs  r1,   r1,   r12,  lsr #1
  bne     write_data_c_loop
write_data_c_exit
  bx    lr
}

__asm void LCD_WriteData_V(uint16_t * data,uint32_t size)
{
  push  {r4}
  cbz   r1,   write_data_v_exit
  ldr   r2,   =__cpp(GPIOD_BASE+0x10)
  mov   r4,   r0

write_data_v_loop

  ldrh  r0,   [r4], #2

  mvn   r3,   r0
  bfi   r0,   r3,   #16,  #16
  lsr   r3,   r0,   #8
  and   r0,   r0,   #0x00FF00FF
  and   r3,   r3,   #0x00FF00FF
  
  
  str   r3,   [r2,#0x400]
  movs  r12,  #1
  str   r12,  [r2]
  
  lsl   r12,  r12,   #1
  str   r12,  [r2,#0x004]
  str   r12,  [r2,#0x000]
  
  str   r0,   [r2,#0x400]
  str   r12,  [r2,#0x004]
  str   r12,  [r2,#0x000]
  
  sub   r1,   r1,   #1
  cbz   r1,   write_data_v_exit
  b     write_data_v_loop
write_data_v_exit
  pop   {r4}
  bx    lr
}


#else

void LCD_WriteData(uint16_t data)
{
  PUT_PEL(data>>8);
  LCD_RS_SET();
  //
  LCD_WR_CLR();
  LCD_WR_SET();
  PUT_PEL(data&0xFF);
  //LCD_RS_SET();
  LCD_WR_CLR();
  LCD_WR_SET();
}

void LCD_WriteData_C(uint16_t data,uint32_t size)
{
  LCD_RS_SET();
  while(size--)
  {
    PUT_PEL(data>>8);
    LCD_WR_CLR();
    LCD_WR_SET();
    PUT_PEL(data&0xFF);
    LCD_WR_CLR();
    LCD_WR_SET();
  }
}


void LCD_WriteData_V(uint16_t * data,uint32_t size)
{
  LCD_RS_SET();
  while(size--)
  {
    PUT_PEL(data[0]>>8);
    LCD_WR_CLR();
    LCD_WR_SET();
    PUT_PEL(data[0]&0xFF);
    LCD_WR_CLR();
    LCD_WR_SET();
    data++;
  }
}
#endif

void LCD_WriteReg(uint8_t idx,uint8_t val)
{
  LCD_WriteCommand(idx);
  LCD_WriteByte(val);
}

#define LCD_WriteRAM_Prepare()  LCD_WriteCommand(0x2C);


void LCD_SetWindow(int x1,int y1,int x2,int y2)
{
  LCD_WriteCommand(0x2A);
  LCD_WriteData(x1+32);
  LCD_WriteData(x2+32);
  
  LCD_WriteCommand(0x2B);
  LCD_WriteData(y1);
  LCD_WriteData(y2);
  
  LCD_WriteRAM_Prepare();
}

void LCD_Init(void)
{
  int i;
  LCD_GPIO_INIT();
  for(i=0;i<1000000;i++);

  LCD_WriteCommand(0x00e9);
  LCD_WriteByte(0x0020);     //Exit_invert_mode
  LCD_WriteCommand(0x0011);     //Exit_sleep_mode 
  for(i=0;i<1000000;i++);

  LCD_WriteCommand(0x00d1);
  LCD_WriteByte(0x0000);
  LCD_WriteByte(0x0071);
  LCD_WriteByte(0x0019);
  LCD_WriteCommand(0x00d0);
  LCD_WriteByte(0x0007);
  LCD_WriteByte(0x0001);
  LCD_WriteByte(0x0008);

  LCD_WriteCommand(0x0036);   //Set_address_mode
  LCD_WriteByte(0x00E8);      //
	 
  LCD_WriteCommand(0x003a);   //Set_pixel_format
  LCD_WriteByte(0x0055);      //DBI 65K colors

  LCD_WriteCommand(0x00C1);
  LCD_WriteByte(0x0010);
  LCD_WriteByte(0x0010);
  LCD_WriteByte(0x0002);
  LCD_WriteByte(0x0002);
  LCD_WriteCommand(0x00C0);//Set Default Gamma
  LCD_WriteByte(0x0000);
  LCD_WriteByte(0x0035);
  LCD_WriteByte(0x0000);
  LCD_WriteByte(0x0000);
  LCD_WriteByte(0x0001);
  LCD_WriteByte(0x0002);
  LCD_WriteCommand(0x00C5); //Set frame rate
  LCD_WriteByte(0x0004);
  LCD_WriteCommand(0x00D2); //power setting
  LCD_WriteByte(0x0001);
  LCD_WriteByte(0x0044);
  LCD_WriteCommand(0x00C8); //Set Gamma
  LCD_WriteByte(0x0004);
  LCD_WriteByte(0x0067);
  LCD_WriteByte(0x0035);
  LCD_WriteByte(0x0004);
  LCD_WriteByte(0x0008);
  LCD_WriteByte(0x0006);
  LCD_WriteByte(0x0024);
  LCD_WriteByte(0x0001);
  LCD_WriteByte(0x0037);
  LCD_WriteByte(0x0040);
  LCD_WriteByte(0x0003);
  LCD_WriteByte(0x0010);
  LCD_WriteByte(0x0008);
  LCD_WriteByte(0x0080);
  LCD_WriteByte(0x0000);
 
  LCD_Clear(0x001F);
  LCD_WriteCommand(0x0029);
}

void LCD_Clear(uint16_t color)
{
  LCD_SetWindow(0,0,399,239);
  LCD_WriteData_C(color,400*240);
} 
//Set Pixel
void LCD_Point(uint16_t x,uint16_t y,uint16_t color)
{
  LCD_SetWindow(x,y,x,y);
  LCD_WriteData(color);  
}

void LCD_VLine(uint16_t x,uint16_t sy,uint16_t ey,uint16_t color)
{
  LCD_SetWindow(x,sy,x,ey);
  LCD_WriteData_C(color,ey-sy+1);
}
void LCD_HLine(uint16_t sx,uint16_t y,uint16_t ex,uint16_t color)
{
  LCD_SetWindow(sx,y,ex,y);
  LCD_WriteData_C(color,ex-sx+1);
}
void LCD_Bar(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{
  LCD_SetWindow(sx,sy,ex,ey);
  LCD_WriteData_C(color,(ex-sx+1)*(ey-sy+1));
}

void LCD_Line(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
	short x0,y0,dx,dy;
	dx=(x2>=x1)?(x2-x1):(x1-x2);
	dy=(y2>=y1)?(y2-y1):(y1-y2);
	if(dx>=dy)//??x????
	{
		if(dx)
		{
			if(x1>=x2)
			{
				x0=x2;
				y0=y2;
			}
			else
			{
				x0=x1;
				y0=y1;
			}
			do
			{
				LCD_Point(x0+dx,y0+dx*(y2-y1)/(x2-x1),color);
			}while(dx--);
		}
		else LCD_Point(x1,y1,color);
	}
	else//??y????
	{
		if(y1>=y2)
		{
			y0=y2;
			x0=x2;
		}
		else
		{
			y0=y1;
			x0=x1;
		}
		do
		{
			LCD_Point(x0+dy*(x2-x1)/(y2-y1),y0+dy,color);
		}while(dy--);
	}
}

void LCD_Rect(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{
  LCD_VLine(sx,sy,ey,color);
  LCD_VLine(ex,sy,ey,color);
  LCD_HLine(sx,sy,ex,color);
  LCD_HLine(sx,ey,ex,color);  
}

void LCD_GetArea(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t * pgram)
{
//   int y;
//   while(sx<=ex)
//   {
//     for(y=sy;y<=ey;y++)
//     {
//       LCD_SetCursor(sx,y);
//       LCD_WriteRAM_Prepare();
//       //LCD_DummyRead();
//       LCD_RD_DATA();
//       *(pgram++)=LCD_BGR2RGB(LCD_RD_DATA());
//     }
//     sx++;
//   }
}

void LCD_SetArea(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t * pgram)
{
  LCD_SetWindow(sx,sy,ex,ey);
  LCD_WriteData_V(pgram,(ex-sx+1)*(ey-sy+1)); 
}
