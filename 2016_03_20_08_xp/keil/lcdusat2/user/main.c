/****************************************************************************
* Copyright (C), 2011 奋斗嵌入式工作室 www.ourstm.net
*
* 本例程在 奋斗版STM32开发板V2,2.1,V3上调试通过           
* QQ: 9191274, 旺旺：sun68, Email: sun68@163.com 
* 淘宝店铺：ourstm.taobao.com  
*
* 文件名: main.c
* 内容简述:
*     2.4触摸屏的校正及用调色板的颜色画图。 显示方向240X320
		
*
* 文件历史:
* 版本号  日期       作者    说明
* v0.2    2011-07-21 sun68  创建该文件
*
*/

#define GLOBALS 

#include "includes.h"
#include "demo.h" 
#include "usart.h"  
#include "key.h"
#include "led.h"
#include "module/lcd/lcd_global.h"




void tp_Config(void) ;
u16 TPReadX(void);
u16 TPReadY(void);
u8 GSMBuf[32]={0};


extern void FSMC_LCD_Init(void); 
extern void LCD_Init(void); 
extern unsigned char SPI_Read(u8 reg);
void Delay_us(__IO uint32_t nTime);
extern void Delay(__IO uint32_t nCount);
/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void); 
void FillColor(u8 x,u16 y, u8 x1, u16 y1, u16 z);
extern void Delay(__IO uint32_t nCount);
void Read_XPT2046(void);
void drawbigpoint(u8 x,u16 y);
void TimingDelay_Decrement(void);
//lcd
extern void TFTSetXY(u16 x,u16 y);     
extern void ili9481_SetCursor(u16 x,u16 y);
extern void ili9481_SetPoint(u16 x,u16 y,u16 point);
extern void ili9481_Clear(u16 dat);
extern void TFT_CLEAR(u8 x,u16 y,u8 len,u16 wid);
extern void ili9481_DrawLine_x(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 point);
extern void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u16 color,u8 mode);
extern void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 col,u8 mode);
extern void LCD_ShowChar_BIG(u16 x,u16 y,u8 num,u8 size,u16 color);
extern void LCD_ShowString_BIG(u16 x,u16 y,const u8 *p,u16 col);
extern void LCD_TEST_Picture3(unsigned char *picture1);
extern unsigned char picture1[];
extern void ili9481_fill(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 color);
//uart
extern u8 USART_RX_BUF[11];
extern u8 flag_tx;
extern char USART_position;
extern u8 flag_position_main;
extern u8 flag_position;
extern u8 flag_COMPUTER;
extern u8 flag_LED;
//LCD_positon
u16 pos_x=0;
u16 pos_y=0;
unsigned char dw=0;
unsigned int Xs=0,Xe=0,Ys=0,Ye=0;
unsigned int X=0,Y=0;
extern u16 POINT_COLOR; 
extern u32 DeviceCode;
float X2=0,Y2=0;  
static __IO uint32_t TimingDelay;
/****************************************************************************
* 名    称：void RCC_Configuration(void)
* 功    能：系统时钟配置为72MHZ， 外设时钟配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void RCC_Configuration(void){

  //SystemInit();	  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC 
  						| RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

/****************************************************************************
* 名    称：void GPIO_Configuration(void)
* 功    能：通用IO口配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/  
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
		/*开启GPIOC的外设时钟*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10| GPIO_Pin_11;				     //状态LED1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //通用推挽输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //输出模式最大速度50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_SetBits(GPIOB,  GPIO_Pin_11);	
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;				 //触摸检测引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

}
/****************************************************************************
* 名    称：void Delay_us(__IO uint32_t nTime)
* 功    能：定时延时程序 10us为单位
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/  
void Delay_us(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/****************************************************************************
* 名    称：void TimingDelay_Decrement(void)
* 功    能：获取节拍程序
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/  
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}


void U32TOSTR(u32 input)
{
	u32 rec,count;
	count=9;
	rec=input;
	GSMBuf[count]=rec%10+48;
	
	while(rec/10>0)
	{
		rec=rec/10;
		count--;
		GSMBuf[count]=rec%10+48;
	}
	
}	

void U16TOSTR(u16 input)
{
	u16 rec,count;
	count=9;
	rec=input;
	GSMBuf[count]=rec%10+48;
	
	while(rec/10>0)
	{
		rec=rec/10;
		count--;
		GSMBuf[count]=rec%10+48;
	}
	
}

void pos_2_lcd(char input)
{
	 pos_x=((input)%30)*16;
	 pos_y=((input)/30)*32;
	
}
void System_Clock_Config(void)
{
    ErrorStatus HSEStartUpStatus;
    
    //初始化
    RCC_DeInit();
    
    //使能外部8Hz晶振
    RCC_HSEConfig(RCC_HSE_ON);
    
    HSEStartUpStatus=RCC_WaitForHSEStartUp();
    
    if(HSEStartUpStatus==SUCCESS)
    {
        //SYSCLK基础上设置HCLK
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        //HCLK基础上设置PCLK1和PCLK2
        RCC_PCLK2Config(RCC_HCLK_Div1);
        RCC_PCLK1Config(RCC_HCLK_Div1);
    	
        //锁相环对 HSE 9倍频(72MHZ)
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);
        //锁相环使能
        RCC_PLLCmd(ENABLE);
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)
    	{}
    	
    //SYSCLK为锁相环输出    		
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    		
        while(RCC_GetSYSCLKSource()!=0x08)
    	{}
    }
    else
    {
        //提示HSE初始化失败
    }
}
/****************************************************************************
* 名    称：int main(void)
* 功    能：奋斗板初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/  



int main(void)
{
//  unsigned short a;
int a;
u16 color;

  System_Clock_Config();
  RCC_Configuration();  	       //系统时钟初始化及端口外设时钟使能
  GPIO_Configuration();			   //状态LED1的初始化 
	

  FSMC_LCD_Init();				   //FSMC TFT接口初始化
  uart_init(115200);
  LCD_Init();					   //LCD初始化代码
  Delay(30000);	
  //TFT_CLEAR(0,0,20,320);          //清屏
	CH452_IO_Init();
	CH452_Init();
	
	EXTI_5Init();
  					     
//  if (SysTick_Config(720))		   //时钟节拍中断时10us一次  用于定时 
//  { 
//    /* Capture error */ 
//    while (1);
//  }
   //ili9481_DrawLine_x(328,16,472,0,YELLOW);
   //LCD_ShowString_BIG(16,0,"ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyz",RED);
   	/*   color=MAGENTA;
	   LCD_ShowString_BIG(320,0,  "|========|",color);
	   LCD_ShowString_BIG(320,32, "|  XJTU  |",color);
	   LCD_ShowString_BIG(320,64, "|  ROBOT |",color);
	   LCD_ShowString_BIG(320,96, "|  TEAM  |",color);
	   LCD_ShowString_BIG(320,128,"|========|",color);
	   LCD_ShowString_BIG(320,160,"| HELLO  |",color);
	   LCD_ShowString_BIG(320,192,"|   TO   |",color);
	   LCD_ShowString_BIG(320,224,"|   BE   |",color);
	   LCD_ShowString_BIG(320,256,"|  NO.1  |",color);
	   LCD_ShowString_BIG(320,288,"|========|",color);
   */

  lcd.init();
	//LCD_ShowString(180, 73, "For Car", WHITE,1);
