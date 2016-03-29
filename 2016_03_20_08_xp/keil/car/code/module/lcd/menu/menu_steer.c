/*
* @Author: obsidian
* @Date:   2016-03-01 10:49:08
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-15 09:28:08
*/

#include "menu_steer.h"
#include "../lcd_global.h"
#include "../lcd.h"
#include "module/steer/steer_global.h"
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

static void menu_steer_init(void *parg);

struct Item steer_i[] = 
{
    //ctx               id  flag   isint color int float pos
    {"ccr:",            1,  4,    FILE,   1,   0,  0,       0,   menu_nop, menu_set_ccr_exec,  menu_set_ccr_input,menu_set_ccr_back},
    {"midvalue",        2,  8,    FILE,   1,   0,  &steer.midvalue,  0,   menu_nop, menu_set_midvalue_exec,  menu_set_midvalue_input,menu_set_midvalue_back},
    {"upperlimit",      3,  10,    FILE,   1,   0,  &steer.limit[0],  0,   menu_nop, menu_set_upperlimit_exec,  menu_set_upperlimit_input,menu_set_upperlimit_back},
    {"lowerlimit",      4,  10,    FILE,   1,   0,  &steer.limit[1],  0,   menu_nop, menu_set_lowerlimit_exec,  menu_set_lowerlimit_input,menu_set_lowerlimit_back},
    {"steer_k",         5,  7,    FILE,   0,   0,  0,  &steer.ratio,   menu_nop, menu_set_lowerlimit_exec,  menu_set_lowerlimit_input,menu_set_lowerlimit_back},
    {"status",         6,  6,    FILE,   1,   0,  (unsigned short *)&steer.status, 0,   menu_nop, menu_set_lowerlimit_exec,  menu_set_lowerlimit_input,menu_set_lowerlimit_back},
     
    //{"setsteer",  0,  0,  {0, 0},  menu_set_steer,},
};

struct Node steer_n = 
{
    sizeof(steer_i) / sizeof(steer_i[0]),
    (unsigned short)sizeof(steer_i) / sizeof(steer_i[0])/PAGE_SIZE,
    0,  //page_index
    3,  //node_id
    steer_i,
    {0, 0},
    &root_n,
    {0},
    menu_steer_init,
};
static void menu_steer_init(void *parg)
{
    steer_n.p_item[0].p_short = &TIM4->CCR4;
    steer_n.save = steer.save;
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
