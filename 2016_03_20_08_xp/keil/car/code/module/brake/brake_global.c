/* 
* @Author: obsidian
* @Date:   2015-12-15 19:59:14
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-08 21:01:16
*/
#include "brake.h"
#include "brake_global.h"
#include "stm32f10x_tim.h"


struct Brake brake =
{
	(unsigned short *)&TIM4->CCR3,         //CCRx
	0,                                     //status
    1300,                                  //midvalue
    {140, 130},                            //limit
    2.0,                                   //set_angle
	brake_init,                     //
	brake_set_angle,
    brake_save,
};
