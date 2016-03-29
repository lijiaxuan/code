/* 
* @Author: obsidian
* @Date:   2015-12-13 22:04:31
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-24 16:42:20
*/
#include "../inc/GPS.h"
#include "../inc/GPS_global.h"
#include "driver/led/inc/led.h"
#include "driver/delay/inc/delay.h"
#include "driver/math/inc/math.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "math.h"

#define USART1_DR_Address ((u32)0x40013804)   

//volatile static uint8_t USART1_RxBuffer[2048];
volatile uint8_t USART1_RxBuffer[8];


static struct Encoder_property encoder_property[ENCODER_NUM] = 
{
	//count_per_rotation  R 		carlen 		TIMx     direction   			convert
	{	2048,					0,		263.0,				TIM3,		1,			3000/43444.0},
	{   2048,					0,		263.0,				TIM8,		-1,			3000/43030.0}

};


static struct Gyro gyro = 
{
	1.60893,		//gyro_convert1
	1.610855,		//gyro_convert2
	{
		0,		//value
		0		//count
	},
	{
		0,		//gyro_float
		0,		//float_state
		0,		//gyro_float_flag1
		0,		//gyro_float_flag2
		0,		//gyro_float_flag
		0,		//gyro_error_flag
	},
	0,			//gyro_total
	0,			//gyro_total_nofloat
	0,			//gyro_radian_total
	0,			//gyro_angle_total
	0,			//gyro_float_total1
	0			//gyro_float_total2
};

static	struct Encoder_data encoder[ENCODER_NUM] = 
{
	{
		0,	//distance
		0,	//delta_distance
		0,	//num_of_rotation
		0,	//last_count
		0,	//total_count	
	},
	{
		0,	//distance
		0,	//delta_distance
		0,	//num_of_rotation
		0,	//last_count
		0,	//total_count	
	},
};

static	struct Encoder_data last_encoder[ENCODER_NUM] = 
{
	{0,	0,	0},{0,	0,	0}, //last_encoder
};


void Encoder_Update(void);
void Gyro_Clear(void);
void UART1_DMA_init(void);
void UART1_DMA_EN(void);
void Encoder_Timer_Config(void);


void GPS_update()
{
	Encoder_Update();
	double delta_x 					= 0;
	double delta_y 					= 0;
	float delta_distance1 			= 0;
	float delta_distance2 			= 0;
	static	float last_radian 		= 0;
	float radian 					= 0;
	float l							= 0;
	//if(fabs(gyro.gyro_angle_total - gps.angle) > 3.0)
	//	{return;}
// 	unsigned char i;
// 	unsigned char* pdata = (unsigned char *)&gps.angle;
// 	


// 	USART_SendData(USART3, 0x01);
// 	USART_SendData(USART3, 0x10);
// 	for(i = 0;i < 4;i++)
// 	{

// 		USART_SendData(USART3, *(pdata + i));
// 	}

	gps.radian = gyro.gyro_radian_total;
	gps.angle = gyro.gyro_angle_total;

	encoder[0].delta_distance = encoder[0].distance - last_encoder[0].distance;
	encoder[1].delta_distance = encoder[1].distance - last_encoder[1].distance;
	delta_distance1 = encoder[0].delta_distance;
	delta_distance2 = encoder[1].delta_distance;
	if(fabs(delta_distance1) < 1 && fabs(delta_distance2) < 1)
		{return;}
	//doc
	radian = (gps.radian + last_radian) / 2;

	if(radian>PI)
		radian-=2*PI;
	if(radian<-PI)
		radian+=2*PI;
	l = ( delta_distance1 + delta_distance2 ) / 2;
 	delta_x = l * sin(radian);
 	delta_y = l * cos(radian);

	gps.rear_point.y += delta_y;
	gps.rear_point.x += delta_x;
	last_encoder[0].distance = encoder[0].distance;
	last_encoder[1].distance = encoder[1].distance;
	last_radian = gps.radian;
	gps.front_point.x = gps.rear_point.x + encoder_property[0].carlen * sin(radian);
	gps.front_point.y = gps.rear_point.y + encoder_property[0].carlen * cos(radian);
	gps.send_status = 1;
}

