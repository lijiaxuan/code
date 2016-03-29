/* 
* @Author: obsidian (xjtu)
* @Date:   2015-12-07 08:35:33
* @Last Modified by:   obsidian
* @Last Modified time: 2015-12-15 20:39:15
*/
#include "../inc/handle_global.h"
struct Handle handle = 
{
	USART1,								//USARTx
	0,									//ducted_fan_control_data
	0,									//steering_control_data
	m_init,
	uart_init,
};