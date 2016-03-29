/* 
* @Author: obsidian
* @Date:   2016-01-03 17:17:04
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-30 09:14:53
*/
#ifndef BLUETOOTH_STRUCT_H

struct Bluetooth
{
	unsigned short	datalenth;	
	void			(*bluetooth_init)();
	void			(*send)(unsigned char * pdata,unsigned char len);
	void			(*send_head)();
    unsigned char   status;
};

#define BLUETOOTH_STRUCT_H
#endif
