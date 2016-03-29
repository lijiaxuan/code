/* 
* @Author: obsidian
* @Date:   2015-12-15 19:59:08
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-24 20:24:21
*/
#include "../inc/steer.h"
#include "../inc/steer_global.h"
void steer_set_angle(float angle)
{
	steer.steer_angle = angle;
	unsigned short pwm = 0;
	pwm = 1400 + (int)(5 * angle);
	if(pwm > 1600)
		{pwm = 1600;}
	else if(pwm < 1200)
		{pwm = 1200;}
	*steer.CCRx = pwm;				//PWM4 
}
