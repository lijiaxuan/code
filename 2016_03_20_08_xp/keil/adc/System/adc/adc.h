#ifndef __ADC_H
#define __ADC_H
#include "stm32f10x.h"

#define ADC1_DR_Address ((u32)0x40012400+0x4c)

void ADC_Config(void);
void DMA_Config(void);

extern __IO u16 ADC_ConvertedValue[2];

u16 AD_Average(int average_count,char id);
#endif
