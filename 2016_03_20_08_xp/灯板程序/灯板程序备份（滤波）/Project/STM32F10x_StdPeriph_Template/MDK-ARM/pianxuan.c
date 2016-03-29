#include "stm32f10x.h"
#include"all.h"
__IO uint32_t TimingDelay; 

void pxint(void)
		{
			GPIO_InitTypeDef GPIO_InitStructure;
			 //rcc config
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

			 //gpio cnfig
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
			GPIO_InitStructure.GPIO_Speed= GPIO_Speed_2MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_Init(GPIOC, &GPIO_InitStructure);
		 //systickint
			if (SysTick_Config(SystemCoreClock / 100000))//当0时结束循环
			{ 
				/* Capture error */ 
				while (1);
			}


		}

//10us延时

void Delay10us(__IO uint32_t nTime) 
		{
			TimingDelay = nTime;

			while(TimingDelay != 0);
		}

//片选

void px(int ynum)
		{

			switch(ynum)
			{

					case 0x0:
							{
								GPIO_ResetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8);
							}
					break;

					case 0x1:
							{
								GPIO_SetBits(GPIOC,GPIO_Pin_6);
								GPIO_ResetBits(GPIOC,GPIO_Pin_7|GPIO_Pin_8);
							}
					break;

					case 0x2:
							 {
								 GPIO_SetBits(GPIOC,GPIO_Pin_7);
								 GPIO_ResetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_8);
							 }
					break;

					case 0x3:
							{
								GPIO_SetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7);
								GPIO_ResetBits(GPIOC,GPIO_Pin_8);
							}
					break;

					case 0x4:
							{
								GPIO_SetBits(GPIOC,GPIO_Pin_8);
								GPIO_ResetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7);
							}
					break;

					case 0x5:
							{
								GPIO_SetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_8);
								GPIO_ResetBits(GPIOC,GPIO_Pin_7);
							}
					break;

					case 0x6:
							{
								GPIO_SetBits(GPIOC,GPIO_Pin_7|GPIO_Pin_8);
								GPIO_ResetBits(GPIOC,GPIO_Pin_6);
							}
					break;

					case 0x7:
							{
								 GPIO_SetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8);
							}
					break;

					default:break;

			}
		}

