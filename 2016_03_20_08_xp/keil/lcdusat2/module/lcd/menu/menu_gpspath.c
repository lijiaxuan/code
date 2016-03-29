/*
* @Author: obsidian
* @Date:   2016-03-10 10:17:33
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-10 14:38:55
*/
#include "menu_gpspath.h"
#include "../lcd_global.h"
#include "../lcd.h"
#include "control/GPSpath/GPSpath_global.h"
//enter  exec input back
void  menu_gpspath_init();
void  menu_gpspath_enter();


struct Item gpspath_i[] = 
{
    {"angle_k",    1,  6,  FILE, 0,    0,    0,            &gpspath.angle_k,        menu_gpspath_enter,  menu_gpspath_enter,  menu_nop,  menu_nop},
    {"distance_k", 2,  10,  FILE, 0,    0,    0,            &gpspath.distance_k,    menu_gpspath_enter,  menu_gpspath_enter,  menu_nop,  menu_nop},
    {"camera_k",   3,  7,  FILE, 0,    0,    0,            &gpspath.camera_k,       menu_gpspath_enter,  menu_gpspath_enter,  menu_nop,  menu_nop},
    {"pre",        4,  4,  FILE, 1,    0,    (unsigned short*)&gpspath.pre,  0,     menu_gpspath_enter,  menu_gpspath_enter,  menu_nop,  menu_nop},
		{"came_center", 5,  12,  FILE, 1,    0,    (unsigned short*)&gpspath.camera_center,  0,     menu_gpspath_enter,  menu_gpspath_enter,  menu_nop,  menu_nop},

};
struct Node gpspath_n = 
{
    sizeof(gpspath_i) / sizeof(gpspath_i[0]),
    sizeof(gpspath_i) / sizeof(gpspath_i[0]) / PAGE_SIZE,
    0,  //page_index
    6,  //node_id
    gpspath_i,
    {0, 0},
    &root_n,
    {0},
    menu_gpspath_init,
};
void  menu_gpspath_init()
{
    gpspath_n.save = gpspath.save;
}
void  menu_gpspath_enter()
{
    lcd.input();
}
