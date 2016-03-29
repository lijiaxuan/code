#include "stm32f10x_conf.h"
#include "stm32f10x_tim.h"
#include "delay.h"
#include "gpio_bb.h"
#include "ch452.h"
#include "../inc/func.h"
#include "EZ_GUI.H"
#include "stm32f10x_dac.h"
float		duty;

unsigned int    Ts;
unsigned int    num_per_T;
unsigned int    num_per_duty;
unsigned int 		num_duty;
float						delta_time;

unsigned char   freq_status;

unsigned char   key_v;
unsigned char   status;

unsigned char unit_index;
EPEN *pen = 0;

float           freq;
float           T;

//0 DAC 2V
//1 DAC 0.8
//2 DAC 0.8

void SystemInit(void)
{
  ErrorStatus	err;
  /* Reset RCC */
  RCC_DeInit();
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  /* Wait till HSE is ready */
  err = RCC_WaitForHSEStartUp();
  if (err == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 
    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);
    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);
    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08);
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock configuration.
       User can add here some code to deal with this error */    

    /* Go to infinite loop */
    //while (1)
    //{
    //}
  }
}

void DAC_VOLTAGE_Configuration(void)

{

  GPIO_InitTypeDef gpio_t;

      DAC_InitTypeDef            DAC_InitStructure;
  gpio_t.GPIO_Mode = GPIO_Mode_AF_PP; 
  gpio_t.GPIO_Pin  = GPIO_Pin_4;
  gpio_t.GPIO_Speed=GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA,&gpio_t);
	
	  gpio_t.GPIO_Mode = GPIO_Mode_Out_PP; 
  gpio_t.GPIO_Pin  = GPIO_Pin_7;
  gpio_t.GPIO_Speed=GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA,&gpio_t);
      DAC_DeInit();

     

     DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;

     DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;      

     DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;

     DAC_Init(DAC_Channel_1, &DAC_InitStructure);

      DAC_Cmd(DAC_Channel_1, ENABLE);

     DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);

}

void GPIO_Config(void)
{

  GPIO_InitTypeDef gpio_t;

  gpio_t.GPIO_Mode=GPIO_Mode_Out_PP; 
  gpio_t.GPIO_Pin=GPIO_Pin_11 | GPIO_Pin_12|GPIO_Pin_13 |GPIO_Pin_14 ;         //
  gpio_t.GPIO_Speed=GPIO_Speed_50MHz; 

  GPIO_Init(GPIOE,&gpio_t);
  gpio_t.GPIO_Mode=GPIO_Mode_Out_PP;
  gpio_t.GPIO_Pin=GPIO_Pin_8 ;       
  gpio_t.GPIO_Speed=GPIO_Speed_50MHz;
	 
  GPIO_Init(GPIOD,&gpio_t);
  gpio_t.GPIO_Mode= GPIO_Mode_IN_FLOATING;
  gpio_t.GPIO_Pin= GPIO_Pin_15 ;       
  gpio_t.GPIO_Speed=GPIO_Speed_50MHz; 
	 
  GPIO_Init(GPIOE,&gpio_t);
}

void delay(unsigned short tim)
{
  unsigned short i = 0;
  for(i = 0; i < tim; i ++);
}

void timer_init()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef         NVIC_InitStruct;

	TIM_DeInit(TIM3);		
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 3600 - 1; 
	TIM_TimeBaseStructure.TIM_Period = 20040; 			
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
  NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
  TIM_Cmd(TIM3, ENABLE);

  TIM_DeInit(TIM2);   
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 1800 - 1; 
  TIM_TimeBaseStructure.TIM_Period = 2000;       
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
  NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
  TIM_Cmd(TIM2, ENABLE);
}
void get_data()
{
    unsigned char   i     = 0;
    unsigned char   data  = 0;
    Ts = 0;
		num_duty = 0;
		num_per_duty = 0;
		num_per_T = 0;
    GPIOE->ODR &= (~CS);       //cs = 0
    delay(100);
    for (i = 0; i < 32; i ++)
    {
      GPIOE->ODR |= SPICLK; //clk = 1
      GPIOB->ODR |= (1<<8);
      delay(5);
      data = (GPIOE->IDR & (DATA)) >> 15;
      Ts = Ts | (data << i);
      delay(50);
      GPIOE->ODR &= (~SPICLK); //clk = 0
      delay(50);
    }
    for (i = 0; i < 32; i ++)
    {
      GPIOE->ODR |= SPICLK; //clk = 1
      delay(5);
      data = (GPIOE->IDR & (DATA)) >> 15;
      num_per_T = num_per_T | (data << i);
      delay(50);
      GPIOE->ODR &= (~SPICLK); //clk = 0
      delay(50);
    }
    for (i = 0; i < 32; i ++)
    {
      GPIOE->ODR |= SPICLK; //clk = 1
      delay(5);
      data = (GPIOE->IDR & (DATA)) >> 15;
      num_per_duty = num_per_duty | (data << i);
      delay(50);
      GPIOE->ODR &= (~SPICLK); //clk = 0
      delay(50);
    }
    for (i = 0; i < 32; i ++)
    {
      GPIOE->ODR |= SPICLK; //clk = 1
      delay(5);
      data = (GPIOE->IDR & (DATA)) >> 15;
      num_duty = num_duty | (data << i);
      delay(50);
      GPIOE->ODR &= (~SPICLK); //clk = 0
      delay(50);
    }
		GPIOE->ODR &= (~ RST);
		delay(50);
		GPIOE->ODR |= RST;
		GPIOE->ODR |= CS;
		delay(100);
		delay(100);
}

