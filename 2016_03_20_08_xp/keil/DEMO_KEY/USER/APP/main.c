#include "stm32f10x_conf.h"
#include "stm32f10x_adc.h"
#include "delay.h"
#include "gpio_bb.h"
#include "ch452.h"
#include "fsmc.h"

#include "ez_gui.h"


/**
  * @name   SystemInit
  * @brief  初始化系统时钟
  * @note   
  ×   此函数不需要在main函数中调用，在进main函数之前已经由启动文件调用了：
  ×   LDR     R0, =SystemInit
  ×   BLX     R0               
  ×   LDR     R0, =__main
  ×   BX      R0
  * @param  无
  * @retval 无
  */
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

void GPIO_Config(void)
{
  /*定义一个GPIO初始化结构体*/
  GPIO_InitTypeDef gpio_t;
  /*使能GPIOA时钟*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);
  /*配置GPIO初始化结构体*/
  gpio_t.GPIO_Mode=GPIO_Mode_Out_PP;  //推挽输出
  gpio_t.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6;         //
  gpio_t.GPIO_Speed=GPIO_Speed_50MHz; //IO最高速度
	 /*配置生效*/
  GPIO_Init(GPIOB,&gpio_t);
}

void get_data()
{
	
}

int main(void)
{
  int i=0;
	int pressure;
  EPEN * pen;
  EZ_GUI_Init();
  CH452_Init();
}
