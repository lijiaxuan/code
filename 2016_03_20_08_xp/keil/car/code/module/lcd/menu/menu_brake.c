/*
* @Author: obsidian
* @Date:   2016-03-01 10:49:08
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-15 09:28:35
*/

#include "menu_brake.h"
#include "../lcd_global.h"
#include "../lcd.h"
#include "module/brake/brake_global.h"
static void menu_set_ccr_exec(void* parg);
static void menu_set_ccr_input(void* parg);
static void menu_set_ccr_back(void* parg);

static void menu_set_midvalue_exec(void* parg);
static void menu_set_midvalue_input(void* parg);
static void menu_set_midvalue_back(void* parg);

static void menu_set_upperlimit_exec(void* parg);
static void menu_set_upperlimit_input(void* parg);
static void menu_set_upperlimit_back(void* parg);

static void menu_set_lowerlimit_exec(void* parg);
static void menu_set_lowerlimit_input(void* parg);
static void menu_set_lowerlimit_back(void* parg);
static void menu_brake_init(void *parg);

struct Item brake_i[] = 
{
    //ctx               id  flag   isint color int float pos
    {"ccr:",            1,  4,    FILE,   1,   0,  0,       0,   menu_nop, menu_set_ccr_exec,  menu_set_ccr_input,menu_set_ccr_back},
    {"midvalue",        2,  9,    FILE,   1,   0,  &brake.midvalue,  0,   menu_nop, menu_set_midvalue_exec,  menu_set_midvalue_input,menu_set_midvalue_back},
    {"brakelimit",      3,  9,    FILE,   1,   0,  (unsigned short *)&brake.limit[0],  0,   menu_nop, menu_set_upperlimit_exec,  menu_set_upperlimit_input,menu_set_upperlimit_back},
    {"status",          4,  6,    FILE,   1,   0,  (unsigned short *)&brake.status,  0,   menu_nop, menu_set_lowerlimit_exec,  menu_set_lowerlimit_input,menu_set_lowerlimit_back},
    
    //{"setbrake",  0,  0,  {0, 0},  menu_set_brake,},
};

struct Node brake_n = 
{
    sizeof(brake_i) / sizeof(brake_i[0]),
    (unsigned short)sizeof(brake_i) / sizeof(brake_i[0])/PAGE_SIZE,
    0,
    1,
    brake_i,
    {0, 0},
    &root_n,
    {0},
    menu_brake_init
};

static void menu_brake_init(void *parg)
{
    brake_n.p_item[0].p_short = &TIM4->CCR3;
    brake_n.save = brake.save;
}

static void menu_set_ccr_exec(void* parg)
{
    input();
}
static void menu_set_ccr_input(void* parg)
{}
static void menu_set_ccr_back(void* parg)
{}
static void menu_set_midvalue_exec(void* parg)
{
    input();
}
static void menu_set_midvalue_input(void* parg)
{}
static void menu_set_midvalue_back(void* parg)
{}
static void menu_set_upperlimit_exec(void* parg)
{
    input();
}
static void menu_set_upperlimit_input(void* parg)
{}
static void menu_set_upperlimit_back(void* parg)
{}
static void menu_set_lowerlimit_exec(void* parg)
{
    input();
}
static void menu_set_lowerlimit_input(void* parg)
{}
static void menu_set_lowerlimit_back(void* parg)
{}
