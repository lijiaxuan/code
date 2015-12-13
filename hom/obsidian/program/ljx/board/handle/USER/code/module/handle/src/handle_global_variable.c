/* 
* @Author: obsidian (xjtu)
* @Date:   2015-12-07 08:35:33
* @Last Modified by:   obsidian
* @Last Modified time: 2015-12-07 15:38:52
*/
#include "../inc/handle_global_variable.h"
struct Handle handle = 
{
	USART1,								//USARTx
	(unsigned short *)&(TIM2->CCR3),	//ducted_fan_ccrx
	(unsigned short *)&(TIM2->CCR4),	//steering_angle_ccrx
	0,									//ducted_fan_control_data
	0,									//steering_control_data
	m_init,
	uart_init,
	TIM2_Mode_Config,
};