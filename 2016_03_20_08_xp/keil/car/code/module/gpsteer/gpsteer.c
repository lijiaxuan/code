/* 
* @Author: obsidian
* @Date:   2015-12-15 19:59:08
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-19 20:55:48
*/

#include "gpsteer.h"
#include "gpsteer_global.h"
#include "gpsteer_driver.h"
#include "driver/sd/file.h"
#include "control/car.h"

void gpsteer_release()
{
    *gpsteer.CCRx = gpsteer.limit[0];               //PWM4 
}

struct Gpsteer  dup_gpsteer;

/*
	初始化
*/
void gpsteer_init()
{
    unsigned int k = 0;
	  //TIM43_Mode_Config();
    car.strjoint("gpsteer");
    f_open(&fil,car.path,FA_READ);
    if(fil.fs == 0){return;}
    f_read(&fil, (char *)&dup_gpsteer, sizeof(struct Gpsteer), &k);
    gpsteer.midvalue = dup_gpsteer.midvalue;
    gpsteer.limit[0] = dup_gpsteer.limit[0];
    gpsteer.limit[1] = dup_gpsteer.limit[1];
    file_close();
    
}
void gpsteer_save()
{
    unsigned int k = 0;
    car.strjoint("gpsteer");
    f_open(&fil,car.path,FA_WRITE | FA_CREATE_ALWAYS);
    if(fil.fs == 0){return;}
    f_write(&fil, (char *)&gpsteer, sizeof(struct Gpsteer), &k);
    file_close();
}
