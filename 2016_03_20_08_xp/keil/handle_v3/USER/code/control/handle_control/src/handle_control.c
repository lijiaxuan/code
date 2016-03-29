/* 
* @Author: obsidian (xjtu)
* @Date:   2015-12-07 10:27:09
* @Last Modified by:   obsidian
* @Last Modified time: 2015-12-15 20:36:42
*/
#include "../inc/handle_control.h"
#include "module/steer/inc/steer_global.h"
#include "module/propeller/inc/propeller_global.h"
#include "../inc/handle_control_global.h"
void m_handle_control_init()
{
	handle_control.phandle->m_init(handle_control.phandle);
};

void m_handle_control(struct Handle_Control* phandle_control)
{
	signed short fan_delay_time = 0;
	struct Handle *phandle = handle_control.phandle;
	fan_delay_time=10 * (phandle->ducted_fan_control_data -0x3000 ) / 0x0800 + 5;//通过改变前面的系数来控制舵机变化速度的大小
	if(fan_delay_time < 0)
	{
		fan_delay_time = -1 * fan_delay_time;
	}
	fan_delay_time=20-fan_delay_time;
	if(phandle->ducted_fan_control_data == 0)
	{
		fan_delay_time = 20;
	}
	if(phandle->ducted_fan_control_data > (0x3000+0x400))
	{
		handle_control.delay_flag =1;
		handle_control.ducted_fan_pwm++;
		propeller.set_angle(handle_control.ducted_fan_pwm);				
	}
	if(phandle->ducted_fan_control_data<(0x3000-0x400))
	{
		handle_control.delay_flag =1;
		handle_control.ducted_fan_pwm--;
		propeller.set_angle(handle_control.ducted_fan_pwm);
	}
				 
				  //下面这一部分是用来控制前轮的  			
	if(phandle->steering_angle_control_data-0x3000>0x40)
	{
		handle_control.steering_pwm_handle=90+90*(phandle->steering_angle_control_data-0x3040)/0x1000;
	}
	else if(phandle->steering_angle_control_data-0x3000<-0x40)
	{
		handle_control.steering_pwm_handle=90+90*(phandle->steering_angle_control_data-0x3000+0x40)/0x1000;
	}
	else
	{
		handle_control.steering_pwm_handle=90;
	} 
	if(handle_control.steering_pwm<handle_control.steering_pwm_handle)
	{
		handle_control.steering_pwm++;
		steer.set_angle(handle_control.steering_pwm);
	}
	if(handle_control.steering_pwm>handle_control.steering_pwm_handle)
	{
		handle_control.steering_pwm--;
		steer.set_angle(handle_control.steering_pwm);	
	}
	if(handle_control.delay_flag==1)//判断帆是否转动
	{
		delay_ms(fan_delay_time);
	}
	else
	{
		delay_ms(10);
	}
	handle_control.delay_flag=0;
}