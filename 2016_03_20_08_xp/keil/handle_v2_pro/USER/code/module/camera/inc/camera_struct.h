/* 
* @Author: obsidian
* @Date:   2015-12-13 22:08:04
* @Last Modified by:   obsidian
* @Last Modified time: 2015-12-19 14:08:21
*/

#ifndef CAMERA_STRUCT_H


struct Message
{
	unsigned char 	status;
	float			angle;
	float			distance_to_center;
};

struct Camera
{
	struct Message message;
	void		   (*can_init)();
	void		   (*fun2)();
	void		   (*fun3)();
};



#define CAMERA_STRUCT_H
#endif