#include "stm32f10x.h"
#include "gpio.h"
#include "adc.h"
#include "rcc.h"
#include "usart.h"
#include "can.h"
float ha;
float haha,hahaha;

	float adc1 = 0;
	float adc2 = 0;
int main(void)
{
  ADC_Config();
  //DMA_Config();
	GPIO_Config();
	RCC_Config();

	//USART_Config(9600);
	//CAN_Config();
	//printf("zheshiyigeshiyan\n");
	
	//CAN_Transmit(CAN1,&TxMessage);
	int i = 0;
	int count = 0;
	float sum1 = 0;
	float sum2 = 0;
  while(1)
	 {
		 if(count == 100)
		 {
			 adc1 = sum1 / 100;
			 adc2 = sum2 / 100;
			 sum1 = 0;
			 sum2 = 0;
			 count = 0;
		 }
		 sum1 += ADC_ConvertedValue[0];
		 sum2 += ADC_ConvertedValue[1];
		 count ++;
		 for(i = 0; i < 100; i ++);
   }

}



