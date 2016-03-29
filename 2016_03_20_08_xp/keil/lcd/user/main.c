/****************************************************************************
* Copyright (C), 2011 ·Ü¶·Ç¶ÈëÊ½¹¤×÷ÊÒ www.ourstm.net
*
* ±¾Àý³ÌÔÚ ·Ü¶·°æSTM32¿ª·¢°åV2,2.1,V3ÉÏµ÷ÊÔÍ¨¹ý           
* QQ: 9191274, ÍúÍú£ºsun68, Email: sun68@163.com 
* ÌÔ±¦µêÆÌ£ºourstm.taobao.com  
*
* ÎÄ¼þÃû: main.c
* ÄÚÈÝ¼òÊö:
*     2.4´¥ÃþÆÁµÄÐ£Õý¼°ÓÃµ÷É«°åµÄÑÕÉ«»­Í¼¡£ ÏÔÊ¾·½Ïò240X320
		
*
* ÎÄ¼þÀúÊ·:
* °æ±¾ºÅ  ÈÕÆÚ       ×÷Õß    ËµÃ÷
* v0.2    2011-07-21 sun68  ´´½¨¸ÃÎÄ¼þ
*
*/

#define GLOBALS 

#include "includes.h"
#include "demo.h" 
#include "usart.h"  
#include "key.h"
#include "led.h"





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
extern void LCD_ShowChar_BIG(u16 x,u16 y,u8 num,u8 size,u16 color);
extern void LCD_ShowString_BIG(u16 x,u16 y,const u8 *p,u16 col);
extern void LCD_TEST_Picture3(unsigned char *picture1);
extern unsigned char picture1[];
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
* Ãû    ³Æ£ºvoid RCC_Configuration(void)
* ¹¦    ÄÜ£ºÏµÍ³Ê±ÖÓÅäÖÃÎª72MHZ£¬ ÍâÉèÊ±ÖÓÅäÖÃ
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎÞ 
****************************************************************************/ 
void RCC_Configuration(void){

  //SystemInit();	  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC 
  						| RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

/****************************************************************************
* Ãû    ³Æ£ºvoid GPIO_Configuration(void)
* ¹¦    ÄÜ£ºÍ¨ÓÃIO¿ÚÅäÖÃ
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£º
****************************************************************************/  
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
		/*¿ªÆôGPIOCµÄÍâÉèÊ±ÖÓ*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10| GPIO_Pin_11;				     //×´Ì¬LED1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //Í¨ÓÃÍÆÍìÊä³öÄ£Ê½
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //Êä³öÄ£Ê½×î´óËÙ¶È50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_SetBits(GPIOB,  GPIO_Pin_11);	
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;				 //´¥Ãþ¼ì²âÒý½Å
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

}
/****************************************************************************
* Ãû    ³Æ£ºvoid Delay_us(__IO uint32_t nTime)
* ¹¦    ÄÜ£º¶¨Ê±ÑÓÊ±³ÌÐò 10usÎªµ¥Î»
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎÞ 
****************************************************************************/  
void Delay_us(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/****************************************************************************
* Ãû    ³Æ£ºvoid TimingDelay_Decrement(void)
* ¹¦    ÄÜ£º»ñÈ¡½ÚÅÄ³ÌÐò
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎÞ 
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
    
    //³õÊ¼»¯
    RCC_DeInit();
    
    //Ê¹ÄÜÍâ²¿8Hz¾§Õñ
    RCC_HSEConfig(RCC_HSE_ON);
    
    HSEStartUpStatus=RCC_WaitForHSEStartUp();
    
    if(HSEStartUpStatus==SUCCESS)
    {
        //SYSCLK»ù´¡ÉÏÉèÖÃHCLK
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        //HCLK»ù´¡ÉÏÉèÖÃPCLK1ºÍPCLK2
        RCC_PCLK2Config(RCC_HCLK_Div1);
        RCC_PCLK1Config(RCC_HCLK_Div1);
    	
        //ËøÏà»·¶Ô HSE 9±¶Æµ(72MHZ)
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);
        //ËøÏà»·Ê¹ÄÜ
        RCC_PLLCmd(ENABLE);
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)
    	{}
    	
    //SYSCLKÎªËøÏà»·Êä³ö    		
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    		
        while(RCC_GetSYSCLKSource()!=0x08)
    	{}
    }
    else
    {
        //ÌáÊ¾HSE³õÊ¼»¯Ê§°Ü
    }
}
/****************************************************************************
* Ãû    ³Æ£ºint main(void)
* ¹¦    ÄÜ£º·Ü¶·°å³õÊ¼»¯º¯Êý
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎÞ 
****************************************************************************/  
int main(void)
{
//  unsigned short a;
int a;
u16 color;

  System_Clock_Config();
  RCC_Configuration();  	       //ÏµÍ³Ê±ÖÓ³õÊ¼»¯¼°¶Ë¿ÚÍâÉèÊ±ÖÓÊ¹ÄÜ
  GPIO_Configuration();			   //×´Ì¬LED1µÄ³õÊ¼»¯ 
	

  FSMC_LCD_Init();				   //FSMC TFT½Ó¿Ú³õÊ¼»¯
  
  LCD_Init();					   //LCD³õÊ¼»¯´úÂë
  Delay(30000);	
  //TFT_CLEAR(0,0,20,320);          //ÇåÆÁ
	CH452_IO_Init();
	CH452_Init();
	
	EXTI_5Init();
  //LCD_ShowString(200, 200, "hello", WHITE, 1);    
//  if (SysTick_Config(720))		   //Ê±ÖÓ½ÚÅÄÖÐ¶ÏÊ±10usÒ»´Î  ÓÃÓÚ¶¨Ê± 
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
  
  LCD_Clear();
  uart_init(115200);
	
   while(1)
   {   
    if(recv_flag == 1)
    {
        parse();
        recv_flag = 0;
    }
  // 		LCD_ShowChar_BIG(300,188,'A',16,RED);
//   		if(flag_position==1)
//   	   {
//	   	  LCD_ShowString_BIG(64,224,"flagposition",RED);	
//       }
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
* Ãû    ³Æ£ºvoid tp_Config(void)
* ¹¦    ÄÜ£ºTFT ´¥ÃþÆÁ¿ØÖÆ³õÊ¼»¯
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎÞ 
****************************************************************************/
void tp_Config(void) 
{ 
//  GPIO_InitTypeDef  GPIO_InitStructure; 
//  SPI_InitTypeDef   SPI_InitStructure; 
//
//  /* SPI1 Ê±ÖÓÊ¹ÄÜ */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE); 
// 
//  /* SPI1 SCK(PA5)¡¢MISO(PA6)¡¢MOSI(PA7) ÉèÖÃ */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//¿ÚÏßËÙ¶È50MHZ
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	        //¸´ÓÃÄ£Ê½
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//  /* SPI1 ´¥ÃþÐ¾Æ¬µÄÆ¬Ñ¡¿ØÖÆÉèÖÃ PB7 */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//¿ÚÏßËÙ¶È50MHZ 
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//ÍÆÍìÊä³öÄ£Ê½
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//  
//  
//  /* ÓÉÓÚSPI1×ÜÏßÉÏ¹Ò½ÓÁË4¸öÍâÉè£¬ËùÒÔÔÚÊ¹ÓÃ´¥ÃþÆÁÊ±£¬ÐèÒª½ûÖ¹ÆäÓà3¸öSPI1 ÍâÉè£¬ ²ÅÄÜÕý³£¹¤×÷ */  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;           		//SPI1 SST25VF016BÆ¬Ñ¡ 
//  GPIO_Init(GPIOC, &GPIO_InitStructure);
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                //SPI1 VS1003Æ¬Ñ¡ 
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                 //SPI1 ÍøÂçÄ£¿éÆ¬Ñ¡
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//  GPIO_SetBits(GPIOC, GPIO_Pin_4);							//SPI CS1
//  GPIO_SetBits(GPIOB, GPIO_Pin_12);							//SPI CS4
//  GPIO_SetBits(GPIOA, GPIO_Pin_4);							//SPI NSS	   
//  
//   /* SPI1×ÜÏß ÅäÖÃ */ 
//  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //È«Ë«¹¤  
//  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						   //Ö÷Ä£Ê½
//  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					   //8Î»
//  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;						   //Ê±ÖÓ¼«ÐÔ ¿ÕÏÐ×´Ì¬Ê±£¬SCK±£³ÖµÍµçÆ½
//  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						   //Ê±ÖÓÏàÎ» Êý¾Ý²ÉÑù´ÓµÚÒ»¸öÊ±ÖÓ±ßÑØ¿ªÊ¼
//  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							   //Èí¼þ²úÉúNSS
//  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;  //²¨ÌØÂÊ¿ØÖÆ SYSCLK/64
//  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //Êý¾Ý¸ßÎ»ÔÚÇ°
//  SPI_InitStructure.SPI_CRCPolynomial = 7;							   //CRC¶àÏîÊ½¼Ä´æÆ÷³õÊ¼ÖµÎª7 
//  SPI_Init(SPI1, &SPI_InitStructure);
//  
//  /* SPI1 Ê¹ÄÜ */  
//  SPI_Cmd(SPI1,ENABLE);  
//}
//
///****************************************************************************
//* Ãû    ³Æ£ºunsigned char SPI_WriteByte(unsigned char data) 
//* ¹¦    ÄÜ£ºSPI1 Ð´º¯Êý
//* Èë¿Ú²ÎÊý£ºÎÞ
//* ³ö¿Ú²ÎÊý£ºÎÞ
//* Ëµ    Ã÷£º
//* µ÷ÓÃ·½·¨£º
//****************************************************************************/  
//unsigned char SPI_WriteByte(unsigned char data) 
//{ 
// unsigned char Data = 0; 
//
//  //µÈ´ý·¢ËÍ»º³åÇø¿Õ
//  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET); 
//  // ·¢ËÍÒ»¸ö×Ö½Ú  
//  SPI_I2S_SendData(SPI1,data); 
//
//   //µÈ´ýÊÇ·ñ½ÓÊÕµ½Ò»¸ö×Ö½Ú 
//  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET); 
//  // »ñµÃ¸Ã×Ö½Ú
//  Data = SPI_I2S_ReceiveData(SPI1); 
//
//  // ·µ»ØÊÕµ½µÄ×Ö½Ú 
//  return Data; 
}  


/****************************************************************************
* Ãû    ³Æ£ºu16 TPReadX(void) 
* ¹¦    ÄÜ£º´¥ÃþÆÁXÖáÊý¾Ý¶Á³ö
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£º
****************************************************************************/  
//u16 TPReadX(void)
//{ 
////   u16 x=0;
////   TP_CS();	                        //Ñ¡ÔñXPT2046 
////   Delay(20);						//ÑÓÊ±
////   SPI_WriteByte(0x90);				//ÉèÖÃXÖá¶ÁÈ¡±êÖ¾
////   Delay(20);						//ÑÓÊ±
////   x=SPI_WriteByte(0x00);			//Á¬Ðø¶ÁÈ¡16Î»µÄÊý¾Ý 
////   x<<=8;
////   x+=SPI_WriteByte(0x00);
////   Delay(20);						//½ûÖ¹XPT2046
////   TP_DCS(); 					    								  
////   x = x>>3;						//ÒÆÎ»»»Ëã³É12Î»µÄÓÐÐ§Êý¾Ý0-4095
////   return (x);
//}
/****************************************************************************
* Ãû    ³Æ£ºu16 TPReadY(void)
* ¹¦    ÄÜ£º´¥ÃþÆÁYÖáÊý¾Ý¶Á³ö
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£º
****************************************************************************/
//u16 TPReadY(void)
//{
////   u16 y=0;
////   TP_CS();	                        //Ñ¡ÔñXPT2046 
////   Delay(20);					//ÑÓÊ±
////   SPI_WriteByte(0xD0);				//ÉèÖÃYÖá¶ÁÈ¡±êÖ¾
////   Delay(20);					//ÑÓÊ±
////   y=SPI_WriteByte(0x00);			//Á¬Ðø¶ÁÈ¡16Î»µÄÊý¾Ý 
////   y<<=8;
////   y+=SPI_WriteByte(0x00);
////   Delay(20);					//½ûÖ¹XPT2046
////   TP_DCS(); 					    								  
////   y = y>>3;						//ÒÆÎ»»»Ëã³É12Î»µÄÓÐÐ§Êý¾Ý0-4095
////   return (y);
//}

/****************************************************************************
* Ãû    ³Æ£ºvoid drawbigpoint(u8 x,u16 y)
* ¹¦    ÄÜ£ºÔÚÆÁÄ»ÉÏ»­Ò»¸ö2X2µÄ´óµã
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£º
****************************************************************************/
void drawbigpoint(u8 x,u16 y)
{
//	if(x>220&&y<12)						       //Èç¹ûµã»÷µÄÊÇ×óÉÏ½ÇCLR×Ö·û£¬¾ÍÖ´ÐÐÇåÆÁÖ¸Áî
//	{
//		TFT_CLEAR(0,30,240,290);               //ÇåÆÁ 
//    	//TFT_ShowString(220,0,"CLR");         //ÏÔÊ¾ÇåÆÁÇøÓò
//	}
//	else if(y==0||x==0){					   //µã»÷µÚÒ»¸öÐ£ÕýµãËùÏÔÊ¾µÄÇéÐÎ
//		TFT_DrawPoint(x,y);//ÖÐÐÄµã 
//		TFT_DrawPoint(x+1,y);
//		TFT_DrawPoint(x,y+1);
//		TFT_DrawPoint(x+1,y+1);	
//	}		
//	else if(x<180&&y<30)					   //µã»÷µ÷É«°åÇøÓò£¬¸ù¾Ýµã»÷µÄ×ø±êÑ¡È¡ÏàÓ¦µÄÑÕÉ«			
//	{
//	    if(x<=30) POINT_COLOR=RED;
//		else if(x>60,x<=60) POINT_COLOR=BLUE;
//		else if(x>90,x<=90) POINT_COLOR=GREEN;
//		else if(x>120,x<=120) POINT_COLOR=GRED;
//		else if(x>150,x<=150) POINT_COLOR=BRED;	
//		else if(x>180,x<=180) POINT_COLOR=BLACK;
//	}
//	else if(y>30){								//µã»÷µÚ¶þ¸öÐ£ÕýµãËùÏÔÊ¾µÄÇéÐÎ
//		TFT_DrawPoint(x,y);
//		TFT_DrawPoint(x+1,y);
//		TFT_DrawPoint(x,y+1);
//		TFT_DrawPoint(x+1,y+1);	
//	}		  	
}
/****************************************************************************
* Ãû    ³Æ£ºvoid FillColor(u8 x,u16 y, u8 x1, u16 y1, u16 z)
* ¹¦    ÄÜ£ºÔÚÆÁÄ»ÉÏ»­µ÷É«°åÇøÓò
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£º
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
* Ãû    ³Æ£ºvoid Read_XPT2046(void)
* ¹¦    ÄÜ£º¶ÁÈ¡´¥Ãþ×ø±ê£¬½øÐÐ²ÉÑù¼ÆËã£¬²¢»­µã
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£º
****************************************************************************/ 
void Read_XPT2046(void)
{	
//	float X1,Y1;    
//	u8 t,t1,count=0;		
//	u16 databuffer[2][30];    				         //´¥Ãþ×ø±ê¹ý²ÉÑù»º³åÇø
//	u16 temp=0;	 
//    do{					                             //Ñ­»·¶ÁÊý30´Î		
//		X=TPReadX();		   
//		Y=TPReadY();
//		if(X>100&&X<4000&&Y>100&&Y<4000)             //Èç¹ûÊÇÔÚ´¥ÃþÏÔÊ¾ÓÐÐ§Çø·¶Î§µÄÖµ£¬±êÊ¾´Ë¶ÁÊýÓÐÐ§
//		{	  
//			databuffer[0][count]=X;
//			databuffer[1][count]=Y;
//			count++;  
//		} 		
//	}while((PEN==0)&&count<30); 			  		 
//	if(count==30)                                    //Ã¿´Î¶ÈÊýÒ»¶¨Òª¶Áµ½30´ÎÊý¾Ý,·ñÔò¶ªÆú
//	{  
//	    do                                            //½«Êý¾ÝXÉýÐòÅÅÁÐ
//		{	
//			t1=0;		  
//			for(t=0;t<count-1;t++)
//			{
//				if(databuffer[0][t]>databuffer[0][t+1])//ÉýÐòÅÅÁÐ
//				{
//					temp=databuffer[0][t+1];
//					databuffer[0][t+1]=databuffer[0][t];
//					databuffer[0][t]=temp;
//					t1=1; 
//				}  
//			}
//		}while(t1); 	  
//		do                                              //½«Êý¾ÝYÉýÐòÅÅÁÐ
//		{	
//			t1=0;		 
//			for(t=0;t<count-1;t++)
//			{
//				if(databuffer[1][t]>databuffer[1][t+1])//ÉýÐòÅÅÁÐ
//				{
//					temp=databuffer[1][t+1];
//					databuffer[1][t+1]=databuffer[1][t];
//					databuffer[1][t]=temp;
//					t1=1;	 
//				}  
//			}
//		}while(t1);
//	}			
//	/* ´ÓÅÅÐò¹ýµÄÊý×éÀïÖÐ¼ä³éÈ¡Á¬ÐøµÄ10×éÊý¾Ý£¬½øÐÐÈ¡Æ½¾ùÖµ,»ñµÃ½Ï¸ßµÄ¾«¶È */
//	X1=0; Y1=0;
//	for(count=10;count<20; count++){
//		X1=X1+databuffer[0][count];
//		Y1=Y1+databuffer[1][count];
//	}	
//	X1=X1/10;  Y1=Y1/10;
//	if(X1<=4096&&Y1<=4096)                       //´¥Ãþ×ø±êÖµÓ¦Ð¡ÓÚ4096
//	{	
//		X=X1;
//		Y=Y1;									 
//		if(dw==1){			 					 //Èç¹û½øÐÐÁËÐ£Õý¹ý³Ì
//			if(X1>=Xs)X1=0;						 //¶Ô¸ÃµãµÄ´¥ÃþXÖµ½øÐÐ½çÏÞÅÐ±ð£¬Èç¹û´óÓÚµÚÒ»¸öÐ£ÕýµãµÄ´¥ÃþXÖµ£¬¸ÃµãºÍÐ£ÕýµãµÄ²îÖµ³õÊ¼»¯Îª0£¨ºÍÆÁÓÐ¹Ø)
//			else X1=Xs-X1;						 //Èç¹ûÐ¡ÓÚµÚÒ»¸öÐ£ÕýµãµÄ´¥ÃþXÖµ£¬¼ÆËã³ö¸ÃµãºÍÐ£ÕýµãµÄ²îÖµ£¨ºÍÆÁÓÐ¹Ø)
//			if(Y1<=Ys)Y1=Ys-Y1;					 //Èç¹ûÐ¡ÓÚµÚÒ»¸öÐ£ÕýµãµÄ´¥ÃþYÖµ£¬¼ÆËã³ö¸ÃµãºÍÐ£ÕýµãµÄ²îÖµ£¨ºÍÆÁÓÐ¹Ø)
//			else Y1=0; 							 //¶Ô¸ÃµãµÄ´¥ÃþYÖµ½øÐÐ½çÏÞÅÐ±ð£¬Èç¹û´óÓÚµÚÒ»¸öÐ£ÕýµãµÄ´¥ÃþYÖµ£¬¸ÃµãºÍÐ£ÕýµãµÄ²îÖµ³õÊ¼»¯Îª0£¨ºÍÆÁÓÐ¹Ø)
//			X=X1/X2; 							 //¸ù¾ÝÒ»¸öÏñËØ¶ÔÓ¦µÄ´¥ÃþÖµ£¬Ëã³öÆÁÄ»ÉÏµÄÊµ¼ÊX×ø±ê
//			Y=Y1/Y2;							 //¸ù¾ÝÒ»¸öÏñËØ¶ÔÓ¦µÄ´¥ÃþÖµ£¬Ëã³öÆÁÄ»ÉÏµÄÊµ¼ÊY×ø±ê
//			drawbigpoint(X,Y); 					 //ÔÚ¸Ã×ø±ê»­µã
//		}
//	}
}    
/******************* (C) COPYRIGHT 2011 ·Ü¶·STM32 *****END OF FILE****/