// 	for(;;)
// 	{
// 		while(a <=1800)                            //判断模式
//     	{
//         	a++;
//             Delay(10000);
//             if(flag_COMPUTER==1)
// 							break;      //有数据输入，跳转到ARM模式
//     	}
// 		if(flag_COMPUTER==1)
// 			break;
// 		else
// 		{
// 			LCD_ShowString_BIG(96,0,"COMPUTER",RED);
// 			Delay(10000);
// 		}
// 	}

   while(1)
   {   
		if(recv_flag == 1)
		{
			  //dealed ++;
			  //parse();
				recv_flag = 0;
		}
        if(lcd.key != 0xff)
        {
            lcd.process(lcd.key);
            lcd.key = 0xff;
        }
  // 		LCD_ShowChar_BIG(300,188,'A',16,RED);
// //   		if(flag_position==1)
// //   	   {
// //	   	  LCD_ShowString_BIG(64,224,"flagposition",RED);	
// //       }
//    	   if(flag_position_main==1)
// 	   {	
// 			pos_2_lcd(USART_position);
// 		  flag_position_main=0;
// 		  //LCD_ShowString_BIG(64,256,"position_done",RED); 
// 		  //LCD_ShowChar_BIG(64,288,USART_position,16,RED);
// 	   }	
// 	   if(flag_tx==1)
// 	   {
// 			LCD_ShowString_BIG(pos_x,pos_y,USART_RX_BUF,RED);
// 		
// 			/*void *memset(void *s, int ch, size_t n);
// 			将s中前n个字节 （typedef unsigned int size_t）用 ch 替换并返回 s 	*/
// 		  memset(USART_RX_BUF,0,11);//清零
// 		  flag_tx=0;      
// 			
// 	   }
// 		 if(flag_LED==1)
// 		 {
// 				LED_Toggle();
// 				Delay(200000);
// 				LED_Toggle();
// 				flag_LED=0;
// 		 }
// 		
   }
	
  //ili9481_Clear(RED);
	//Delay(6000000);
	//ili9481_Clear(YELLOW);
	//Delay(6000000);
	//ili9481_Clear(BLUE);
	//Delay(6000000);
	//ili9481_Clear(GREEN);
	//Delay(6000000);
  //ili9481_Clear(BRED);
	//Delay(6000000);
	//ili9481_Clear(CYAN);
	//Delay(6000000);
	//ili9481_Clear(MAGENTA);
	//Delay(6000000);
	//ili9481_Clear(DARKBLUE);
	//Delay(6000000);  
  
  


   	  
}















