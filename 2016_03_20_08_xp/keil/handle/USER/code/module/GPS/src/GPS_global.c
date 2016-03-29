/* 
* @Author: obsidian
* @Date:   2015-12-13 22:04:42
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-18 14:09:56
*/
#include "../inc/GPS_global.h"
#include "../inc/GPS.h"

struct GPS gps =
{

	{
		0,  //x
		0	//y
	},
	0,		//radian
	0,		//angle
	Gyro_Update,
	GPS_init,
	GPS_update,
};
