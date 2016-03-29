/* 
* @Author: obsidian
* @Date:   2015-12-13 22:08:31
* @Last Modified by:   obsidian
* @Last Modified time: 2015-12-19 14:10:08
*/
#include "../inc/camera_global.h"
#include "driver/can/inc/can.h"
struct Camera camera = 
{
	{
		0,	//status
		0,	//angle
		0,	//distance to center
	},
	Can1_Init
};