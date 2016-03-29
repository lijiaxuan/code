/* 
* @Author: obsidian
* @Date:   2015-12-16 09:53:14
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-18 14:12:40
*/
#ifndef MATH_H

#include "module/GPS/inc/GPS.h"
#ifndef PI
#define PI	3.1415926
#endif

float get_point2line_length(struct Point p_point,struct Point p_line_point,float angle);
float get_point2point_length(struct Point p_line_point1,struct Point p_line_point2);
float angletoradian(float angle);
float radiantoangle(float radian);
#define MATH_H
#endif
