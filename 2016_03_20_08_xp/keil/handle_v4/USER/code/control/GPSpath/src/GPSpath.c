/* 
* @Author: obsidian
* @Date:   2015-12-16 09:43:22
* @Last Modified by:   obsidian
* @Last Modified time: 2016-02-28 14:31:54
*/

#include "../inc/GPSpath.h"
#include "module/GPS/inc/GPS_global.h"
#include "../inc/GPSpath_global.h"
#include "module/pid/inc/pid_global.h"
#include "driver/math/inc/math.h"
#include "driver/delay/inc/delay.h"
#include "../inc/pointpath.h"
#include "module/steer/inc/steer_global.h"
#include "module/camera/inc/camera_global.h"
#include "driver/led/inc/led.h"
#include "math.h"
#include "stm32f10x_usart.h"
#include "module/bluetooth/inc/bluetooth_global.h"
#include "module/def.h"
#include "module/radar/inc/radar_global.h"
#include "module/brake/inc/brake_global.h"
#include "module/infrared_s/inc/infrared_global.h"
static int Route[25]={0, 162, 258, 402, 498, 639, 735, 879, 975, 1116, 1203, 1373, 1522, 1625, 1707, 1789, 1947, 2097, 2391, 2655, 2773, 3018, 3096, 3262, 3713,};

static unsigned char  PRO =  15;

