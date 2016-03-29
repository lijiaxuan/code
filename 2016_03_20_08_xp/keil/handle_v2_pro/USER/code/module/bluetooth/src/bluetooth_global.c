/* 
* @Author: obsidian
* @Date:   2016-01-03 17:16:43
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-24 16:58:21
*/
#include "../inc/bluetooth_global.h"

#include "module/GPS/inc/GPS_global.h"

struct Bluetooth bluetooth = 
{
	8,
	init,
	send,
	send_head
};