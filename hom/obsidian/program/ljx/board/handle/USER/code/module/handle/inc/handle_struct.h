/* 
* @Author: obsidian (xjtu)
* @Date:   2015-12-07 08:35:06
* @Last Modified by:   obsidian
* @Last Modified time: 2015-12-07 15:37:32
*/
#ifndef HANDLE_STRUCT_H

#include "stm32f10x_usart.h"
#define PI 3.1415926

struct Point
{
	float x;
	float y;
};
struct Handle
{
	USART_TypeDef  *USARTx;
	unsigned short *ducted_fan_ccrx;
	unsigned short *steering_angle_ccrx;

	unsigned short ducted_fan_control_data;
	unsigned short steering_angle_control_data;

	void 		   (*m_init)(struct Handle*);
	void		   (*usart_init)(int baudrate);
	void 		   (*PWM_Config)(int t1,int t2);
};


#define HANDLE_STRUCT_H
#endif