void gps_path_update()
{
	unsigned short		index 			= 0;
	unsigned short		delta_index 	= 0;
	float				angle_error 	= 0;
	float				distance_error 	= 0;
	float 				tmp_len_min 	= 0;
	float				tmp_len 		= 0;
	signed short		min_len_index 	= 0;
	float 				mend 			= 0;
	float				mixed_error		= 0;
	signed char			route_index 	= 0;
	if(gpspath.status == OFF)
		{return;}






	tmp_len_min = get_point2point_length(path[gpspath.current_index+1].point,gps.front_point);
	min_len_index = gpspath.current_index;

	for(index = gpspath.current_index;index < gpspath.current_index + 5; index ++)
	{
		if(gpspath.current_index > PATH_LEN) return;
		tmp_len = get_point2point_length(path[index].point,gps.front_point);
		if(tmp_len < tmp_len_min)
		{
			min_len_index = index;	
		}
	}

	
	for(route_index = pid.route_id ; route_index < 25 ; route_index ++)
	{
		if(min_len_index < Route[route_index])
		{
			if(pid.route_id == route_index - 1)
			{
				break;
			}
			else
			{
 				pid.mend[route_index - 1] = pid.mend[pid.route_id];
				pid.error[route_index - 1][0] = pid.error[pid.route_id][0];
 				pid.error[route_index - 1][1] = pid.error[pid.route_id][1];
				pid.route_id = route_index - 1;
				break;
			}
		}
	}
	
	gpspath.current_index = min_len_index;
	//PRO = (unsigned char )gps.speed * 5.0 / 1000.0; 
	//get error
	#ifdef LEFT 
		angle_error = (path[min_len_index + PRO].target_radian - gps.radian) * 180 / PI;
	#endif
	#ifdef RIGHT
		angle_error = ( path[min_len_index + PRO].target_radian - gps.radian) * 180 / PI;
	#endif

	if(angle_error > 180)
	{
		angle_error = angle_error - 360;
	}
	else if(angle_error < - 180)
	{
		angle_error = angle_error + 360;
	}
	if(path[min_len_index].target_radian == 0)
	{
		distance_error =path[min_len_index].point.x -  gps.front_point.x;
	}
	else if(path[min_len_index].target_radian > 0)
	{
		distance_error = get_point2line_length(gps.front_point, path[min_len_index].point,path[min_len_index].target_radian * 180.0 / PI);
	}
	else if(path[min_len_index].target_radian < 0)
	{
		distance_error = - get_point2line_length(gps.front_point, path[min_len_index].point,path[min_len_index].target_radian * 180.0 / PI);
	}
	if(gps.front_point.y > 7350 - 80 && camera.offset!= 0)
	{
		distance_error = (camera.offset-40)*300/80;
	}
		mixed_error = 1.0 * distance_error + 4.5  *  angle_error;
  //  mixed_error = 5.5  *  angle_error;
	// if(gpspath.current_index >= 1755 && gpspath.current_index <= 1830 && radar.status == ON)
	// {
	// 	radar.pre_point.x = gps.rear_point.x;
	// 	radar.pre_point.y = gps.rear_point.y;
	// 	radar.pre_radian = gps.radian;
	// 	radar.scan();
	// 	radar.status = OFF;
// 	// }
	gpspath.distance_error = distance_error;
	gpspath.target_angle =  path[min_len_index  + PRO].target_radian * 180 / PI;
	gpspath.angle_error = angle_error;
	gpspath.mixed_error = mixed_error;
 	mend = pid.get_pid(mixed_error);
	steer.set_angle(mend);
	//steer.set_angle(camera.offset-40);

	unsigned char* pdata;
	unsigned char i = 0;
	static float an = 0;
	static char stopflag  = 1;
	if(gps.send_status == 1 )
	{

// 		if(SA4 || SA2)
// 		{
// 			infrared.flag = 1;
// 		}
// 		else if(SA3 || SA1)
// 		{
// 			infrared.flag = -1;
// 		}
// 		else
// 		{
// 			infrared.flag = 0;
// 		}
// 		if(gps.front_point.y > 7350)
// 		{
// 			//gps.rear_point.x -= infrared.flag * 1.0;
// 			rotate((float *)&gps.rear_point, -infrared.flag * infrared.step * gps.d_dis / gps.rear_point.y);
// 			gps.base_radian += infrared.flag * infrared.step * gps.d_dis / gps.rear_point.y;
// 			gps.front_point.x = gps.rear_point.x + gps.car_len * sin(gps.radian);
// 			gps.front_point.y = gps.rear_point.y + gps.car_len * cos(gps.radian);
// 			
// 		}

		if(min_len_index > 2680 && min_len_index < 2760)
		{
			TIM4->CCR3 = 0x655;
		}
 		else if(min_len_index > 2900 && min_len_index < 3050 && brake.stopflag)
		//else if(min_len_index > 2200 && min_len_index < 2230 && brake.stopflag)
		{
				TIM4->CCR3 = 0x66d;
				TIM_Cmd(TIM6,ENABLE);
		}
		else 
		{
			TIM4->CCR3 = 0x560;
		}
// 	if(min_len_index > 2885 && min_len_index < 2945)
// 	{
// 		TIM4->CCR3 = 0x6f0;
// 	}
// 	else if(min_len_index > 2515 && min_len_index < 2565)
// 	{
// 		TIM4->CCR3 = 0x6f0;
// 	}
// 	else if(min_len_index > 2720 && min_len_index < 2790)
// 	{
// 		TIM4->CCR3 = 0x6f0;
// 	}
// 	else
// 	{
// 		TIM4->CCR3 = 0x787;
// 	}

// 		an += infrared.flag * infrared.step * 57.3;
// 		steer.set_angle(an);
		bluetooth.send_head();
		bluetooth.send((unsigned char *)&gps.front_point.x,4);
		bluetooth.send((unsigned char *)&gps.front_point.y,4);
		bluetooth.send((unsigned char *)&gpspath.distance_error,4);
		bluetooth.send((unsigned char *)&gpspath.angle_error,4);
		bluetooth.send((unsigned char *)&pid.mend[pid.route_id],4);
		bluetooth.send((unsigned char *)&path[min_len_index  + PRO].point.x,4);
		bluetooth.send((unsigned char *)&path[min_len_index  + PRO].point.y,4);
 		gps.send_status = 0;
	}
}


void timer_cfg(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1; 
	TIM_TimeBaseStructure.TIM_Period = 2000; // 5000 - 5ms
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM5,ENABLE);

}

void TIM5_IRQHandler(void)
{
	TIM_ClearFlag(TIM5,TIM_IT_Update);
	// radar.counter = (radar.counter + 1)%200;
	// if((radar.counter == 0 ) && (radar.status))
	// {
	// radar.scan();
	// }
	gps.GPS_update();
	gpspath.gps_path_update();
}
