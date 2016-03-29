#include "rcc.h"

void RCC_Config(void)
{
  ErrorStatus HSEStartUpStatus;
  RCC_DeInit();
  RCC_HSEConfig(RCC_HSE_ON);
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  if(HSEStartUpStatus==SUCCESS)
  {
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //����FLASHԤ�����幦�ܣ�����FLASH�Ķ�ȡ�����г����б�����÷�.λ�ã�RCC��ʼ���Ӻ������棬ʱ������֮��
    FLASH_SetLatency(FLASH_Latency_2);                    //flash��������ʱ
	
    RCC_HCLKConfig(RCC_SYSCLK_Div1);                      //����AHB(HCLK)ʱ��=SYSCLK
	  RCC_PCLK2Config(RCC_HCLK_Div1);                       //����APB2(PCLK2)��=AHBʱ��
    RCC_PCLK1Config(RCC_HCLK_Div2);                       //����APB2(PCLK2)��=AHBʱ�� 1/2
    
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);   //����PLLʱ�� == �ⲿ���پ���ʱ��*9
	  RCC_PLLCmd(ENABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)      //���PLLʱ���Ƿ���Ϊϵͳʱ��
		{
    }
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);            //����ϵͳʱ�� = PLLʱ��
		while(RCC_GetSYSCLKSource()!=0x08)                    //���PLLʱ���Ƿ���Ϊϵͳʱ��
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




















