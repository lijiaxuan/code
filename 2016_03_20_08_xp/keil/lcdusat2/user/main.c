/****************************************************************************
* Copyright (C), 2011 �ܶ�Ƕ��ʽ������ www.ourstm.net
*
* �������� �ܶ���STM32������V2,2.1,V3�ϵ���ͨ��           
* QQ: 9191274, ������sun68, Email: sun68@163.com 
* �Ա����̣�ourstm.taobao.com  
*
* �ļ���: main.c
* ���ݼ���:
*     2.4��������У�����õ�ɫ�����ɫ��ͼ�� ��ʾ����240X320
		
*
* �ļ���ʷ:
* �汾��  ����       ����    ˵��
* v0.2    2011-07-21 sun68  �������ļ�
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
* ��    �ƣ�void RCC_Configuration(void)
* ��    �ܣ�ϵͳʱ������Ϊ72MHZ�� ����ʱ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void RCC_Configuration(void){

  //SystemInit();	  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC 
  						| RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

/****************************************************************************
* ��    �ƣ�void GPIO_Configuration(void)
* ��    �ܣ�ͨ��IO������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/  
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
		/*����GPIOC������ʱ��*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10| GPIO_Pin_11;				     //״̬LED1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //ͨ���������ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���ģʽ����ٶ�50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_SetBits(GPIOB,  GPIO_Pin_11);	
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;				 //�����������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

}
/****************************************************************************
* ��    �ƣ�void Delay_us(__IO uint32_t nTime)
* ��    �ܣ���ʱ��ʱ���� 10usΪ��λ
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/  
void Delay_us(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/****************************************************************************
* ��    �ƣ�void TimingDelay_Decrement(void)
* ��    �ܣ���ȡ���ĳ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
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
    
    //��ʼ��
    RCC_DeInit();
    
    //ʹ���ⲿ8Hz����
    RCC_HSEConfig(RCC_HSE_ON);
    
    HSEStartUpStatus=RCC_WaitForHSEStartUp();
    
    if(HSEStartUpStatus==SUCCESS)
    {
        //SYSCLK����������HCLK
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        //HCLK����������PCLK1��PCLK2
        RCC_PCLK2Config(RCC_HCLK_Div1);
        RCC_PCLK1Config(RCC_HCLK_Div1);
    	
        //���໷�� HSE 9��Ƶ(72MHZ)
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);
        //���໷ʹ��
        RCC_PLLCmd(ENABLE);
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)
    	{}
    	
    //SYSCLKΪ���໷���    		
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    		
        while(RCC_GetSYSCLKSource()!=0x08)
    	{}
    }
    else
    {
        //��ʾHSE��ʼ��ʧ��
    }
}
/****************************************************************************
* ��    �ƣ�int main(void)
* ��    �ܣ��ܶ����ʼ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/  



int main(void)
{
//  unsigned short a;
int a;
u16 color;

  System_Clock_Config();
  RCC_Configuration();  	       //ϵͳʱ�ӳ�ʼ�����˿�����ʱ��ʹ��
  GPIO_Configuration();			   //״̬LED1�ĳ�ʼ�� 
	

  FSMC_LCD_Init();				   //FSMC TFT�ӿڳ�ʼ��
  uart_init(115200);
  LCD_Init();					   //LCD��ʼ������
  Delay(30000);	
  //TFT_CLEAR(0,0,20,320);          //����
	CH452_IO_Init();
	CH452_Init();
	
	EXTI_5Init();
  					     
//  if (SysTick_Config(720))		   //ʱ�ӽ����ж�ʱ10usһ��  ���ڶ�ʱ 
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
// 		while(a <=1800)                            //�ж�ģʽ
//     	{
//         	a++;
//             Delay(10000);
//             if(flag_COMPUTER==1)
// 							break;      //���������룬��ת��ARMģʽ
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
// 			��s��ǰn���ֽ� ��typedef unsigned int size_t���� ch �滻������ s 	*/
// 		  memset(USART_RX_BUF,0,11);//����
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
* ��    �ƣ�void tp_Config(void)
* ��    �ܣ�TFT ���������Ƴ�ʼ��
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void tp_Config(void) 
{ 
//  GPIO_InitTypeDef  GPIO_InitStructure; 
//  SPI_InitTypeDef   SPI_InitStructure; 
//
//  /* SPI1 ʱ��ʹ�� */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE); 
// 
//  /* SPI1 SCK(PA5)��MISO(PA6)��MOSI(PA7) ���� */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//�����ٶ�50MHZ
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	        //����ģʽ
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//  /* SPI1 ����оƬ��Ƭѡ�������� PB7 */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//�����ٶ�50MHZ 
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//�������ģʽ
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//  
//  
//  /* ����SPI1�����Ϲҽ���4�����裬������ʹ�ô�����ʱ����Ҫ��ֹ����3��SPI1 ���裬 ������������ */  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;           		//SPI1 SST25VF016BƬѡ 
//  GPIO_Init(GPIOC, &GPIO_InitStructure);
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                //SPI1 VS1003Ƭѡ 
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                 //SPI1 ����ģ��Ƭѡ
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//  GPIO_SetBits(GPIOC, GPIO_Pin_4);							//SPI CS1
//  GPIO_SetBits(GPIOB, GPIO_Pin_12);							//SPI CS4
//  GPIO_SetBits(GPIOA, GPIO_Pin_4);							//SPI NSS	   
//  
//   /* SPI1���� ���� */ 
//  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //ȫ˫��  
//  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						   //��ģʽ
//  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					   //8λ
//  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;						   //ʱ�Ӽ��� ����״̬ʱ��SCK���ֵ͵�ƽ
//  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						   //ʱ����λ ���ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ
//  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							   //�������NSS
//  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;  //�����ʿ��� SYSCLK/64
//  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //���ݸ�λ��ǰ
//  SPI_InitStructure.SPI_CRCPolynomial = 7;							   //CRC����ʽ�Ĵ�����ʼֵΪ7 
//  SPI_Init(SPI1, &SPI_InitStructure);
//  
//  /* SPI1 ʹ�� */  
//  SPI_Cmd(SPI1,ENABLE);  
//}
//
///****************************************************************************
//* ��    �ƣ�unsigned char SPI_WriteByte(unsigned char data) 
//* ��    �ܣ�SPI1 д����
//* ��ڲ�������
//* ���ڲ�������
//* ˵    ����
//* ���÷�����
//****************************************************************************/  
//unsigned char SPI_WriteByte(unsigned char data) 
//{ 
// unsigned char Data = 0; 
//
//  //�ȴ����ͻ�������
//  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET); 
//  // ����һ���ֽ�  
//  SPI_I2S_SendData(SPI1,data); 
//
//   //�ȴ��Ƿ���յ�һ���ֽ� 
//  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET); 
//  // ��ø��ֽ�
//  Data = SPI_I2S_ReceiveData(SPI1); 
//
//  // �����յ����ֽ� 
//  return Data; 
}  


