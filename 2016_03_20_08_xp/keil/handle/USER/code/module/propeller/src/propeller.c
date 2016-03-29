/* 
* @Author: obsidian
* @Date:   2015-12-15 19:40:06
* @Last Modified by:   obsidian
* @Last Modified time: 2015-12-16 22:43:32
*/
#include "../inc/propeller.h"
#include "../inc/propeller_global.h"
void propeller_set_angle(float angle)
{
	if(angle > 0 && angle <180)
	{
		*(propeller.CCRx) = 200 + 800 * angle / 180;				//PWM4 
	}
}