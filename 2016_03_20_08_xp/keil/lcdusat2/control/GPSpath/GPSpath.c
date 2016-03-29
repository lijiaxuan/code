/* 
* @Author: obsidian
* @Date:   2015-12-16 09:43:22
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-10 10:47:12
*/

#include "GPSpath.h"
#include "GPSpath_global.h"
#include "driver/sd/file.h"


#include "module/GPS/GPS_global.h"

#include "module/pid/pid_global.h"
#include "driver/math/inc/math.h"
#include "pointpath.h"
#include "module/steer/steer_global.h"
#include "module/camera/camera_global.h"
#include "driver/led/inc/led.h"
#include "math.h"
#include "module/def.h"
static int Route[25]={0, 162, 258, 402, 498, 639, 735, 879, 975, 1116, 1203, 1373, 1522, 1625, 1707, 1789, 1947, 2097, 2391, 2655, 2773, 3018, 3096, 3262, 3713,};


struct Path_point judge_direc(unsigned short index)
{
	struct Path_point   point			= {0};
	if(gps.direction == 2)
	{
		point.point.x = - path[index].point.x;
		point.point.y =   path[index].point.y;
		point.target_radian = - path[index].target_radian;
	}
	return point;
}

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





	tmp_len_min = get_point2point_length(judge_direc(gpspath.current_index+1).point,gps.front_point);
	min_len_index = gpspath.current_index;

	for(index = gpspath.current_index;index < gpspath.current_index + 5; index ++)
	{
		if(gpspath.current_index > PATH_LEN) {return;}
		
		tmp_len = get_point2point_length(judge_direc(index).point,gps.front_point);
		
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
	gpspath.pre = (unsigned char )gps.speed * 3.0 / 1000.0; 
	//get error
	//#ifdef LEFT 
	angle_error = (judge_direc(min_len_index + gpspath.pre).target_radian - gps.radian) * 180 / PI;
	// #endif
	// #ifdef RIGHT
	// 	angle_error = ( - judge_direc(min_len_index + gpspath.pre].target_radian - gps.radian) * 180 / PI;
	// #endif

	if(angle_error > 180)
	{
		angle_error = angle_error - 360;
	}
	else if(angle_error < - 180)
	{
		angle_error = angle_error + 360;
	}

	float min_len_radian = judge_direc(min_len_index).target_radian;
	if(min_len_radian == 0)
	{
		distance_error =judge_direc(min_len_index).point.x -  gps.front_point.x;
	}
	else if(min_len_radian > 0)
	{
		distance_error = get_point2line_length(gps.front_point, judge_direc(min_len_index).point,min_len_radian * 180.0 / PI);
	}
	else if(min_len_radian < 0)
	{
		distance_error = - get_point2line_length(gps.front_point, judge_direc(min_len_index).point,min_len_radian * 180.0 / PI);
	}
	if(gps.front_point.y > 7350)
	{
		distance_error = (camera.offset-gpspath.camera_center) * gpspath.camera_k;
	}
	mixed_error = gpspath.angle_k * distance_error + gpspath.distance_k  *  angle_error;

	gpspath.distance_error = distance_error;
	gpspath.target_angle =  judge_direc(min_len_index  + gpspath.pre).target_radian * 180 / PI;
	gpspath.angle_error = angle_error;
	gpspath.mixed_error = mixed_error;
 	
 	mend = pid.get_pid(mixed_error);
	steer.set_angle(mend);

	unsigned char* pdata;
	unsigned char i = 0;
	static float an = 0;

	if(gps.send_status == 1 )
	{
 		gps.send_status = 0;
	}
}

static struct GPSpath dup_gpspath;

void gpspath_init()
{
    unsigned int k = 0;
    f_open(&fil,"gpspath",FA_READ);
    if(fil.fs == 0){return;}
    f_read(&fil, (char *)&dup_gpspath, sizeof(struct GPSpath), &k);
    gpspath.angle_k = dup_gpspath.angle_k;
    gpspath.distance_k = dup_gpspath.distance_k;
    gpspath.camera_k = dup_gpspath.camera_k;
    gpspath.pre = gpspath.pre;
    file_close();
}
void gpspath_save()
{
    unsigned int k = 0;
    f_open(&fil,"gpspath",FA_WRITE | FA_CREATE_ALWAYS);
    if(fil.fs == 0){return;}
    f_write(&fil, (char *)&gpspath, sizeof(struct GPSpath), &k);
    file_close();
}


void timer_cfg(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1; 
	TIM_TimeBaseStructure.TIM_Period = 2000; // 5000 - 5ms
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4,ENABLE);

}

void TIM4_IRQHandler(void)
{
	TIM_ClearFlag(TIM4,TIM_IT_Update);
	gps.GPS_update();
	gpspath.gps_path_update();
}
