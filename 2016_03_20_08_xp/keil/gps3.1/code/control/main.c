
#include "systick.h"
#include "led.h"
#include "engineConfig.h"
#include "misc.h"
#include "gyro.h"
#include "encoder.h"
#include "gps.h"
#include "lcd.h"
#include "func.h"
#include "can.h"


void RCC_Configuration(void);


int main()
{
	//RCC_Configuration();//��λ��ʱ�ӿ��� ����
	SysTick_init();
// 	
// 	LED_Configuration();//��ʼ��LED��
// 	TIM_PWM_Init();
// 	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
// 	
// 	LED1_on;
// 	LED2_on;
// 	LED3_on;
// 	LED4_on; 
// 	
// 	UART1_DMA_init(38400);//��������������3ms
// 	UART1_DMA_EN();
// 	//Gyro_CheckFloat();
// 	LED4_off;
// 	
// 	QEI_Init();		//����
// 	
	TIM1_INIT();  //gps
	Can1_Init();
	UART3_init(115200);//	����
	
	LED1_off;
	LED2_off;
	LED3_off;
	
	ShowMenuList();
	
	while(1)
	{
// 		if (KeyReady==1)
// 		{
// 			KeyNumOperate(KeyValue);	
// 			KeyReady=0;
// 		}
// 		refresh();
		USART_SendData(USART3,0x11);
		delay(10);
	}
}


/*************************************************
����: void RCC_Configuration(void)
����: ��λ��ʱ�ӿ��� ����
����: ��
����: ��
**************************************************/
void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;                    //�����ⲿ���پ�������״̬ö�ٱ���
  RCC_DeInit();                                    //��λRCC�ⲿ�豸�Ĵ�����Ĭ��ֵ
  RCC_HSEConfig(RCC_HSE_ON);                       //���ⲿ���پ���
  HSEStartUpStatus = RCC_WaitForHSEStartUp();      //�ȴ��ⲿ����ʱ��׼����
  if(HSEStartUpStatus == SUCCESS)                  //�ⲿ����ʱ���Ѿ�׼���
  {
      	
    RCC_HCLKConfig(RCC_SYSCLK_Div1);               //����AHB(HCLK)ʱ�ӵ���==SYSCLK
    RCC_PCLK2Config(RCC_HCLK_Div1);                //����APB2(PCLK2)��==AHBʱ��
    RCC_PCLK1Config(RCC_HCLK_Div1);                //����APB1(PCLK1)��==AHB1/2ʱ��
         
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  //����PLLʱ�� == �ⲿ���پ���ʱ�� * 9 = 72MHz
    RCC_PLLCmd(ENABLE);                                   //ʹ��PLLʱ��
   
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)    //�ȴ�PLLʱ�Ӿ���
    {
    }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);            //����ϵͳʱ�� = PLLʱ��
    while(RCC_GetSYSCLKSource() != 0x08)                  //���PLLʱ���Ƿ���Ϊϵͳʱ��
    {
    }
  }
}

