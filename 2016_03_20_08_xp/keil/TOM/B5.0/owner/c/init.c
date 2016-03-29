#include "all.h"
#include "can.h"

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
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //����FLASHԤ�����幦�ܣ�����FLASH�Ķ�ȡ�����г����б�����÷�.λ�ã�RCC��ʼ���Ӻ������棬ʱ������֮��
    FLASH_SetLatency(FLASH_Latency_2);                    //flash��������ʱ
      	
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

/*************************************************
����: void GPIO_Configuration(void��
����: GPIO����
����: ��
����: ��
**************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;        //����GPIO��ʼ���ṹ��

	//  ��GPIO A B C D��ʱ�� 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	/* Configure PB.3,PB.4 as Output push-pull */ 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	/* Configure PC.13,PC.14 as Output push-pull */ 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 


}

void led_tag(uint8_t i)
{
	switch(i)
	{
		case 1:
			if((GPIOB->ODR & GPIO_Pin_3) == 0)
				GPIO_SetBits(GPIOB, GPIO_Pin_3);
			else
				GPIO_ResetBits(GPIOB, GPIO_Pin_3); 
			break;
		case 2:
			if((GPIOB->ODR & GPIO_Pin_4) == 0)
				GPIO_SetBits(GPIOB, GPIO_Pin_4);
			else
				GPIO_ResetBits(GPIOB, GPIO_Pin_4); 
			break;
		case 3:
			if((GPIOC->ODR & GPIO_Pin_13) == 0)
				GPIO_SetBits(GPIOC, GPIO_Pin_13);
			else
				GPIO_ResetBits(GPIOC, GPIO_Pin_13); 
			break;
		case 4:
			if((GPIOC->ODR & GPIO_Pin_14) == 0)
				GPIO_SetBits(GPIOC, GPIO_Pin_14);
			else
				GPIO_ResetBits(GPIOC, GPIO_Pin_14); 
			break;
	}
}

void LedInit(void)
{
    GPIO_Configuration();
    LED1_on;
	LED2_on;
	LED3_on;
	LED4_on;
}

void PwmInit(void)
{
    pwm3_4_Freqz=50;
	pwm1_2_Freqz=50;
	TIM_PWM_Init();
	SET_ANGLE(0,1);//-45 580mm 45
    SET_ANGLE(0,2);//-45 580mm 45
}

void SdInit(void)
{
    
    SD_Initialize();
    f_mount_on();
    set_dir();  //ÿһ���¿����ᴴ��һ���µ�Ŀ¼    
}

void InitAll(void)
{
    
	RCC_Configuration();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	LedInit();
    SysTick_init();
    
    TIM1_INIT();
    brake_timer_cfg();//ɲ���ö�ʱ����ʼ��

    SdInit();

    //Data_Load();	//���̡������Ǳ궨����
	//Data_Save();
    USART3_Init();
	Can1_Init();	//canͨ��
    
    PwmInit();      //���
    
    #if GYRO
	UART1_DMA_init(38400);//��������������3ms
    UART1_DMA_EN();
    Gyro_Error_Flag = 0;
   delay(1000);//�ȴ�һ��ʱ��У��Ư����ֹ���嶶��
    //Gyro_CheckFloat();
	#endif
    
    QEI_Init();		//����
  //colour();       //��ɫ������
    

	GPS_Clear(); 
    
    LED1_off;   //��ʼ������
	LED2_off;
	LED3_off;
	LED4_off;
    
}



