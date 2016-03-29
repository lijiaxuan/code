/* 
* @Author: obsidian
* @Date:   2016-02-23 14:28:29
* @Last Modified by:   obsidian
* @Last Modified time: 2016-02-24 19:05:10
*/
#include "../inc/infrared.h"
#include "../inc/infrared_global.h"
#include "module/bluetooth/inc/bluetooth_global.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"

static  unsigned short ADC_ConvertedValue[2];

#define MAX  2800
#define MIN  1700
void GPIO_Config(void);
void ADC_Config(void);
void NVIC_Configuration(void);
void infrared_init()
{
	GPIO_Config();
	ADC_Config();
	NVIC_Configuration();	
}
void ADC_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
    /* DMA channel1 configuration */
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned int)&ADC1->DR;     //ADC??
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;//????
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 2;//????2???,???2
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//??????
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//???????
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;    //??
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;        //????
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
    DMA_Cmd(DMA1_Channel1, ENABLE);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;    //??ADC??
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;//????,???,DISABLE ;      //??????,???????????
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;    //????????,??????ADC??
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;    //?????????
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;     //???????
    ADC_InitStructure.ADC_NbrOfChannel = 2;         //2????????2
    ADC_Init(ADC1, &ADC_InitStructure);

    RCC_ADCCLKConfig(RCC_PCLK2_Div8); 

    ADC_RegularChannelConfig(ADC1, ADC_Channel_10 , 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11 , 2, ADC_SampleTime_55Cycles5);

    ADC_DMACmd(ADC1, ENABLE);

    ADC_Cmd(ADC1, ENABLE);
 
    ADC_ResetCalibration(ADC1);

    while(ADC_GetResetCalibrationStatus(ADC1));

    ADC_StartCalibration(ADC1);

    while(ADC_GetCalibrationStatus(ADC1));

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO,ENABLE);

    GPIO_PinRemapConfig(GPIO_FullRemap_USART3,ENABLE);
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
    GPIO_Init(GPIOC,&GPIO_InitStructure);

}
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
     /* Enable DMA channel1 IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void DMA1_Channel1_IRQHandler(void)
{
    static char count = 0;
    static float sum1 = 0;
    static float sum2 = 0;
    static float sensor_r = 0;
    static float sensor_l = 0;
    if(DMA_GetITStatus(DMA1_IT_TC1))
    {
        DMA_ClearITPendingBit(DMA1_IT_TC1); 
        count ++;
        sum1 += ADC_ConvertedValue[0];
        sum2 += ADC_ConvertedValue[1];
        if(count == 100)
        {
            sensor_r = sum1 / 100;
            sensor_l = sum2 / 100;
            if(sensor_r < MIN)
            {
                infrared.flag = -1 + 10;
            }
            else if(sensor_l < MIN)
            {
                infrared.flag = 1 + 10;
            }
            else
            {
                infrared.flag = 0 + 10;
            }
            bluetooth.send((unsigned char *)&infrared.flag,1);
            count = 0;
            sum1 = 0;
            sum2 = 0;
        }
    }
}