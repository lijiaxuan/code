/* 
* @Author: obsidian
* @Date:   2015-12-15 19:59:08
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-17 09:04:13
*/

#include "steer.h"
#include "steer_global.h"
#include "driver/sd/file.h"
#include "steer_driver.h"
#include "control/car.h"

void steer_set_angle(float angle)
{
	if(steer.status == OFF) {return;}
    steer.angle = angle;
	unsigned short pwm = 0;
	
	pwm = steer.midvalue + (short)(steer.ratio * angle);
	if(pwm > steer.limit[0]) 
	{pwm = steer.limit[0];}
	else if(pwm < steer.limit[1]) 
	{pwm = steer.limit[1];}
	*steer.CCRx = pwm;
}

struct Steer dup_steer;

void steer_init()
{
    unsigned int k = 0;
	  TIM4_Mode_Config();
    car.strjoint("steer");
    f_open(&fil,car.path,FA_READ);
    if(fil.fs == 0){return;}
    f_read(&fil, (char *)&dup_steer, sizeof(struct Steer), &k);
    
    steer.midvalue = dup_steer.midvalue;
    steer.limit[0] = dup_steer.limit[0];
    steer.limit[1] = dup_steer.limit[1];
    steer.ratio = dup_steer.ratio;
    file_close();

}
void steer_save()
{
	unsigned int k = 0;
    car.strjoint("steer");
	  file_open(car.path);
    if(fil.fs == 0){return;}
    f_write(&fil, (char *)&steer, sizeof(struct Steer), &k);
    file_close();
}
