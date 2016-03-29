/* 
* @Author: obsidian
* @Date:   2015-12-15 19:40:01
* @Last Modified by:   obsidian
* @Last Modified time: 2015-12-15 20:32:53
*/
#ifndef PROPELLER_STRUCT_H
#include "stm32f10x_tim.h"


struct Propeller
{
	unsigned short 	*CCRx;
	void			(*propeller_init)(int t1, int t2);
	void			(*set_angle)(float angle);
};

#define PROPELLER_STRUCT_H
#endif

