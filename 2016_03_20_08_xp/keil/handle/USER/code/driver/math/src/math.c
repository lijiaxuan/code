/* 
* @Author: obsidian
* @Date:   2015-12-16 09:53:22
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-18 14:13:42
*/
#include "../inc/math.h"
#include "math.h"
float get_point2line_length(struct Point p_point,struct Point p_line_point,float angle)
{
	float k = tan((90 - angle) * PI / 180);
	float a = p_point.y - k*p_point.x + k * p_line_point.x - p_line_point.y;
	return a/(sqrt(1 + k*k));
}
float get_point2point_length(struct Point p_line_point1,struct Point p_line_point2)
{
	return sqrt((p_line_point1.x - p_line_point2.x)*(p_line_point1.x - p_line_point2.x) + ((p_line_point1.y - p_line_point2.y)*(p_line_point1.y - p_line_point2.y)));
}
float angletoradian(float angle)
{
	return angle * PI / 180.0;
}
float radiantoangle(float radian)
{
	return radian * 180.0 / PI;
}