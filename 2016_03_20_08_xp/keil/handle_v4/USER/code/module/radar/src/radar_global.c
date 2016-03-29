/* 
* @Author: obsidian
* @Date:   2016-01-16 14:57:29
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-30 16:52:30
*/
#include "../inc/radar_global.h"
#include "../inc/radar.h"
#include "stm32f10x.h"
struct Radar_commands radar_command = 
{
	{0x47,0x53,0x30,0x32,0x30,0x30,0x30,0x36,0x34,0x30,0x30,0x31,0x0d,0x0a},
	{0x42,0x4d,0x0d,0x0a},
	{0x51,0x54,0x0d,0x0a,0x0d}
};
struct Radar radar = 
{
	USART3,
	radar_init,
	update,
	0,		//counter
	0,		//status
	0,		//radian
	0,		//x
	0,		//y
	325,	//len
	{0},	//prepoint
	0,
	scan
};
