/* 
* @Author: obsidian
* @Date:   2016-01-03 17:16:43
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-17 08:52:12
*/
#include "../inc/bluetooth_global.h"

#include "module/GPS/inc/GPS_global.h"

struct Bluetooth bluetooth = 
{
	8,
	(char *)( &gps.point),
	init,
	send
};