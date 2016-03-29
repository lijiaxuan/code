/* 
* @Author: obsidian
* @Date:   2015-12-15 19:59:08
* @Last Modified by:   obsidian
* @Last Modified time: 2015-12-31 17:13:17
*/
#include "../inc/steer.h"
#include "../inc/steer_global.h"
void steer_set_angle(float angle)
{
	steer.steer_angle = angle;
	unsigned short pwm = 0;
	float real_angle = angle + 90;
	pwm = 1830 - (int)(5 * angle);
	if(pwm > 2060)
		{pwm = 2060;}
	else if(pwm < 1600)
		{pwm = 1600;}
	*steer.CCRx = pwm;				//PWM4 
}