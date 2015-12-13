/* 
* @Author: obsidian (xjtu)
* @Date:   2015-12-07 08:34:55
* @Last Modified by:   obsidian
* @Last Modified time: 2015-12-07 10:33:58
*/
#include "../inc/handle.h"

// void m_set_ducted_fan_angle(unsigned short *TIMx_CCRx,float angle);
// void m_set_steering_angle(unsigned short *TIMx_CCRx,float angle);

void m_init(struct Handle* phandle)
{
	phandle->usart_init(115200);
	phandle->PWM_Config(3,3);
}