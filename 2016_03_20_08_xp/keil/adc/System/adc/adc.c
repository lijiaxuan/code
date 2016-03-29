#include "adc.h"
__IO u16 ADC_ConvertedValue[2];
void ADC_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
    /* DMA channel1 configuration */
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;     //ADC??
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
    
    /* Enable DMA channel1 */
    DMA_Cmd(DMA1_Channel1, ENABLE);
    
    /* ADC1 configuration */
    
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;    //??ADC??
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;//????,???,DISABLE ;      //??????,???????????
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;    //????????,??????ADC??
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;    //?????????
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;     //???????
    ADC_InitStructure.ADC_NbrOfChannel = 2;         //2????????2
    ADC_Init(ADC1, &ADC_InitStructure);
    
    /*??ADC??,?PCLK2?8??,?9Hz*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
    /*??ADC1???2?55.    5?????,???1 */ 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10 , 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11 , 2, ADC_SampleTime_55Cycles5);
    /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);
    
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);
    
    /*??????? */   
    ADC_ResetCalibration(ADC1);
    /*??????????? */
    while(ADC_GetResetCalibrationStatus(ADC1));
    
    /* ADC?? */
    ADC_StartCalibration(ADC1);
    /* ??????*/
    while(ADC_GetCalibrationStatus(ADC1));
    
    /* ??????????,????????ADC?? */ 
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
  DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr=ADC1_DR_Address;//定义 DMA通道外设基地址=ADC1_DR_Address 0x4001244C  
  //0x4001244C 寄存器边界地址0X40012400+ADC规则数据寄存器偏移地址4CH 
  DMA_InitStructure.DMA_MemoryBaseAddr=(u32)ADC_ConvertedValue;
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;//外设作为数据传输的来源 DST 外设为目的地 
	DMA_InitStructure.DMA_BufferSize=2;//用以定义指定DMA通道的DMA缓存的大小，单位为数据单位。
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;//当前外设寄存器地址不变
  DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;//DMA通道操作模式位环形缓冲模式
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;//DMA通道操作模式位环形缓冲模式
	
  DMA_Init(DMA1_Channel1,&DMA_InitStructure);//初始化DMA通道1
	
  DMA_Cmd(DMA1_Channel1,ENABLE);//使能DMA通道1
}

//u16 AD_Average(int average_count,int channel)
//{
//	int i=0;
//	u16 temp;
//	float sum=0;
//	for(i=0;i<average_count;i++)
//	{
//		temp=ADC_ConvertedValue[channel];
//		sum+=temp;
//  }
//	sum=sum/(float)average_count;
//	return (u16)sum;
//}

u16 AD_Average(int average_count,char id)
{
	int i=0;
	float sum=0;
	for(i=0;i<average_count;i++)
	{
		sum+=ADC_ConvertedValue[id];
  }
	sum=sum/(float)average_count;
	return (u16)sum;
}
















