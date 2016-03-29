/* 
* @Author: obsidian
* @Date:   2015-12-16 09:43:04
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-30 15:47:54
*/
#ifndef GPSPATH_STRUCT_H
#include "module/GPS/GPS_global.h"

#ifndef ON
#define ON 1
#endif

#ifndef OFF
#define OFF 0
#endif

struct GPSpath
{
    unsigned short  status;
    unsigned int    current_index;
    unsigned char   direction;
	float			target_angle;
	float			distance_error;
	float 			angle_error;
	float			mixed_error;
    float           angle_k;
    float           distance_k;
    float           camera_k;
    char            pre;
    short           camera_center;
    void            (*init)();
	void 			(*gps_path_update)();
    void            (*timer_driver)();
    void            (*save)();
};

#define GPSPATH_STRUCT_H
#endif