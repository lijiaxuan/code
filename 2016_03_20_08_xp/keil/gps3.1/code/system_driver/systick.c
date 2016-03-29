#include "systick.h"

static __IO uint32_t TimingDelay;
void SysTick_init(void)
{
	if (SysTick_Config(SystemCoreClock/1000))   //Setup SysTick Timer for  1ms interrupts
	{
		/* Capture error */ 
		while (1);
	}

}

void delay(__IO uint32_t nTime)
{
	TimingDelay = nTime;
	
	while(TimingDelay != 0);
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

