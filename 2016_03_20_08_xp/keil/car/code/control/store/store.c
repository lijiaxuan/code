/*
* @Author: obsidian
* @Date:   2016-03-16 17:48:00
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-19 08:54:35
*/
#include "store.h"
#include "store_global.h"

void str_init()
{
    f_open(&store.str_fl, "path", FA_CREATE_ALWAYS|FA_WRITE);
}

void str_save()
{
    if(store.str_fl.fs == 0){return;}
    f_printf(&store.str_fl, "{%d,%d,%d,%d},\n", (int)(gps.front_point.x * 10),(int)(gps.front_point.y * 10), (int)(gps.angle * 10),(int)(gps.speed * 10));
}
void str_close()
{
    if(store.str_fl.fs == 0){return;}
    f_close(&store.str_fl);
}