signed int get_encoder(i)
{
	return (int32_t)((int32_t)(encoder[i].num_of_rotation)*encoder_property[i].count_per_rotation +(int32_t)(encoder_property[i].TIMx->CNT));
}

void Encoder_Update()
{
	int i = 0;
	signed int encoder_count = 0;
	signed int step;
	for(i = 0; i < 2; i ++)
	{
		encoder_count = get_encoder(i);
		step = encoder_count - encoder[i].last_count;
		if(step < 2000 && step > -2000)
		{
			encoder[i].last_count = encoder_count;
			encoder[i].total_count += step * encoder_property[i].direction;
			encoder[i].distance += step * encoder_property[i].convert * encoder_property[i].direction;
		}
	}
}

void Gyro_Update(void)
{

	struct Gyro*	pgyro = &gyro;
	pgyro->gyro_total+=pgyro->gyro_value.value;//1152 往大调是负偏


   pgyro->gyro_total_nofloat = pgyro->gyro_total - pgyro->gyro_calibration.gyro_float*pgyro->gyro_value.count;
	
	pgyro->gyro_angle_total = pgyro->gyro_total_nofloat*0.0000001*pgyro->gyro_convert1;
	
	pgyro->gyro_angle_total = pgyro->gyro_angle_total - (int)(pgyro->gyro_angle_total/360)*360;//强制类型转换直接舍去小数点
	
	if(pgyro->gyro_angle_total > 180.0)
		pgyro->gyro_angle_total = 360.0;
	if(pgyro->gyro_angle_total < -180.0)
		pgyro->gyro_angle_total += 360.0;
	pgyro->gyro_radian_total = pgyro->gyro_angle_total* 3.1415926 / 180;
}

void Gyro_CheckFloat(void)
{
	struct Gyro*	pgyro = &gyro;
	struct Gyro_calibration* pgyro_calibration = &gyro.gyro_calibration;
	LED2_on;
	Gyro_Clear();
	pgyro_calibration->gyro_float_flag=1;
	pgyro_calibration->gyro_float_flag1=1;
	pgyro_calibration->gyro_float_flag2=1;
	while(pgyro_calibration->gyro_float_flag)
	{
		delay_ms(1);
	if(pgyro->gyro_float_total2 - pgyro->gyro_float_total1  ==0)
		pgyro_calibration->float_state = CF_ERROR;
	else //if(abs(pgyro->gyro_float_total2 -pgyro->gyro_float_total1  - pgyro_calibration->gyro_float   * 1024) > 40960)//pgyro->gyro_float_total1  =pgyro->gyro_total;
	{
		pgyro_calibration->gyro_float = (pgyro->gyro_float_total2 -pgyro->gyro_float_total1  )/1024;//*Gyro_Float每一次的漂移量
		pgyro_calibration->float_state = CF_CHANGED;
	}

	}
	Gyro_Clear();
	if(pgyro_calibration->float_state == CF_ERROR)
	{
		while(1)
		{
			delay_ms(800);
			LED2_off;
			delay_ms(800);
			LED2_on;
		}
	}
	else
	{
		LED2_off;
	}
	
}
void Gyro_Clear()
{
	gyro.gyro_value.count = 0;
	gyro.gyro_value.value = 0;
}
void GPS_init()
{
	Encoder_Timer_Config();
	LED_Configuration();
	UART1_DMA_init();
	UART1_DMA_EN();
	Gyro_CheckFloat();
	gps.rear_point.y = 700;// 6200;
	gps.rear_point.x = 0;// 2641.2;
}






/*
USART
 */
void UART1_DMA_init()
{
    unsigned int baud = 38400;
	uint32_t i;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* USARTy RX DMA1 Channel --DMA1_Channel5(triggered by USART1 Rx event) Config */
	DMA_DeInit(DMA1_Channel5); 												
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Address;		
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_RxBuffer;	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;					
	DMA_InitStructure.DMA_BufferSize = 8;					
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;		
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;						
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;				
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;						
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);							



	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

	DMA_ITConfig(DMA1_Channel5,DMA_IT_TC, ENABLE);

	DMA_Cmd(DMA1_Channel5, DISABLE);
   
    USART_Cmd(USART1, ENABLE);  

} 

