/* 
* @Author: obsidian
* @Date:   2015-12-15 19:59:14
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-29 09:09:45
*/
#include "../inc/steer.h"
#include "../inc/steer_global.h"



struct Steer steer =
{
	(unsigned short *)&TIM4->CCR4,
	0,
	TIM4_Mode_Config,
	steer_set_angle,
    0
};