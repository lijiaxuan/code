/* 
* @Author: obsidian
* @Date:   2015-12-16 09:43:29
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-10 10:46:30
*/

#include "GPSpath_global.h"
#include "GPSpath.h"

struct  GPSpath gpspath = 
{
	0,	                //status
	0,                  //current_index
	0,                  //direction
	0,                  //target_radian
	0,                  //distance_error
    0,                  //angle_error
    0,                  //mixed_error
    1.0,                 //angle_k
    4.5,                 //distance_k
    600.0/80.0,          //camera_k
    0,                  //pre
    37,                 //camera_center
    gpspath_init,
    gps_path_update,    //
    timer_cfg,
    gpspath_save,
};
