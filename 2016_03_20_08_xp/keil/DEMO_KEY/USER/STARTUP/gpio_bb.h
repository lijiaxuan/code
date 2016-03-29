#include "stm32f10x.h"

typedef struct 
{
  uint32_t bit0;
  uint32_t bit1;
  uint32_t bit2;
  uint32_t bit3;
  uint32_t bit4;
  uint32_t bit5;
  uint32_t bit6;
  uint32_t bit7;
  uint32_t bit8;
  uint32_t bit9;
  uint32_t bit10;
  uint32_t bit11;
  uint32_t bit12;
  uint32_t bit13;
  uint32_t bit14;
  uint32_t bit15;
}GPIO_BITS;

#define ADDR_BB_PERIPH(x) (PERIPH_BB_BASE+((((uint32_t)x)&0x1FFFFFFF)<<5))


#define PAx ((GPIO_BITS *)ADDR_BB_PERIPH(&GPIOA->ODR))
#define PBx ((GPIO_BITS *)ADDR_BB_PERIPH(&GPIOB->ODR))
#define PCx ((GPIO_BITS *)ADDR_BB_PERIPH(&GPIOC->ODR))
#define PDx ((GPIO_BITS *)ADDR_BB_PERIPH(&GPIOD->ODR))
#define PEx ((GPIO_BITS *)ADDR_BB_PERIPH(&GPIOE->ODR))
#define PFx ((GPIO_BITS *)ADDR_BB_PERIPH(&GPIOF->ODR))
#define PGx ((GPIO_BITS *)ADDR_BB_PERIPH(&GPIOG->ODR))

///*
#define PUT_PCL(x)  (GPIOC->BSRR=(x)|(((~x)&0xFF)<<16))
#define PUT_PCH(x)  (GPIOC->BSRR=((x)<<8)|(((~x)&0xFF)<<24))
#define PUT_PBL(x)  (GPIOB->BSRR=(x)|(((~x)&0xFF)<<16))
#define PUT_PBH(x)  (GPIOB->BSRR=((x)<<8)|(((~x)&0xFF)<<24))
//#define PUT_PEL(x)  (GPIOE->BSRR=(x)|(((~x)&0xff)<<16))

#define PUT_PEL(x)  (GPIOE->ODR=(GPIOE->ODR&0xFF00)|x)
