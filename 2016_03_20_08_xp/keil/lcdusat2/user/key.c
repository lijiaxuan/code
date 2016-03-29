

#include "stm32f10x_conf.h"
#include "key.h"



#define CH452_DCLK_SET()  (CH452_DCLK_PORT->BSRR=CH452_DCLK_PIN)
#define CH452_DCLK_CLR()  (CH452_DCLK_PORT->BRR=CH452_DCLK_PIN)

#define CH452_DIN_SET()  (CH452_DIN_PORT->BSRR=CH452_DIN_PIN)
#define CH452_DIN_CLR()  (CH452_DIN_PORT->BRR=CH452_DIN_PIN)

#define CH452_LOAD_SET()  (CH452_LOAD_PORT->BSRR=CH452_LOAD_PIN)
#define CH452_LOAD_CLR()  (CH452_LOAD_PORT->BRR=CH452_LOAD_PIN)

#define CH452_DOUT_GET()  (CH452_DOUT_PORT->IDR&CH452_DOUT_PIN)


void CH452_IO_Init(void)
{
  GPIO_InitTypeDef gpio_t;
  //peripheral clock enable
  RCC_APB2PeriphClockCmd(CH452_DCLK_PERIPH|CH452_DIN_PERIPH|CH452_LOAD_PERIPH|CH452_DOUT_PERIPH,ENABLE);
  //PD12->DCLK
  gpio_t.GPIO_Mode=GPIO_Mode_Out_PP;
  gpio_t.GPIO_Pin=CH452_DCLK_PIN;
  gpio_t.GPIO_Speed=GPIO_Speed_10MHz;
  GPIO_Init(CH452_DCLK_PORT,&gpio_t);
  //PD13->DIN
  gpio_t.GPIO_Pin=CH452_DIN_PIN;
  GPIO_Init(CH452_DIN_PORT,&gpio_t);
  //PD14->LOAD
  gpio_t.GPIO_Pin=CH452_LOAD_PIN;
  GPIO_Init(CH452_LOAD_PORT,&gpio_t);
  //PD15->DOUT
  gpio_t.GPIO_Mode=GPIO_Mode_IPU;
  gpio_t.GPIO_Pin=CH452_DOUT_PIN;
  GPIO_Init(CH452_DOUT_PORT,&gpio_t);
  //
}

void CH452_Command(int cmd,int para,int cmdlen)
{
  int mask;
  cmd|=para;
  cmd>>=(12-cmdlen);
  CH452_LOAD_CLR();
  for(mask=0x0001;mask<(1<<cmdlen);mask<<=1)
  {
    CH452_DCLK_CLR();
    if(cmd&mask)CH452_DIN_SET();
    else CH452_DIN_CLR();
    CH452_DCLK_SET();
  }
  CH452_LOAD_SET();
}

uint8_t CH452_GetKey(void)
{
  int key=0;
  if(CH452_DOUT_GET())return 0xFF;
  CH452_Command(CH452_CMD_GETKEY,0,4);
  //
  if(CH452_DOUT_GET())key|=0x40;
  CH452_DCLK_CLR();
  CH452_DCLK_SET();
  if(CH452_DOUT_GET())key|=0x20;
  CH452_DCLK_CLR();
  CH452_DCLK_SET();
  if(CH452_DOUT_GET())key|=0x10;
  CH452_DCLK_CLR();
  CH452_DCLK_SET();
  if(CH452_DOUT_GET())key|=0x08;
  CH452_DCLK_CLR();
  CH452_DCLK_SET();
  if(CH452_DOUT_GET())key|=0x04;
  CH452_DCLK_CLR();
  CH452_DCLK_SET();
  if(CH452_DOUT_GET())key|=0x02;
  CH452_DCLK_CLR();
  CH452_DCLK_SET();
  if(CH452_DOUT_GET())key|=0x01;
  CH452_DCLK_CLR();
  CH452_DCLK_SET();
  //
  if(key&0x40)return key&0xFF;
  else return 0xFF;
}

void CH452_Init(void)
{
  CH452_IO_Init();
  CH452_Command(CH452_CMD_NOP,0,12);
  CH452_Command(CH452_CMD_RESET,0,12);
  CH452_Command(CH452_CMD_SYSCFG,CH452_SYSCFG_KEYB|CH452_SYSCFG_WDOG,12);
}


void EXTI_5Init(void)
{
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource5);
		
		EXTI_InitStructure.EXTI_Line = EXTI_Line5;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // or Rising 		
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
	
	
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; 
		// EXTI0_IRQn is defined in stm32f10x.h
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
}

