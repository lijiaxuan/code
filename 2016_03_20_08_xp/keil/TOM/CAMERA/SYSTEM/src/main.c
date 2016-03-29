#include "stm32f4xx_it.h"
#include "system_clock.h"
#include "vision.h"
#include "ILI9481.h"
#include "MT9M111.h"
#include "can.h"
#include "ext_sram.h"
#include "led.h"
#include "ov7725.h"
#include "stdio.h"
#include "usart.h"

#define panelcolor DARKBLUE
#define UI_Indent_Left 8



extern u16 vsync;
extern u16 find;
extern s16 Result_Dist;
extern int normal_flag;

u16 myCount=0;

u32 frames=0;
extern u32 frame;
extern u32 dcmi;


void SysTick_config(void)
{
    SysTick_Config(SystemCoreClock/1000);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
}




void window_Init()
{
  FillSolidRect(0, 0, 480, 320, RED);//
  delay_ms(20);
  FillSolidRect(0, 0, 480, 320, GREEN);
  delay_ms(20);
  FillSolidRect(0, 0, 480, 320, panelcolor);//
  delay_ms(20);
  FillSolidRect(UI_Indent_Left, IMG_STA_Y-2, 324, 244, WHITE);//Main Window
  delay_ms(20);
  FillSolidRect(UI_Indent_Left+2, IMG_STA_Y, 319, 239, BLACK);//Main Window
  delay_ms(20);
	
  FillSolidRect(348, 8, 124, 104, WHITE);//
  delay_ms(20);
  FillSolidRect(350, 10, 120, 100, BLACK);//Graph Window
  delay_ms(20);
  FillSolidRect(348, 110, 124, 104, WHITE);//
  delay_ms(20);
  FillSolidRect(350, 112, 120, 100, BLACK);//Graph Window
  delay_ms(20);
  FillSolidRect(348, 212, 124, 104, WHITE);//
  delay_ms(20);
  FillSolidRect(350, 214, 120, 100, BLACK);//Graph Window
  delay_ms(20);
}

//一帧摄像头数据为320*240*2字节 153 600字节 2M缓冲区可用10个长度的 FIFO
//DMA传输长度为 0x9600



int main(void) 
{	
	
	char str[20];
	System_Clock_Init();
	SysTick_config();
	LED_Init();
	COM1Init(9600); 	
	CANx_Init();
	ext_sram_init();
	LCD_GPIOInit();	
	ili9481_Init();
	GPIO_SetBits(GPIOC, GPIO_Pin_5);
	ili9481_WriteRegister(0x0036);
	ili9481_WriteData(0x002f);
	window_Init();
	LCD_SetBox(UI_Indent_Left+2,IMG_STA_Y,UI_Indent_Left+321,IMG_STA_Y+239);
//	while((1!=SENSOR_init()) &&  (1==OV7725_Init()));//MT9M111 or ov7725
    while((1==OV7725_Init()));//MT9M111 or ov7725  
	//OV7725_WriteReg(0x13, 0xfd); //WB_OFF
	set_25fps();
//  	OV7725_WriteReg(0x11, 0x0f); //	Night mode
//  	OV7725_WriteReg(0x0e, 0x65); 
	DCMI_Cmd(ENABLE);
 	Cam_Start();
    GPIOG->BSRRH = GPIO_Pin_6;



	while(1)
	{
		if(frames<dcmi)
		{              

//            imshow();
            Decompose();          
            Sobel();               
            frames++;
            sprintf(str,"frames:%d",frames);	
            LCD_WriteString16_ili9481(8, 8, GREEN,panelcolor,str);

    }
        myCount++;
    
  }
}
