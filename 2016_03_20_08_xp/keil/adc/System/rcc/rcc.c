#include "rcc.h"

void RCC_Config(void)
{
  ErrorStatus HSEStartUpStatus;
  RCC_DeInit();
  RCC_HSEConfig(RCC_HSE_ON);
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  if(HSEStartUpStatus==SUCCESS)
  {
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //开启FLASH预读缓冲功能，加速FLASH的读取。所有程序中必须的用法.位置：RCC初始化子函数里面，时钟起振之后
    FLASH_SetLatency(FLASH_Latency_2);                    //flash操作的延时
	
    RCC_HCLKConfig(RCC_SYSCLK_Div1);                      //配置AHB(HCLK)时钟=SYSCLK
	  RCC_PCLK2Config(RCC_HCLK_Div1);                       //配置APB2(PCLK2)钟=AHB时钟
    RCC_PCLK1Config(RCC_HCLK_Div2);                       //配置APB2(PCLK2)钟=AHB时钟 1/2
    
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);   //配置PLL时钟 == 外部高速晶体时钟*9
	  RCC_PLLCmd(ENABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)      //检查PLL时钟是否作为系统时钟
		{
    }
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);            //配置系统时钟 = PLL时钟
		while(RCC_GetSYSCLKSource()!=0x08)                    //检查PLL时钟是否作为系统时钟
		{
    }
  }
}


u32 TimeDelay;
 void delay_ms(u32 ticks)
 {
	SysTick_Config(72000);
	TimeDelay = ticks;
	while( TimeDelay != 0)
		;
	SysTick->CTRL = 0; //disable systick
 }
  void delay_us(u32 ticks)
 {
	SysTick_Config(72);
	TimeDelay = ticks;
	while( TimeDelay != 0)
		;
	SysTick->CTRL = 0; //disable systick
 }
 
 void SysTick_Handler(void)
{
	TimeDelay--;
}




















