/* 
* @Author: obsidian
* @Date:   2015-12-15 19:55:59
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-30 08:40:10
*/
#ifndef STEER_STRUCT_H

#include "stm32f10x_tim.h"

#define MIDVALUE 1430
#define eff             3.8
#define SCALE           171

#ifndef ON
#define ON 1
#endif

#ifndef OFF
#define OFF 0
#endif

struct Steer
{
	unsigned short	*CCRx;
	float			steer_angle;
	void			(*steer_init)();
	void			(*set_angle)(float angle);
    unsigned char   status;
};

#define STEER_STRUCT_H
#endif