void UART1_DMA_EN(void)
{
	uint8_t gyro_start,j;
	uint8_t gyro_temp;
	gyro_temp = 0;
	gyro_start=1; 
	j=0;
	
	while(gyro_start)
	{
		if(j==0)
		{
			if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
			{
				USART_ClearFlag(USART1, USART_FLAG_ORE);
				gyro_temp = USART_ReceiveData(USART1);
			}

			if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
			{
				USART_ClearITPendingBit(USART1, USART_IT_RXNE);
				gyro_temp = USART_ReceiveData(USART1);
			}
			
			if(gyro_temp == 0xDD)
			{
				j=1;
			}
		}
		else
		{
			if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
			{
				USART_ClearFlag(USART1, USART_FLAG_ORE);
				gyro_temp = USART_ReceiveData(USART1);
				j++;
			}

			if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
			{
				USART_ClearITPendingBit(USART1, USART_IT_RXNE);
				gyro_temp = USART_ReceiveData(USART1);
				j++;
			}

			if(j==9)
			{
				if(gyro_temp==0xDD)
				{
					gyro_start = 0;		
					DMA_Cmd(DMA1_Channel5, ENABLE);
				}
				else
				{
					j=0;
				}
			}else if(j>9)
			{
				j=0;
			}
		}
	}
}

#ifdef GPS_ENABLE

void DMA1_Channel5_IRQHandler(void)
{
	struct Gyro* pgyro = &gyro;
	static union u8to32
	{
		int32_t value;
		uint8_t datas[4];
	}gyro;
    
	if(DMA_GetITStatus(DMA1_IT_GL5)==SET)
	{
		gyro.datas[3] = USART1_RxBuffer[1];
		gyro.datas[2] = USART1_RxBuffer[2];
		gyro.datas[1] = USART1_RxBuffer[0];
		gyro.datas[0] = 0;
		
		DMA_ClearITPendingBit(DMA1_IT_GL5);
		gyro.value >>=8;
		pgyro->gyro_value.value= gyro.value;
		pgyro->gyro_value.count++;
		Gyro_Update();
		if(pgyro->gyro_value.count==256&&pgyro->gyro_calibration.gyro_float_flag1)
		{
			pgyro->gyro_float_total1 = pgyro->gyro_total;
			pgyro->gyro_calibration.gyro_float_flag1=0;
		}
		if(pgyro->gyro_value.count==1280&&pgyro->gyro_calibration.gyro_float_flag2)
		{
            pgyro->gyro_float_total2 = pgyro->gyro_total;
			pgyro->gyro_calibration.gyro_float_flag2 = 0;
			pgyro->gyro_calibration.gyro_float_flag = 0;
		}
	}
}

#endif
/*
Timer
 */

void Encoder_Timer_Config()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	
	//config GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* configue timer */	
	TIM_DeInit(TIM3);		
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 
	TIM_TimeBaseStructure.TIM_Period = encoder_property[0].count_per_rotation - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = (u8)0;
	
	TIM_ICInit(TIM3, &TIM_ICInitStructure);

	TIM_ClearFlag(TIM3, TIM_FLAG_Update);	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	/* reset the timer3 counter */
	TIM3->CNT = (u16)0;
	/* enable the timer3 */
	TIM_Cmd(TIM3, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	TIM_DeInit(TIM8);		
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 
	TIM_TimeBaseStructure.TIM_Period = encoder_property[1].count_per_rotation - 1; 			
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = (u8)0;
	
	TIM_ICInit(TIM8, &TIM_ICInitStructure);

	TIM_ClearFlag(TIM8, TIM_FLAG_Update);	
	TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);
	
	/* reset the timer3 counter */
	TIM8->CNT = (u16)0;
	/* enable the timer3 */
	TIM_Cmd(TIM8, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

#ifdef GPS_ENABLE

void TIM3_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
    if(!(TIM3->CR1&0x0010))
    {/* count up overflow interrupt */
        encoder[0].num_of_rotation++;
    }
    else
    {/* count down overflow interrupt */
        encoder[0].num_of_rotation--;
    }
}
void TIM8_UP_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM8,TIM_IT_Update);
    if(!(TIM8->CR1&0x0010))
    {/* count up overflow interrupt */
        encoder[1].num_of_rotation++;
    }
    else
    {/* count down overflow interrupt */
        encoder[1].num_of_rotation--;
    }
}
#endif
