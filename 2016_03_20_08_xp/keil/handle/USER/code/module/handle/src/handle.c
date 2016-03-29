/* 
* @Author: obsidian (xjtu)
* @Date:   2015-12-07 08:34:55
* @Last Modified by:   obsidian
* @Last Modified time: 2015-12-15 20:38:54
*/
#include "../inc/handle.h"
#include "../inc/handle_global.h"
// void m_set_ducted_fan_angle(unsigned short *TIMx_CCRx,float angle);
// void m_set_steering_angle(unsigned short *TIMx_CCRx,float angle);

void m_init()
{
	handle.usart_init(115200);
}