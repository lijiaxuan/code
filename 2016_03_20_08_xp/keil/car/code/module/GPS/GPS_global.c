/* 
* @Author: obsidian
* @Date:   2015-12-13 22:04:42
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-10 20:45:17
*/
#include "GPS_global.h"
#include "GPS.h"

struct GPS gps =
{

	{
		0,  //x
		0	//y
	},
	{
		0,
		0
	},
	{
		0,
		0
	},
	0,		//start_angle
	0,		//radian
	0,		//angle
	0,     	//status
	0,     	//base_radian
	287.5,  //carlen
	0,     	//status
	0,		//direction
	0,		//d_dis
	0,		//speed
	gps_init,
	Gyro_Update,
	GPS_init,
	GPS_update,
	gps_save,
};

struct Encoder_property encoder_property[ENCODER_NUM] = 
{
	//count_per_rotation  R 			TIMx     direction   			convert
	{	2048,					0,				TIM3,		-1,			0.1026483268},
	{   2048,					0,				TIM8,		1,			0.1037775010}

};


struct Gyro gyro = 
{
	1.60011,		//gyro_convert1 //1.60011
	1.60322,		//gyro_convert2 //1.60322
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
struct Encoder_data encoder[ENCODER_NUM] = 
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
