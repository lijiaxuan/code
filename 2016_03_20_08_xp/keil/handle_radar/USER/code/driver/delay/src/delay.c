#include "../inc/delay.h"
__IO uint32_t TimingDelay;
void SYSTICK_Init(void)
{
	if(SysTick_Config(SystemCoreClock /500))
	{ 
		/* Capture error */ 
		while (1);
	}
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_SetPriority(SysTick_IRQn,0);
}
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}
void SysTick_Handler(void)
{

	TimingDelay_Decrement();
}

void delay_us(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;
	
  while(TimingDelay!=0);
}
void delay_ms(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;
	
  while(TimingDelay!=0);
}	
void DelayS(uint32_t t)
{
	while(t)
	{t--;}
}