/****************************************************************************
* 名    称：void tp_Config(void)
* 功    能：TFT 触摸屏控制初始化
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void tp_Config(void) 
{ 
//  GPIO_InitTypeDef  GPIO_InitStructure; 
//  SPI_InitTypeDef   SPI_InitStructure; 
//
//  /* SPI1 时钟使能 */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE); 
// 
//  /* SPI1 SCK(PA5)、MISO(PA6)、MOSI(PA7) 设置 */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//口线速度50MHZ
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	        //复用模式
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//  /* SPI1 触摸芯片的片选控制设置 PB7 */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//口线速度50MHZ 
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//推挽输出模式
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//  
//  
//  /* 由于SPI1总线上挂接了4个外设，所以在使用触摸屏时，需要禁止其余3个SPI1 外设， 才能正常工作 */  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;           		//SPI1 SST25VF016B片选 
//  GPIO_Init(GPIOC, &GPIO_InitStructure);
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                //SPI1 VS1003片选 
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                 //SPI1 网络模块片选
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//  GPIO_SetBits(GPIOC, GPIO_Pin_4);							//SPI CS1
//  GPIO_SetBits(GPIOB, GPIO_Pin_12);							//SPI CS4
//  GPIO_SetBits(GPIOA, GPIO_Pin_4);							//SPI NSS	   
//  
//   /* SPI1总线 配置 */ 
//  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //全双工  
//  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						   //主模式
//  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					   //8位
//  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;						   //时钟极性 空闲状态时，SCK保持低电平
//  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						   //时钟相位 数据采样从第一个时钟边沿开始
//  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							   //软件产生NSS
//  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;  //波特率控制 SYSCLK/64
//  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //数据高位在前
//  SPI_InitStructure.SPI_CRCPolynomial = 7;							   //CRC多项式寄存器初始值为7 
//  SPI_Init(SPI1, &SPI_InitStructure);
//  
//  /* SPI1 使能 */  
//  SPI_Cmd(SPI1,ENABLE);  
//}
//
///****************************************************************************
//* 名    称：unsigned char SPI_WriteByte(unsigned char data) 
//* 功    能：SPI1 写函数
//* 入口参数：无
//* 出口参数：无
//* 说    明：
//* 调用方法：
//****************************************************************************/  
//unsigned char SPI_WriteByte(unsigned char data) 
//{ 
// unsigned char Data = 0; 
//
//  //等待发送缓冲区空
//  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET); 
//  // 发送一个字节  
//  SPI_I2S_SendData(SPI1,data); 
//
//   //等待是否接收到一个字节 
//  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET); 
//  // 获得该字节
//  Data = SPI_I2S_ReceiveData(SPI1); 
//
//  // 返回收到的字节 
//  return Data; 
}  


