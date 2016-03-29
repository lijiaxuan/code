
#include "systick.h"
#include "led.h"
#include "engineConfig.h"
#include "misc.h"
#include "gyro.h"
#include "encoder.h"
#include "gps.h"
#include "lcd.h"
#include "func.h"
#include "can.h"


void RCC_Configuration(void);


int main()
{
	//RCC_Configuration();//复位和时钟控制 配置
	SysTick_init();
// 	
// 	LED_Configuration();//初始化LED灯
// 	TIM_PWM_Init();
// 	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
// 	
// 	LED1_on;
// 	LED2_on;
// 	LED3_on;
// 	LED4_on; 
// 	
// 	UART1_DMA_init(38400);//陀螺仪数据周期3ms
// 	UART1_DMA_EN();
// 	//Gyro_CheckFloat();
// 	LED4_off;
// 	
// 	QEI_Init();		//码盘
// 	
	TIM1_INIT();  //gps
	Can1_Init();
	UART3_init(115200);//	键盘
	
	LED1_off;
	LED2_off;
	LED3_off;
	
	ShowMenuList();
	
	while(1)
	{
// 		if (KeyReady==1)
// 		{
// 			KeyNumOperate(KeyValue);	
// 			KeyReady=0;
// 		}
// 		refresh();
		USART_SendData(USART3,0x11);
		delay(10);
	}
}


/*************************************************
函数: void RCC_Configuration(void)
功能: 复位和时钟控制 配置
参数: 无
返回: 无
**************************************************/
void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;                    //定义外部高速晶体启动状态枚举变量
  RCC_DeInit();                                    //复位RCC外部设备寄存器到默认值
  RCC_HSEConfig(RCC_HSE_ON);                       //打开外部高速晶振
  HSEStartUpStatus = RCC_WaitForHSEStartUp();      //等待外部高速时钟准备好
  if(HSEStartUpStatus == SUCCESS)                  //外部高速时钟已经准别好
  {
      	
    RCC_HCLKConfig(RCC_SYSCLK_Div1);               //配置AHB(HCLK)时钟等于==SYSCLK
    RCC_PCLK2Config(RCC_HCLK_Div1);                //配置APB2(PCLK2)钟==AHB时钟
    RCC_PCLK1Config(RCC_HCLK_Div1);                //配置APB1(PCLK1)钟==AHB1/2时钟
         
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  //配置PLL时钟 == 外部高速晶体时钟 * 9 = 72MHz
    RCC_PLLCmd(ENABLE);                                   //使能PLL时钟
   
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)    //等待PLL时钟就绪
    {
    }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);            //配置系统时钟 = PLL时钟
    while(RCC_GetSYSCLKSource() != 0x08)                  //检查PLL时钟是否作为系统时钟
    {
    }
  }
}

