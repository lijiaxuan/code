#include "all.h"

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



/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
// 	Encoder_Update();//��������ֻ����һ������
// 	GPS_Update();	//GPSֻ����һ������

// 	if(clk==1)
// 	{
// 		LED1_on;
// 	}
// 	else if(clk==250)
// 	{
// 		
// 		LED1_off;
// 		//��������Ǳ�ͷ����������������Ƿ��д�
// 		if(Gyro_Error_Flag==0)
// 		{
// 				DMA_check();
// 		}
// 	}
// 	else if(clk>=500)
// 	{
// 		clk=0;
// 	}
// 	
// 	clk++;
// 	real_time++;
	
}
