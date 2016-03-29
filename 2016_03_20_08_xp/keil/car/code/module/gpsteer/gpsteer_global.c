/* 
* @Author: obsidian
* @Date:   2015-12-15 19:59:14
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-19 20:56:11
*/
#include "gpsteer.h"
#include "gpsteer_global.h"
#include "stm32f10x_tim.h"


struct Gpsteer gpsteer =
{
	(unsigned short *)&TIM4->CCR3,         //CCRx
	0,                                     //status
    1300,                                  //midvalue
    {140, 130},                            //limit
    2.0,                                   //set_angle
	gpsteer_init,                     //
	gpsteer_release,
    gpsteer_save,
};
