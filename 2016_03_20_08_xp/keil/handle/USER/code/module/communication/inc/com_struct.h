/* 
* @Author: obsidian
* @Date:   2015-12-13 21:32:00
* @Last Modified by:   obsidian
* @Last Modified time: 2015-12-13 21:51:49
*/
#ifndef COMMUNICATE_STRUCT_H

struct Message
{
	unsigned char 	status;
	float			angle;
	float			distance_to_center;
};

struct Communication
{
	struct Message 	*pmessage;
	void			(*com_init)();
	void 			(*fun2)();
	void			(*fun3)();
};

#define COMMUNICATE_STRUCT_H 
#endif