#include "stm32f10x.h"
#include"all.h"

#define ADC1_DR_Address    ((uint32_t)0x4001244C)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ADC_InitTypeDef ADC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;
__IO uint32_t ADC_DualConvertedValueTab[2];//adc˫ת��ͨ��ֵ��

void adcint(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
//RCC CONFIG

  /* ADCCLK = PCLK2/4 */
  RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
  /* Enable peripheral clocks ------------------------------------------------*/
  /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

 /* Enable ADC1 and GPIOC clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_ADC2 | RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOB, ENABLE);


 /* Configure PC.04 (ADC Channel14/15) as analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);


//initial adc
 
  /* DMA1 channel1 configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;//����������Դ��ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_DualConvertedValueTab;//����copyĿ���ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//����copyĿ���ַ
  DMA_InitStructure.DMA_BufferSize = 2;//���������С
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//������Դ����
  DMA_InitStructure.DMA_MemoryInc =DMA_MemoryInc_Enable;//���ݱ���������ַ����
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//DMA�����ֳ�32λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ѭ��ģʽ
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//�����ȼ�
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//DMA�ر�MTM
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
  
  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;//ͬ��������ģʽ
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;//����ɨ��
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//����ת��
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//���ⲿ����
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//����Ҷ���
  ADC_InitStructure.ADC_NbrOfChannel = 2;//ת��ͨ����2
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel14 configuration */ 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_1Cycles5);//ͨ������
  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 2, ADC_SampleTime_1Cycles5);
  ADC_DMACmd(ADC1, ENABLE);

  /* ADC2 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;
  ADC_InitStructure.ADC_ScanConvMode =ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 2;
  ADC_Init(ADC2, &ADC_InitStructure);
  /* ADC2 regular channels configuration */ 
  ADC_RegularChannelConfig(ADC2, ADC_Channel_9, 1, ADC_SampleTime_1Cycles5);
  ADC_RegularChannelConfig(ADC2, ADC_Channel_8, 2, ADC_SampleTime_1Cycles5);
  ADC_ExternalTrigConvCmd(ADC2, ENABLE);
  
  
 /* Enable ADC1 */
  ADC_Cmd(ADC1,ENABLE); 	

  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));

  /* Enable ADC2 */

	ADC_Cmd(ADC2, ENABLE);
  /* Enable ADC2 reset calibaration register */   
  ADC_ResetCalibration(ADC2);
  /* Check the end of ADC2 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC2));

  /* Start ADC2 calibaration */
  ADC_StartCalibration(ADC2);
  /* Check the end of ADC2 calibration */
  while(ADC_GetCalibrationStatus(ADC2));

  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);

  /* Test on DMA1 channel1 transfer complete flag */
  while(!DMA_GetFlagStatus(DMA1_FLAG_TC1));
  /* Clear DMA1 channel1 transfer complete flag */
  DMA_ClearFlag(DMA1_FLAG_TC1);

}
