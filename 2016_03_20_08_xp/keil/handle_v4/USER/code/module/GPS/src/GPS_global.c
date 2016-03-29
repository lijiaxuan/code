/* 
* @Author: obsidian
* @Date:   2015-12-13 22:04:42
* @Last Modified by:   obsidian
* @Last Modified time: 2016-02-27 11:16:59
*/
#include "../inc/GPS_global.h"
#include "../inc/GPS.h"

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
	0,		//radian
	0,		//angle
	Gyro_Update,
	GPS_init,
	GPS_update,
	0,     //status
	0,     //base_radian
	287.5,  //carlen
	0,     //status
	0,//d_dis
	0,//speed
};

// 0x535
// 0x5c8
// 0x665
