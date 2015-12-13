#ifndef __DELAY_H
#define __DELAY_H 			
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"   
void SYSTICK_Init(void);
void delay_us(unsigned int nTime);
void delay_ms(unsigned int nTime);
void TimingDelay_Decrement(void);
void DelayS(unsigned int t);
#endif



























