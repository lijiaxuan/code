/* 
* @Author: obsidian
* @Date:   2015-12-16 09:43:04
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-30 15:47:54
*/
#ifndef GPSPATH_STRUCT_H
#include "module/GPS/inc/GPS_global.h"

#ifndef ON
#define ON 1
#endif

#ifndef OFF
#define OFF 0
#endif

struct GPSpath
{
	float			target_angle;
	float			distance_error;
	float 			angle_error;
	float			mixed_error;
	//void			(*gps_set_path)(float **path);
	void 			(*gps_path_update)();
	unsigned short	status;
    unsigned int    current_index;
    void            (*timer_cfg)();
};

#define GPSPATH_STRUCT_H
#endif