#include "stm32f10x_conf.h"
#include "fsmc.h"


void FPGA_FSMC_Init(void)
{
  {//IO
    GPIO_InitTypeDef gpio_t ;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE,ENABLE);
    gpio_t.GPIO_Mode=GPIO_Mode_AF_PP;
    gpio_t.GPIO_Speed=GPIO_Speed_50MHz;
    //GPIOD
    ///PD4->NRD
    ///PD5->NWR
    ///PD7->NE1
    ///PD8->D13
    ///PD9->D14
    ///PD10->D15
    ///PD11->A16
    gpio_t.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8|\
      GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
    GPIO_Init(GPIOD,&gpio_t);
    //GPIOE
    ///PE11->D8
    ///PE12->D9
    ///PE13->D10
    ///PE14->D11
    ///PE15->D12
    gpio_t.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_Init(GPIOE,&gpio_t);
  }
  {//FSMC
    FSMC_NORSRAMInitTypeDef sram_t;
    FSMC_NORSRAMTimingInitTypeDef time_t;
    sram_t.FSMC_ReadWriteTimingStruct = &time_t;
    sram_t.FSMC_WriteTimingStruct = &time_t;
    
    FSMC_NORSRAMStructInit(&sram_t);
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);
    
    time_t.FSMC_AddressSetupTime = 0;
    time_t.FSMC_AddressHoldTime = 0;
    time_t.FSMC_DataSetupTime = 2;
    time_t.FSMC_BusTurnAroundDuration = 1;
    time_t.FSMC_CLKDivision = 0;
    time_t.FSMC_DataLatency = 0;
    time_t.FSMC_AccessMode = FSMC_AccessMode_B;
    
    sram_t.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    sram_t.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    sram_t.FSMC_MemoryType = FSMC_MemoryType_SRAM;
    sram_t.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    sram_t.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    sram_t.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    sram_t.FSMC_WrapMode = FSMC_WrapMode_Disable;
    sram_t.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    sram_t.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    sram_t.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    sram_t.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    sram_t.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    sram_t.FSMC_ReadWriteTimingStruct = &time_t;
    sram_t.FSMC_WriteTimingStruct = &time_t;
    
    FSMC_NORSRAMInit(&sram_t);
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1,ENABLE);
    
  }
}

void FPGA_Write(int addr,void * data,int size)
{
  uint16_t * pio=addr?(uint16_t*)((uint32_t)0x60000000):(uint16_t*)((uint32_t)0x60000000);
  uint8_t * pdata=(uint8_t *)data;
  while(size--)
  {
    *pio=((uint16_t)(*(pdata++)))<<8;
  }
}

