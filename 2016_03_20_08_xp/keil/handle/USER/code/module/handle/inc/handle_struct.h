/* 
* @Author: obsidian (xjtu)
* @Date:   2015-12-07 08:35:06
* @Last Modified by:   obsidian
* @Last Modified time: 2015-12-20 09:28:29
*/
#ifndef HANDLE_STRUCT_H

#include "stm32f10x_usart.h"
#define PI 3.1415926
#include "module/GPS/inc/GPS_struct.h"


// struct Point
// {
// 	float x;
// 	float y;
// };
struct Handle
{
	USART_TypeDef  *USARTx;
	
	unsigned short ducted_fan_control_data;
	unsigned short steering_angle_control_data;

	void 		   (*m_init)();
	void		   (*usart_init)(int baudrate);
};


#define HANDLE_STRUCT_H
#endif