void TIM3_IRQHandler()
{
  TIM_ClearFlag(TIM3, TIM_IT_Update);
	get_data();
  unit_index = 0;
  if(Ts > 10000)
  {
    GPIOA->ODR |= BPF;
    freq = Ts;
    duty = 100.0 - num_duty * 100.0 / 50000000.0 + Ts / (20090000.0);
    //low
  }
  else
  {
    GPIOA->ODR &= ~BPF;
    freq = 50000000.0 / num_per_T;
    duty = 100.0 - num_per_duty * 100.0 / (float)num_per_T;
    //high
  }
  if((freq_status == 0 ) && Ts > 3) // > 2
  {
      freq_status = 1;
      TIM3->ARR = 20040;
  }
  else if((freq_status == 1) && Ts <= 3)
  {
    freq_status = 0;
    TIM3->ARR = 42080; 
  }
	
	delta_time = 10000000 / freq * duty;
	
  // if(freq_status == 0)
  // {
  //   freq = freq * 2;
  // }


  switch(ui.state)
  {
    case NO_ACTION:
      
      break;
    case SELECT:
      
      break;
    case GET:
      switch(ui.selected)
      {
        case 0:
          for(;freq > 1000;)
          {
            freq = freq / 1000.0;
            unit_index ++;
						if(unit_index > 3)
						{
							break;
						}
          }
          pen->fcolor = GET_COLOR;
          num2str(freq);
          pen->PutString(ui.widget[0].data_point[0],ui.widget[0].data_point[1],(unsigned char *)blank_data);
          pen->PutString(ui.widget[0].data_point[0],ui.widget[0].data_point[1],(unsigned char *)data);
					pen->PutString(ui.widget[0].unit_point[0],ui.widget[0].unit_point[1],(unsigned char *)blank_data);
          pen->PutString(ui.widget[0].unit_point[0],ui.widget[0].unit_point[1],ui.widget[0].unit[unit_index]);
          break;
        case 1:
          for(;delta_time > 1000;)
          {
            delta_time = delta_time / 1000.0;
            unit_index ++;
          }
          pen->fcolor = GET_COLOR;
          num2str(delta_time);
          pen->PutString(ui.widget[1].data_point[0],ui.widget[1].data_point[1],(unsigned char *)blank_data);
          pen->PutString(ui.widget[1].data_point[0],ui.widget[1].data_point[1],(unsigned char *)data);
					pen->PutString(ui.widget[1].unit_point[0],ui.widget[1].unit_point[1],(unsigned char *)blank_data);
          pen->PutString(ui.widget[1].unit_point[0],ui.widget[1].unit_point[1],ui.widget[1].unit[unit_index]);					
          break;
        case 2:
          num2str(duty);
          pen->PutString(ui.widget[2].data_point[0],ui.widget[2].data_point[1],(unsigned char *)blank_data);
          pen->PutString(ui.widget[2].data_point[0],ui.widget[2].data_point[1],(unsigned char *)data);
          pen->PutString(ui.widget[2].unit_point[0],ui.widget[2].unit_point[1],ui.widget[2].unit[0]);

          break;
      }
      break;
  }

}

void TIM2_IRQHandler()
{
  TIM_ClearFlag(TIM2, TIM_IT_Update);
  key_process();
}


int main(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOD, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	GPIO_Config();
	pen=EZ_DRAW_GetPen();
  EZ_GUI_Init();
  CH452_Init();
	timer_init();
  DAC_VOLTAGE_Configuration();
	GPIOE->ODR &= (~ RST);
	delay(50);
	GPIOE->ODR |= RST;
	//GPIOD->ODR |= (MOD);
	GPIOE->ODR &= (~MOD);
  fresh_screen();
	while(1)
	{}
}
