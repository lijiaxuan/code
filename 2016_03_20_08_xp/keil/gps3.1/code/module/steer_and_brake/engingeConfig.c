#include "engineConfig.h"


//TIM4 CH4    TIM4 CH3
//  PWM1      PWM2

int mid_value_pwm1=1430;
int mid_value_pwm2=2112;

float engine_angle1=0;
float engine_angle2=0;

unsigned char pwm1_2_Freqz=50;

void  PWM_GPIO_Config(void)
{	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//开启TIM4时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;// 配置 PB8,PB9为复用推挽模式
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

}

void  PWM_Mode_Config(void)
{	
	//TIM4 ModeConfig
	int TIM4_PERIOD=SystemCoreClock/pwm1_2_Freqz/72;	//72 is prescaler	// 定时器初始化;
	{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
        TIM_OCInitTypeDef TIM_OCInitStruct;

	
		TIM_TimeBaseStructure.TIM_Period=TIM4_PERIOD;
		TIM_TimeBaseStructure.TIM_Prescaler=72-1;
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM4, & TIM_TimeBaseStructure);
		
		TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;  //设为PWM输出模式
		TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;//
		TIM_OCInitStruct.TIM_Pulse=mid_value_pwm2;//
		TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;//
		TIM_OC3Init(TIM4,&TIM_OCInitStruct);
		TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
		
		TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;//
		TIM_OCInitStruct.TIM_Pulse=mid_value_pwm1;
		TIM_OC4Init(TIM4,&TIM_OCInitStruct);	//初始化TIM4的channel 4(pwm1)
		TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);
		
		TIM_ARRPreloadConfig(TIM4, ENABLE);//开启重载寄存器ARR		
		TIM_Cmd(TIM4,ENABLE);	
		
	}
}



void TIM_PWM_Init(void)
{
	PWM_GPIO_Config();
	PWM_Mode_Config();
}



void SET_ANGLE(double angle,int pwm_port)//控制舵机偏转角度, 右偏为正
{
	u16 pwm_count;
	if(pwm_port==1)
	{
		
		pwm_count=mid_value_pwm1+(int)(angle*ANGLE_CONVERT_pwm1);	//
		if(pwm_count>mid_value_pwm1+153)
			pwm_count=mid_value_pwm1+153;
		else if(pwm_count<mid_value_pwm1-189)
			pwm_count=mid_value_pwm1-189;
		TIM4 ->CCR4=pwm_count;
		engine_angle1=angle;
    }
	
	if(pwm_port==2)
	{
		pwm_count=mid_value_pwm2+(int)(angle*ANGLE_CONVERT_pwm2);
		
// 		if(pwm_count>mid_value_pwm2+271)
// 			pwm_count=mid_value_pwm2+271;
// 		else if(pwm_count<mid_value_pwm2-271)
// 			pwm_count=mid_value_pwm2-271;
		
		TIM4 ->CCR3=pwm_count;
		engine_angle2=angle;
    }
 
}
