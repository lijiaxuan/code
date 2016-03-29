/* 
* @Author: obsidian
* @Date:   2016-01-03 17:17:04
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-15 09:08:30
*/
#ifndef BLUETOOTH_STRUCT_H

struct Bluetooth
{
	unsigned short	datalenth;
	char*			pdata;	
	void			(*bluetooth_init)(unsigned int bound);
	void			(*send)();
};

#define BLUETOOTH_STRUCT_H
#endif
