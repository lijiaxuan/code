/* 
* @Author: obsidian
* @Date:   2015-12-15 19:59:14
* @Last Modified by:   obsidian
* @Last Modified time: 2016-02-25 18:44:57
*/
#include "../inc/brake.h"
#include "../inc/brake_global.h"



struct Brake brake =
{
	(unsigned short *)&TIM4->CCR3,
	0,
	TIM43_Mode_Config,
	brake_set_angle,
    0
};