/*
* @Author: obsidian
* @Date:   2016-03-01 10:39:43
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-16 16:36:20
*/

#include "menu_select.h"
#include "../lcd_global.h"
#include "../lcd.h"
#include "control/car.h"
#include "module/pid/pid_global.h"
#include "module/GPS/GPS_global.h"
#include "module/steer/steer_global.h"
#include "module/brake/brake_global.h"
#include "control/GPSpath/GPSpath_global.h"
#include "driver/sd/file.h"


static void menu_select_init(void *parg);

static void menu_select_exec(void *parg)
{
    lcd.input();
}

static void menu_select_input(void *parg)
{
    car.save();
    steer.init();
    //gps.init();
    gpspath.init();
    brake.init();
    pid.init();
}

void menu_clear_exec(void* parg)
{
    car.strjoint("pid");
    f_unlink((const TCHAR *)car.path);
    car.strjoint("gps");
    f_unlink(car.path);
    car.strjoint("encoder");
    f_unlink(car.path);
    car.strjoint("gyro");
    f_unlink(car.path);
    car.strjoint("steer");
    f_unlink(car.path);
    car.strjoint("brake");
    f_unlink(car.path);
}

struct Item select_i[] = 
{
    //ctx               id  flag   isint color int float pos
    {"car_id",          1,  5,    FILE,   1,   0,  (unsigned short *)&car.version,       0,   menu_nop, menu_select_exec,  menu_select_input, menu_nop,},
    {"clear",          2,  5,    FILE,   1,   0,  (unsigned short *)&car.version,       0,   menu_nop, menu_clear_exec,  menu_select_input, menu_nop,},
    
    //{"setselect",  0,  0,  {0, 0},  menu_set_select,},
};

struct Node select_n = 
{
    sizeof(select_i) / sizeof(select_i[0]),
    (unsigned short)sizeof(select_i) / sizeof(select_i[0])/PAGE_SIZE,
    0,
    7,
    select_i,
    {0, 0},
    &root_n,
    {0},
    menu_select_init
};

static void menu_select_init(void *parg)
{
    select_n.save = car.save;
}
