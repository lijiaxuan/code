/* 
* @Author: obsidian
* @Date:   2015-12-16 09:43:22
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-24 19:34:35
*/

#include "../inc/GPSpath.h"
#include "module/GPS/inc/GPS_global.h"
#include "../inc/GPSpath_global.h"
#include "module/pid/inc/pid_global.h"
#include "driver/math/inc/math.h"
#include "../inc/pointpath.h"
#include "module/steer/inc/steer_global.h"
#include "driver/led/inc/led.h"
#include "math.h"
#include "stm32f10x_usart.h"
#include "module/bluetooth/inc/bluetooth_global.h"

static int Route[25]={0, 162, 258, 402, 498, 639, 735, 879, 975, 1116, 1203, 1373, 1522, 1625, 1707, 1789, 1947, 2097, 2391, 2655, 2773, 3018, 3096, 3262, 3713,};

static unsigned char  PRO =  20;

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
	static	unsigned short current_index  = 0 ;
	if(gpspath.status == 0)
		{return;}
	// {






	tmp_len_min = get_point2point_length(path[current_index+1].point,gps.front_point);
	min_len_index = current_index;

	for(index = current_index;index < current_index + 9; index ++)
	{
		if(current_index > PATH_LEN) return;
		tmp_len = get_point2point_length(path[index].point,gps.front_point);
		if(tmp_len < tmp_len_min)
		{
			min_len_index = index;	
		}
	}

	
	for(route_index = pid.route_id ; route_index < 24 ; route_index ++)
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
				if(pid.route_id == 10 || pid.route_id == 9|| pid.route_id == 11 || pid.route_id == 8)
				{
					PRO = 25;
				}
				else
				{
					PRO = 20;
				}
				break;
			}
		}
	}
	
	current_index = min_len_index;
	
	//get error
	angle_error = (path[min_len_index + PRO].target_radian - gps.radian) * 180 / PI;

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
		distance_error = gps.front_point.x;
	}
	else if(path[min_len_index].target_radian > 0)
	{
		distance_error = get_point2line_length(gps.front_point, path[min_len_index].point,path[min_len_index].target_radian * 180.0 / PI);
	}
	else if(path[min_len_index].target_radian < 0)
	{
		distance_error = - get_point2line_length(gps.front_point, path[min_len_index].point,path[min_len_index].target_radian * 180.0 / PI);
	}

	mixed_error = 1.0 * distance_error + 4.5  *  angle_error;


	gpspath.distance_error = distance_error;
	gpspath.target_angle =  path[min_len_index  + PRO].target_radian * 180 / PI;
	gpspath.angle_error = angle_error;
	gpspath.mixed_error = mixed_error;

	
 	mend = pid.get_pid(mixed_error);
	steer.set_angle(mend);


	unsigned char* pdata;
	unsigned char i = 0;
	if(gps.send_status == 1)
	{
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
