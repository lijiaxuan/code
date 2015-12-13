/* 
* @Author: obsidian (xjtu)
* @Date:   2015-12-07 10:26:51
* @Last Modified by:   obsidian
* @Last Modified time: 2015-12-07 15:43:58
*/
#ifndef HANDLE_CONTROL_STRUCT_H

#include "module/handle/inc/handle_struct.h"

struct Handle_Control
{
	struct Handle  *phandle;
	unsigned char  delay_flag;
	unsigned short ducted_fan_pwm;
	unsigned short steering_pwm;
	unsigned short steering_pwm_handle;
	void           (*m_handle_control)(struct Handle_Control *phandle_control);
	void           (*m_handle_control_init)(struct Handle_Control* phandle_control);
};

#define HANDLE_CONTROL_STRUCT_H
#endif
