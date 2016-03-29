/* 
* @Author: obsidian
* @Date:   2015-12-15 19:59:14
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-08 21:04:13
*/
#include "steer.h"
#include "steer_global.h"
#include "stm32f10x_tim.h"


struct Steer steer =
{
	(unsigned short *)&TIM4->CCR4,         //CCRx  short
	0,                                    //status  s char
    1500,                                   //midvalue short
	{153, 171},                          //limit[2]
    3.8,                                    //ratio
    steer_init,                      //
	steer_set_angle,                       //
    steer_save,
};
