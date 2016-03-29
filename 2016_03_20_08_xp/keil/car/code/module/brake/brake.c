/* 
* @Author: obsidian
* @Date:   2015-12-15 19:59:08
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-12 20:42:48
*/

#include "brake.h"
#include "brake_global.h"
#include "brake_driver.h"
#include "driver/sd/file.h"
#include "control/car.h"



void brake_set_angle(float angle)
{
	if(brake.status == OFF) {return;}
	
	unsigned short pwm = 0;

	pwm = brake.midvalue + (unsigned short)(brake.ratio * angle);
	if(pwm > brake.midvalue + brake.limit[0])
	{pwm = brake.midvalue + brake.limit[0];}
	else if(pwm < brake.midvalue - brake.limit[1])
	{pwm = brake.midvalue - brake.limit[1];}
	*brake.CCRx = pwm;				//PWM4 
}

struct Brake dup_brake;

/*
	初始化
*/
void brake_init()
{
    unsigned int k = 0;
	  TIM43_Mode_Config();
    car.strjoint("brake");
    f_open(&fil,car.path,FA_READ);
    if(fil.fs == 0){return;}
    f_read(&fil, (char *)&dup_brake, sizeof(struct Brake), &k);
    brake.midvalue = dup_brake.midvalue;
    brake.limit[0] = dup_brake.limit[0];
    brake.limit[1] = dup_brake.limit[1];
    file_close();
    
}
void brake_save()
{
    unsigned int k = 0;
    car.strjoint("brake");
    f_open(&fil,car.path,FA_WRITE | FA_CREATE_ALWAYS);
    if(fil.fs == 0){return;}
    f_write(&fil, (char *)&brake, sizeof(struct Brake), &k);
    file_close();
}
