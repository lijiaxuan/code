/* 
* @Author: obsidian (xjtu)
* @Date:   2015-12-07 10:27:09
* @Last Modified by:   obsidian
* @Last Modified time: 2015-12-07 15:43:08
*/
#include "../inc/handle_control.h"
void m_set_ducted_fan_angle(unsigned short *TIMx_CCRx,int angle);
void m_set_steering_angle(unsigned short *TIMx_CCRx,int angle);

void m_handle_control_init(struct Handle_Control* phandle_control)
{
	phandle->m_init(phandle);
};

void m_handle_control(struct Handle_Control* phandle_control)
{
	signed short fan_delay_time = 0;
	struct Handle *phandle = phandle_control->phandle;
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
		phandle_control->delay_flag =1;
		phandle_control->ducted_fan_pwm++;
		m_set_ducted_fan_angle(phandle->ducted_fan_ccrx,phandle_control->ducted_fan_pwm);				
	}
	if(phandle->ducted_fan_control_data<(0x3000-0x400))
	{
		phandle_control->delay_flag =1;
		phandle_control->ducted_fan_pwm--;
		m_set_ducted_fan_angle(phandle->steering_angle_ccrx,phandle_control->ducted_fan_pwm);
	}
				 
				  //下面这一部分是用来控制前轮的  			
	if(phandle->steering_angle_control_data-0x3000>0x40)
	{
		phandle_control->steering_pwm_handle=90+90*(phandle->steering_angle_control_data-0x3040)/0x1000;
	}
	else if(phandle->steering_angle_control_data-0x3000<-0x40)
	{
		phandle_control->steering_pwm_handle=90+90*(phandle->steering_angle_control_data-0x3000+0x40)/0x1000;
	}
	else
	{
		phandle_control->steering_pwm_handle=90;
	} 
	if(phandle_control->steering_pwm<phandle_control->steering_pwm_handle)
	{
		phandle_control->steering_pwm++;
		m_set_steering_angle(phandle->steering_angle_ccrx,phandle_control->steering_pwm);
	}
	if(phandle_control->steering_pwm>phandle_control->steering_pwm_handle)
	{
		phandle_control->steering_pwm--;
		m_set_steering_angle(phandle->steering_angle_ccrx,phandle_control->steering_pwm);	
	}
	if(phandle_control->delay_flag==1)//判断帆是否转动
	{
		delay_ms(fan_delay_time);
	}
	else
	{
		delay_ms(10);
	}
	phandle_control->delay_flag=0;
}

void m_set_ducted_fan_angle(unsigned short *TIMx_CCRx,int angle)
{
	if(angle > 0 && angle <180)
	{
		*TIMx_CCRx = 200 + 800 * angle / 180;				//PWM4 
	}
}
void m_set_steering_angle(unsigned short *TIMx_CCRx,int angle)
{
	if(angle > 0 && angle < 180)
	{
		*TIMx_CCRx = 200 + 800 * angle / 180;				//PWM3  
	}
}
