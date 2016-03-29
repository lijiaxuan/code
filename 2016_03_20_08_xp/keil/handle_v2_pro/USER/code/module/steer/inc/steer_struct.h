/* 
* @Author: obsidian
* @Date:   2015-12-15 19:55:59
* @Last Modified by:   obsidian
* @Last Modified time: 2015-12-20 16:37:53
*/
#ifndef STEER_STRUCT_H

#include "stm32f10x_tim.h"

struct Steer
{
	unsigned short	*CCRx;
	float			steer_angle;
	void			(*steer_init)();
	void			(*set_angle)(float angle);
};

#define STEER_STRUCT_H
#endif
