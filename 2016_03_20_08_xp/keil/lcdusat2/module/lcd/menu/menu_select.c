/*
* @Author: obsidian
* @Date:   2016-03-01 10:39:43
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-12 22:06:41
*/

#include "menu_select.h"
#include "../lcd_global.h"
#include "../lcd.h"
#include "control/car.h"

static void menu_select_init(void *parg);

static void menu_select_exec(void *parg)
{
    lcd.input();
}

struct Item select_i[] = 
{
    //ctx               id  flag   isint color int float pos
    {"car_id",          1,  5,    FILE,   1,   0,  (unsigned short *)&car.version,       0,   menu_nop, menu_select_exec,  menu_nop, menu_nop,},
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
