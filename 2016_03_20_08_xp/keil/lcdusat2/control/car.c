/*
* @Author: obsidian
* @Date:   2016-03-01 11:08:57
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-12 20:31:16
*/
#include "driver/math/inc/math.h"
#include "driver/sd/file.h"
#include "car.h"
void strjoint(unsigned char *filename);
void car_init();
void car_save();
struct Car car = 
{
    3,
    "config1/",
    strjoint,
    car_init,    
    car_save,        
};

void strjoint(unsigned char *filename)
{
    unsigned char i = 0;
    zero((char *)&car.path[8], 12);
    car.path[6] = car.version + '0';
    for(i = 8;i < 20 && *filename != 0; i ++, filename ++)
    {
        car.path[i] = *filename;
    }
}

static struct Car dup_car;

void car_init()
{
    unsigned int k = 0;
    f_open(&fil,"car",FA_READ);
    if(fil.fs == 0){return;}
    f_read(&fil, (char *)&dup_car, sizeof(struct Car), &k);
    car.version = dup_car.version;
    file_close();
}

void car_save()
{
    unsigned int k = 0;
    f_open(&fil,"car",FA_WRITE | FA_CREATE_ALWAYS);
    if(fil.fs == 0){return;}
    f_write(&fil, (char *)&car, sizeof(struct Car), &k);
    file_close();
}
