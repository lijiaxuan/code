#ifndef _USERDEVICE_H
#define _USERDEVICE_H

#include "stm32f10x.h"
#include "platform_config.h"
#include"all.h"

typedef enum { FAILED = 0, PASSED = !FAILED}TestStatus;

//PWM

extern TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
extern TIM_OCInitTypeDef  TIM_OCInitStructure;
extern uint16_t TimerPeriod ;
extern uint16_t Channel1Pulse ;

extern void pwmint(void);

//USART

extern USART_InitTypeDef USART_InitStructure;
extern void Usartint(void);
extern void Usend(uint16_t TxBuffer);

//ADC

#define ADC1_DR_Address    ((uint32_t)0x4001244C)

extern ADC_InitTypeDef ADC_InitStructure;
extern DMA_InitTypeDef DMA_InitStructure;
extern __IO uint32_t ADC_DualConvertedValueTab[2];

extern void adcint(void);

//flash
#define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#define BANK1_WRITE_START_ADDR  ((uint32_t)0x0800E000)
#define BANK1_WRITE_END_ADDR    ((uint32_t)0x0800F000)

extern uint32_t EraseCounter;
extern uint32_t Address;
extern uint32_t NbrOfPage;
extern volatile FLASH_Status FLASHStatus;
extern volatile TestStatus MemoryProgramStatus;

void proflash(uint32_t Ads,__IO uint32_t Data);
extern void intflash(void);

//display
extern int nn;
extern int mm;
extern u8 data00;
extern u8 data01;
extern u8 data10;
extern u8 data11;
extern void delay(unsigned long t);
extern void dispint(void);
extern void Display(uint32_t dis);
//switch
extern u8 tSW;
extern void SWint(void);
extern u8 SWx(void);
extern u8 S_flag;
//pianxuan
extern __IO uint32_t TimingDelay;
extern void pxint(void);
extern void Delay10us(__IO uint32_t nTime);
extern void px(int ynum);

#endif
