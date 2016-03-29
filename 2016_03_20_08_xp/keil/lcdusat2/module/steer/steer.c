/* 
* @Author: obsidian
* @Date:   2015-12-15 19:59:08
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-10 20:20:07
*/

#include "steer.h"
#include "steer_global.h"
#include "driver/sd/file.h"
#include "steer_driver.h"

void steer_set_angle(float angle)
{
	if(steer.status == OFF) {return;}

	unsigned short pwm = 0;
	
	pwm = steer.midvalue + (unsigned short)(steer.ratio * angle);
	if(pwm > steer.midvalue + steer.limit[0]) 
	{pwm = steer.midvalue + steer.limit[0];}
	else if(pwm < steer.midvalue - steer.limit[1]) 
	{pwm = steer.midvalue - steer.limit[1];}
	*steer.CCRx = pwm;
}

struct Steer dup_steer;

void steer_init()
{
    unsigned int k = 0;
   
    f_open(&fil,"steer",FA_READ);
    if(fil.fs == 0){return;}
    f_read(&fil, (char *)&dup_steer, sizeof(struct Steer), &k);
    
    steer.midvalue = dup_steer.midvalue;
    steer.limit[0] = dup_steer.limit[0];
    steer.limit[1] = dup_steer.limit[1];
    file_close();
     TIM4_Mode_Config();
}
void steer_save()
{
	unsigned int k = 0;
	file_open("steer");
    if(fil.fs == 0){return;}
    f_write(&fil, (char *)&steer, sizeof(struct Steer), &k);
    file_close();
}
