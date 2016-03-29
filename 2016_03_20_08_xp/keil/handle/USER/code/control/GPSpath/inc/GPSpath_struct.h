/* 
* @Author: obsidian
* @Date:   2015-12-16 09:43:04
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-18 14:37:46
*/
#ifndef GPSPATH_STRUCT_H
#include "module/GPS/inc/GPS_global.h"

struct GPSpath
{
	float			target_angle;
	float			distance_error;
	float 			angle_error;
	float			mixed_error;
	//void			(*gps_set_path)(float **path);
	void 			(*gps_path_update)();
	unsigned short	status;
};

#define GPSPATH_STRUCT_H
#endif