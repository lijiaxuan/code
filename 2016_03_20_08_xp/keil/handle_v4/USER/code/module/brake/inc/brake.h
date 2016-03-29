/* 
* @Author: obsidian
* @Date:   2015-12-15 19:55:53
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-30 08:39:47
*/

#ifndef BRAKE_H
#include "brake_struct.h"

void TIM43_Mode_Config(void);
void brake_set_angle(float angle);
void brake_timer_cfg(void);
#define BRAKE_H
#endif
