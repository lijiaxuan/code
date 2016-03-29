#include "stm32f10x.h"
#include"all.h"
u8 tSW=0;
u8 S_flag=0;
void SWint(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
   //rcc config
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

   //gpio cnfig
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed= GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

}

u8 SWx(void)
{
	  tSW=0x02;

((GPIO_ReadInputData(GPIOC)&0x1c00)>>10);//读取制定管脚的输入
	return tSW;
}

