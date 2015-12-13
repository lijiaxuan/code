/* 
* @Author: obsidian (xjtu)
* @Date:   2015-12-07 15:44:35
* @Last Modified by:   obsidian
* @Last Modified time: 2015-12-07 22:40:48
*/

#include "control/handle_control/inc/handle_control_global_variable.h"
#include "driver/delay/inc/delay.h"

int main(int argc, char const *argv[])
{
	/* code */
	SYSTICK_Init();
	handle_control.m_handle_control_init(&handle_control);
	while(1)
	{
		handle_control.m_handle_control(&handle_control);
	}

	return 0;
}
