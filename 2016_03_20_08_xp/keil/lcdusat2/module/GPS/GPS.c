/* 
* @Author: obsidian
* @Date:   2015-12-13 22:04:31
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-10 20:44:48
*/
#include "GPS.h"
#include "GPS_global.h"


#include "driver/delay/inc/delay.h"
#include "driver/math/inc/math.h"
#include "driver/led/inc/led.h"
#include "driver/sd/file.h"

#include "gps_driver.h"
#include "math.h"

#ifndef PI
#define PI 3.1415926
#endif

static	struct Encoder_data last_encoder[ENCODER_NUM] = 
{
	{0,	0,	0},{0,	0,	0}, //last_encoder
};


void Encoder_Update(void);
void Gyro_Clear(void);
void UART1_DMA_EN(void);
void Gyro_CheckFloat(void);


void GPS_init()
{
	Encoder_Timer_Config();
	UART1_DMA_init();
	UART1_DMA_EN();
	Gyro_CheckFloat();
}

static struct GPS dup_gps;
static struct Gyro dup_gyro;
static struct Encoder_property dup_encoder[2];

void gps_init()
{
	GPS_init();
	unsigned int k = 0;
	f_open(&fil,"GPS",FA_READ);
	if(fil.fs == 0){return;}
	f_read(&fil, (char *)&dup_gps, sizeof(struct GPS), &k);
	gps.start_point.x = dup_gps.start_point.x;
	gps.start_point.y = dup_gps.start_point.y;
	gps.start_angle = dup_gps.start_angle;
	file_close();
	
	f_open(&fil,"encoder",FA_READ);
	if(fil.fs == 0){return;}
	f_read(&fil, (char *)dup_encoder, 2 * sizeof(struct Encoder_property), &k);
	encoder_property[0].convert = dup_encoder[0].convert;
	encoder_property[1].convert = dup_encoder[1].convert;
	file_close();
	
	f_open(&fil,"gyro",FA_READ);
	if(fil.fs == 0){return;}
	f_read(&fil, (char *)&dup_gyro, sizeof(struct Gyro), &k);
	gyro.gyro_convert1 = dup_gyro.gyro_convert1;
	gyro.gyro_convert2 = dup_gyro.gyro_convert2;
	file_close();
	gps.rear_point.x = gps.start_point.x;
	gps.rear_point.y = gps.start_point.y;
	gps.base_radian = gps.start_angle * PI / 180.0;
	gps.front_point.x = gps.rear_point.x + gps.car_len * sin(gps.base_radian);
	gps.front_point.y = gps.rear_point.y + gps.car_len * cos(gps.base_radian);

}
void gps_save()
{
	unsigned int k = 0;
	f_open(&fil,"GPS",FA_WRITE | FA_CREATE_ALWAYS);
	if(fil.fs == 0){return;}
	f_write(&fil, (char *)&gps, sizeof(struct GPS), &k);
	file_close();
	f_open(&fil,"encoder",FA_WRITE | FA_CREATE_ALWAYS);
	if(fil.fs == 0){return;}
	f_write(&fil, (char *)&encoder_property, 2 * sizeof(struct Encoder_property), &k);
	file_close();
	f_open(&fil,"gyro",FA_WRITE | FA_CREATE_ALWAYS);
	if(fil.fs == 0){return;}
	f_write(&fil, (char *)&gyro, sizeof(struct Gyro), &k);
	file_close();
}

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
	if(gps.status == OFF)
	{
		return;
	}

	gps.radian = gyro.gyro_radian_total  +gps.base_radian;
	gps.angle = gyro.gyro_angle_total + gps.base_radian * 180 / PI;

	encoder[0].delta_distance = encoder[0].distance - last_encoder[0].distance;
	encoder[1].delta_distance = encoder[1].distance - last_encoder[1].distance;
	
	delta_distance1 = encoder[0].delta_distance;
	delta_distance2 = encoder[1].delta_distance;
	gps.speed = (fabs(delta_distance1) + fabs(delta_distance1)) * 200;
	
	if(fabs(delta_distance1) < 1 && fabs(delta_distance2) < 1)
		{return;}
	//doc
	if(fabs(delta_distance1) > fabs(delta_distance2))
	{
		gps.d_dis = delta_distance1;
	}
	else
	{
		gps.d_dis = delta_distance2;
	}
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
	
	gps.front_point.x = gps.rear_point.x + gps.car_len * sin(radian);
	gps.front_point.y = gps.rear_point.y + gps.car_len * cos(radian);
	
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
		{pgyro_calibration->float_state = CF_ERROR;}
		else 
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
	}
	LED2_off;
}
void Gyro_Clear()
{
	gyro.gyro_value.count = 0;
	gyro.gyro_value.value = 0;
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

/*
Timer
 */

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
void TIM2_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
    if(!(TIM2->CR1&0x0010))
    {/* count up overflow interrupt */
        encoder[1].num_of_rotation++;
    }
    else
    {/* count down overflow interrupt */
        encoder[1].num_of_rotation--;
    }
}
