/* 
* @Author: obsidian
* @Date:   2015-12-13 22:04:07
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-30 08:47:28
*/
#ifndef ENCODER_STRUCT_H
#include "stm32f10x_tim.h"
#include "module/def.h"
#define  ENCODER_NUM 2

#define CF_ERROR   6
#define CF_CHANGED 7
#define CF_NORMAL  8

#ifndef ON
#define ON 1
#endif

#ifndef OFF
#define OFF 0
#endif

/*
	ENCODER
 */
struct Encoder_property
{
	unsigned short		count_per_rotation;
	float				R;
	TIM_TypeDef*		TIMx;
	signed char			direction;
	float				convert;
};

struct Encoder_data
{
	float			distance;
	float			delta_distance;
	signed short 	num_of_rotation;
	signed int    	last_count;
	signed int		total_count;
};

/*
	GYRO
 */
struct Gyro_value
{
	signed int	value;
	signed int	count;
};

struct Gyro_calibration
{
	float			gyro_float;
	unsigned char	float_state;
	unsigned char	gyro_float_flag1;
	unsigned char	gyro_float_flag2;
	unsigned char	gyro_float_flag;
	unsigned char	gyro_error_flag;
};

struct Gyro
{
	float 						gyro_convert1;
	float 						gyro_convert2;
	struct 	Gyro_value 			gyro_value;
	struct  Gyro_calibration 	gyro_calibration;
	float				gyro_total;
	float				gyro_total_nofloat;
	float				gyro_radian_total;
	float 				gyro_angle_total;
	float				gyro_float_total1;
	float 				gyro_float_total2;
};


/*
	GPS = ENCODER + GYRO
 */


struct GPS
{
	struct Point        front_point;
	struct Point 		rear_point;
	struct Point 		start_point;
	float				start_angle;
	float				radian;
	float				angle;
	unsigned char		send_status;
	float				base_radian;
	float				car_len;
	unsigned char		status;
	unsigned char		direction;
	float				d_dis;
	float				speed;
	void 				(*init)();
	void				(*gyro_update)();
	void				(*GPS_init)();
	void				(*GPS_update)();
	void				(*save)();
};

#define ENCODER_STRUCT_H
#endif