/****************************************************************************
* 名    称：u16 TPReadX(void) 
* 功    能：触摸屏X轴数据读出
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/  
//u16 TPReadX(void)
//{ 
////   u16 x=0;
////   TP_CS();	                        //选择XPT2046 
////   Delay(20);						//延时
////   SPI_WriteByte(0x90);				//设置X轴读取标志
////   Delay(20);						//延时
////   x=SPI_WriteByte(0x00);			//连续读取16位的数据 
////   x<<=8;
////   x+=SPI_WriteByte(0x00);
////   Delay(20);						//禁止XPT2046
////   TP_DCS(); 					    								  
////   x = x>>3;						//移位换算成12位的有效数据0-4095
////   return (x);
//}
/****************************************************************************
* 名    称：u16 TPReadY(void)
* 功    能：触摸屏Y轴数据读出
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/
//u16 TPReadY(void)
//{
////   u16 y=0;
////   TP_CS();	                        //选择XPT2046 
////   Delay(20);					//延时
////   SPI_WriteByte(0xD0);				//设置Y轴读取标志
////   Delay(20);					//延时
////   y=SPI_WriteByte(0x00);			//连续读取16位的数据 
////   y<<=8;
////   y+=SPI_WriteByte(0x00);
////   Delay(20);					//禁止XPT2046
////   TP_DCS(); 					    								  
////   y = y>>3;						//移位换算成12位的有效数据0-4095
////   return (y);
//}

/****************************************************************************
* 名    称：void drawbigpoint(u8 x,u16 y)
* 功    能：在屏幕上画一个2X2的大点
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/
void drawbigpoint(u8 x,u16 y)
{
//	if(x>220&&y<12)						       //如果点击的是左上角CLR字符，就执行清屏指令
//	{
//		TFT_CLEAR(0,30,240,290);               //清屏 
//    	//TFT_ShowString(220,0,"CLR");         //显示清屏区域
//	}
//	else if(y==0||x==0){					   //点击第一个校正点所显示的情形
//		TFT_DrawPoint(x,y);//中心点 
//		TFT_DrawPoint(x+1,y);
//		TFT_DrawPoint(x,y+1);
//		TFT_DrawPoint(x+1,y+1);	
//	}		
//	else if(x<180&&y<30)					   //点击调色板区域，根据点击的坐标选取相应的颜色			
//	{
//	    if(x<=30) POINT_COLOR=RED;
//		else if(x>60,x<=60) POINT_COLOR=BLUE;
//		else if(x>90,x<=90) POINT_COLOR=GREEN;
//		else if(x>120,x<=120) POINT_COLOR=GRED;
//		else if(x>150,x<=150) POINT_COLOR=BRED;	
//		else if(x>180,x<=180) POINT_COLOR=BLACK;
//	}
//	else if(y>30){								//点击第二个校正点所显示的情形
//		TFT_DrawPoint(x,y);
//		TFT_DrawPoint(x+1,y);
//		TFT_DrawPoint(x,y+1);
//		TFT_DrawPoint(x+1,y+1);	
//	}		  	
}
/****************************************************************************
* 名    称：void FillColor(u8 x,u16 y, u8 x1, u16 y1, u16 z)
* 功    能：在屏幕上画调色板区域
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/
//void FillColor(u8 x,u16 y, u8 x1, u16 y1, u16 z)
//{	u16 a,b;
//	POINT_COLOR=z; 
//	for(a=0; a<(y1-y); a++){
//	  for(b=0; b<(x1-x); b++){
//	   TFT_DrawPoint(x+b,y+a);
//	  }
//	}	  	
//}
/****************************************************************************
* 名    称：void Read_XPT2046(void)
* 功    能：读取触摸坐标，进行采样计算，并画点
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/ 
void Read_XPT2046(void)
{	
//	float X1,Y1;    
//	u8 t,t1,count=0;		
//	u16 databuffer[2][30];    				         //触摸坐标过采样缓冲区
//	u16 temp=0;	 
//    do{					                             //循环读数30次		
//		X=TPReadX();		   
//		Y=TPReadY();
//		if(X>100&&X<4000&&Y>100&&Y<4000)             //如果是在触摸显示有效区范围的值，标示此读数有效
//		{	  
//			databuffer[0][count]=X;
//			databuffer[1][count]=Y;
//			count++;  
//		} 		
//	}while((PEN==0)&&count<30); 			  		 
//	if(count==30)                                    //每次度数一定要读到30次数据,否则丢弃
//	{  
//	    do                                            //将数据X升序排列
//		{	
//			t1=0;		  
//			for(t=0;t<count-1;t++)
//			{
//				if(databuffer[0][t]>databuffer[0][t+1])//升序排列
//				{
//					temp=databuffer[0][t+1];
//					databuffer[0][t+1]=databuffer[0][t];
//					databuffer[0][t]=temp;
//					t1=1; 
//				}  
//			}
//		}while(t1); 	  
//		do                                              //将数据Y升序排列
//		{	
//			t1=0;		 
//			for(t=0;t<count-1;t++)
//			{
//				if(databuffer[1][t]>databuffer[1][t+1])//升序排列
//				{
//					temp=databuffer[1][t+1];
//					databuffer[1][t+1]=databuffer[1][t];
//					databuffer[1][t]=temp;
//					t1=1;	 
//				}  
//			}
//		}while(t1);
//	}			
//	/* 从排序过的数组里中间抽取连续的10组数据，进行取平均值,获得较高的精度 */
//	X1=0; Y1=0;
//	for(count=10;count<20; count++){
//		X1=X1+databuffer[0][count];
//		Y1=Y1+databuffer[1][count];
//	}	
//	X1=X1/10;  Y1=Y1/10;
//	if(X1<=4096&&Y1<=4096)                       //触摸坐标值应小于4096
//	{	
//		X=X1;
//		Y=Y1;									 
//		if(dw==1){			 					 //如果进行了校正过程
//			if(X1>=Xs)X1=0;						 //对该点的触摸X值进行界限判别，如果大于第一个校正点的触摸X值，该点和校正点的差值初始化为0（和屏有关)
//			else X1=Xs-X1;						 //如果小于第一个校正点的触摸X值，计算出该点和校正点的差值（和屏有关)
//			if(Y1<=Ys)Y1=Ys-Y1;					 //如果小于第一个校正点的触摸Y值，计算出该点和校正点的差值（和屏有关)
//			else Y1=0; 							 //对该点的触摸Y值进行界限判别，如果大于第一个校正点的触摸Y值，该点和校正点的差值初始化为0（和屏有关)
//			X=X1/X2; 							 //根据一个像素对应的触摸值，算出屏幕上的实际X坐标
//			Y=Y1/Y2;							 //根据一个像素对应的触摸值，算出屏幕上的实际Y坐标
//			drawbigpoint(X,Y); 					 //在该坐标画点
//		}
//	}
}    
/******************* (C) COPYRIGHT 2011 奋斗STM32 *****END OF FILE****/

