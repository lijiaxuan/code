#include "all.h"
#include "can.h"

/*************************************************
函数: void RCC_Configuration(void)
功能: 复位和时钟控制 配置
参数: 无
返回: 无
**************************************************/
void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;                    //定义外部高速晶体启动状态枚举变量
  RCC_DeInit();                                    //复位RCC外部设备寄存器到默认值
  RCC_HSEConfig(RCC_HSE_ON);                       //打开外部高速晶振
  HSEStartUpStatus = RCC_WaitForHSEStartUp();      //等待外部高速时钟准备好
  if(HSEStartUpStatus == SUCCESS)                  //外部高速时钟已经准别好
  {
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //开启FLASH预读缓冲功能，加速FLASH的读取。所有程序中必须的用法.位置：RCC初始化子函数里面，时钟起振之后
    FLASH_SetLatency(FLASH_Latency_2);                    //flash操作的延时
      	
    RCC_HCLKConfig(RCC_SYSCLK_Div1);               //配置AHB(HCLK)时钟等于==SYSCLK
    RCC_PCLK2Config(RCC_HCLK_Div1);                //配置APB2(PCLK2)钟==AHB时钟
    RCC_PCLK1Config(RCC_HCLK_Div1);                //配置APB1(PCLK1)钟==AHB1/2时钟
         
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  //配置PLL时钟 == 外部高速晶体时钟 * 9 = 72MHz
    RCC_PLLCmd(ENABLE);                                   //使能PLL时钟
   
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)    //等待PLL时钟就绪
    {
    }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);            //配置系统时钟 = PLL时钟
    while(RCC_GetSYSCLKSource() != 0x08)                  //检查PLL时钟是否作为系统时钟
    {
    }
  }

}

/*************************************************
函数: void GPIO_Configuration(void）
功能: GPIO配置
参数: 无
返回: 无
**************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;        //定义GPIO初始化结构体

	//  打开GPIO A B C D的时钟 
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
    set_dir();  //每一次新开机会创建一个新的目录    
}

void InitAll(void)
{
    
	RCC_Configuration();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	LedInit();
    SysTick_init();
    
    TIM1_INIT();
    brake_timer_cfg();//刹车用定时器初始化

    SdInit();

    //Data_Load();	//码盘、陀螺仪标定参数
	//Data_Save();
    USART3_Init();
	Can1_Init();	//can通信
    
    PwmInit();      //舵机
    
    #if GYRO
	UART1_DMA_init(38400);//陀螺仪数据周期3ms
    UART1_DMA_EN();
    Gyro_Error_Flag = 0;
   delay(1000);//等待一段时间校零漂，防止车体抖动
    //Gyro_CheckFloat();
	#endif
    
    QEI_Init();		//码盘
  //colour();       //颜色传感器
    

	GPS_Clear(); 
    
    LED1_off;   //初始化结束
	LED2_off;
	LED3_off;
	LED4_off;
    
}



