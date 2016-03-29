/* 
* @Author: obsidian
* @Date:   2015-12-15 19:59:14
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-17 11:57:08
*/
#include "../inc/steer.h"
#include "../inc/steer_global.h"
#include "driver/timer/inc/timer.h"
struct Steer steer =
{
	(unsigned short *)&TIM4->CCR4,
	0,
	TIM4_Mode_Config,
	steer_set_angle
};