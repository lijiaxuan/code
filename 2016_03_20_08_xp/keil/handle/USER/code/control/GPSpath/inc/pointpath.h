/* 
* @Author: obsidian
* @Date:   2015-12-16 09:43:53
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-17 10:31:09
*/
#ifndef POINT_PATH_H

#include "module/GPS/inc/GPS_global.h"


#define PATH_LEN 3740


struct Path_point
{
	struct Point 		point;
	float				target_radian;
};

extern struct Path_point path[PATH_LEN];

#define POINT_PATH_H
#endif