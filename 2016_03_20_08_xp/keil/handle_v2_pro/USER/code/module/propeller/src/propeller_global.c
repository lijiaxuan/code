/* 
* @Author: obsidian
* @Date:   2015-12-15 19:40:12
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-18 09:16:18
*/
#include "../inc/propeller_global.h"
#include "../inc/propeller.h"
#include "driver/timer/inc/timer.h"
struct Propeller propeller = 
{
	(unsigned short*)&TIM2->CCR3,
	TIM4_Mode_Config,
	propeller_set_angle
};