/* 
* @Author: obsidian
* @Date:   2016-01-03 17:16:43
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-30 09:16:00
*/
#include "../inc/bluetooth_global.h"

struct Bluetooth bluetooth = 
{
	8,
	init,
	send,
	send_head,
    0   //status
};