/****************************************************************************
* ��    �ƣ�u16 TPReadX(void) 
* ��    �ܣ�������X�����ݶ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/  
//u16 TPReadX(void)
//{ 
////   u16 x=0;
////   TP_CS();	                        //ѡ��XPT2046 
////   Delay(20);						//��ʱ
////   SPI_WriteByte(0x90);				//����X���ȡ��־
////   Delay(20);						//��ʱ
////   x=SPI_WriteByte(0x00);			//������ȡ16λ������ 
////   x<<=8;
////   x+=SPI_WriteByte(0x00);
////   Delay(20);						//��ֹXPT2046
////   TP_DCS(); 					    								  
////   x = x>>3;						//��λ�����12λ����Ч����0-4095
////   return (x);
//}
/****************************************************************************
* ��    �ƣ�u16 TPReadY(void)
* ��    �ܣ�������Y�����ݶ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/
//u16 TPReadY(void)
//{
////   u16 y=0;
////   TP_CS();	                        //ѡ��XPT2046 
////   Delay(20);					//��ʱ
////   SPI_WriteByte(0xD0);				//����Y���ȡ��־
////   Delay(20);					//��ʱ
////   y=SPI_WriteByte(0x00);			//������ȡ16λ������ 
////   y<<=8;
////   y+=SPI_WriteByte(0x00);
////   Delay(20);					//��ֹXPT2046
////   TP_DCS(); 					    								  
////   y = y>>3;						//��λ�����12λ����Ч����0-4095
////   return (y);
//}

/****************************************************************************
* ��    �ƣ�void drawbigpoint(u8 x,u16 y)
* ��    �ܣ�����Ļ�ϻ�һ��2X2�Ĵ��
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/
void drawbigpoint(u8 x,u16 y)
{
//	if(x>220&&y<12)						       //�������������Ͻ�CLR�ַ�����ִ������ָ��
//	{
//		TFT_CLEAR(0,30,240,290);               //���� 
//    	//TFT_ShowString(220,0,"CLR");         //��ʾ��������
//	}
//	else if(y==0||x==0){					   //�����һ��У��������ʾ������
//		TFT_DrawPoint(x,y);//���ĵ� 
//		TFT_DrawPoint(x+1,y);
//		TFT_DrawPoint(x,y+1);
//		TFT_DrawPoint(x+1,y+1);	
//	}		
//	else if(x<180&&y<30)					   //�����ɫ�����򣬸��ݵ��������ѡȡ��Ӧ����ɫ			
//	{
//	    if(x<=30) POINT_COLOR=RED;
//		else if(x>60,x<=60) POINT_COLOR=BLUE;
//		else if(x>90,x<=90) POINT_COLOR=GREEN;
//		else if(x>120,x<=120) POINT_COLOR=GRED;
//		else if(x>150,x<=150) POINT_COLOR=BRED;	
//		else if(x>180,x<=180) POINT_COLOR=BLACK;
//	}
//	else if(y>30){								//����ڶ���У��������ʾ������
//		TFT_DrawPoint(x,y);
//		TFT_DrawPoint(x+1,y);
//		TFT_DrawPoint(x,y+1);
//		TFT_DrawPoint(x+1,y+1);	
//	}		  	
}
/****************************************************************************
* ��    �ƣ�void FillColor(u8 x,u16 y, u8 x1, u16 y1, u16 z)
* ��    �ܣ�����Ļ�ϻ���ɫ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
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
* ��    �ƣ�void Read_XPT2046(void)
* ��    �ܣ���ȡ�������꣬���в������㣬������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/ 
void Read_XPT2046(void)
{	
//	float X1,Y1;    
//	u8 t,t1,count=0;		
//	u16 databuffer[2][30];    				         //�������������������
//	u16 temp=0;	 
//    do{					                             //ѭ������30��		
//		X=TPReadX();		   
//		Y=TPReadY();
//		if(X>100&&X<4000&&Y>100&&Y<4000)             //������ڴ�����ʾ��Ч����Χ��ֵ����ʾ�˶�����Ч
//		{	  
//			databuffer[0][count]=X;
//			databuffer[1][count]=Y;
//			count++;  
//		} 		
//	}while((PEN==0)&&count<30); 			  		 
//	if(count==30)                                    //ÿ�ζ���һ��Ҫ����30������,������
//	{  
//	    do                                            //������X��������
//		{	
//			t1=0;		  
//			for(t=0;t<count-1;t++)
//			{
//				if(databuffer[0][t]>databuffer[0][t+1])//��������
//				{
//					temp=databuffer[0][t+1];
//					databuffer[0][t+1]=databuffer[0][t];
//					databuffer[0][t]=temp;
//					t1=1; 
//				}  
//			}
//		}while(t1); 	  
//		do                                              //������Y��������
//		{	
//			t1=0;		 
//			for(t=0;t<count-1;t++)
//			{
//				if(databuffer[1][t]>databuffer[1][t+1])//��������
//				{
//					temp=databuffer[1][t+1];
//					databuffer[1][t+1]=databuffer[1][t];
//					databuffer[1][t]=temp;
//					t1=1;	 
//				}  
//			}
//		}while(t1);
//	}			
//	/* ����������������м��ȡ������10�����ݣ�����ȡƽ��ֵ,��ýϸߵľ��� */
//	X1=0; Y1=0;
//	for(count=10;count<20; count++){
//		X1=X1+databuffer[0][count];
//		Y1=Y1+databuffer[1][count];
//	}	
//	X1=X1/10;  Y1=Y1/10;
//	if(X1<=4096&&Y1<=4096)                       //��������ֵӦС��4096
//	{	
//		X=X1;
//		Y=Y1;									 
//		if(dw==1){			 					 //���������У������
//			if(X1>=Xs)X1=0;						 //�Ըõ�Ĵ���Xֵ���н����б�������ڵ�һ��У����Ĵ���Xֵ���õ��У����Ĳ�ֵ��ʼ��Ϊ0�������й�)
//			else X1=Xs-X1;						 //���С�ڵ�һ��У����Ĵ���Xֵ��������õ��У����Ĳ�ֵ�������й�)
//			if(Y1<=Ys)Y1=Ys-Y1;					 //���С�ڵ�һ��У����Ĵ���Yֵ��������õ��У����Ĳ�ֵ�������й�)
//			else Y1=0; 							 //�Ըõ�Ĵ���Yֵ���н����б�������ڵ�һ��У����Ĵ���Yֵ���õ��У����Ĳ�ֵ��ʼ��Ϊ0�������й�)
//			X=X1/X2; 							 //����һ�����ض�Ӧ�Ĵ���ֵ�������Ļ�ϵ�ʵ��X����
//			Y=Y1/Y2;							 //����һ�����ض�Ӧ�Ĵ���ֵ�������Ļ�ϵ�ʵ��Y����
//			drawbigpoint(X,Y); 					 //�ڸ����껭��
//		}
//	}
}    
/******************* (C) COPYRIGHT 2011 �ܶ�STM32 *****END OF FILE****/

