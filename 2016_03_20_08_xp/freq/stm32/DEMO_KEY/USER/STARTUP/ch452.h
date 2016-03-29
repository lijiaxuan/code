#ifndef _CH452_H_
#define _CH452_H_

/*IO defination for CH452*/
//PD12->DCLK
#define CH452_DCLK_PERIPH   RCC_APB2Periph_GPIOD
#define CH452_DCLK_PORT     GPIOD
#define CH452_DCLK_PIN      GPIO_Pin_12
//PD13->DIN
#define CH452_DIN_PERIPH    RCC_APB2Periph_GPIOD
#define CH452_DIN_PORT      GPIOD
#define CH452_DIN_PIN       GPIO_Pin_13
//PD14->LOAD
#define CH452_LOAD_PERIPH   RCC_APB2Periph_GPIOD
#define CH452_LOAD_PORT     GPIOD
#define CH452_LOAD_PIN      GPIO_Pin_14
//PD15->DOUT
#define CH452_DOUT_PERIPH   RCC_APB2Periph_GPIOD
#define CH452_DOUT_PORT     GPIOD
#define CH452_DOUT_PIN      GPIO_Pin_15

#define CH452_CMD_NOP       0x000 //4
#define CH452_CMD_RESET     0x201 //12
#define CH452_CMD_SLEEP     0x202 //12
#define CH452_CMD_SYSCFG    0x400 //12
  #define CH452_SYSCFG_GPOE 0x040
  #define CH452_SYSCFG_INTM 0x020
  #define CH452_SYSCFG_SSPD 0x010
  #define CH452_SYSCFG_DPLR 0x008
  #define CH452_SYSCFG_WDOG 0x004
  #define CH452_SYSCFG_KEYB 0x002
  #define CH452_SYSCFG_DISP 0x001
#define CH452_CMD_GETKEY    0x700 //4

#include "stm32f10x.h"

void CH452_Command(int cmd,int para,int cmdlen);
void CH452_Init(void);
uint8_t CH452_GetKey(void);


#endif

