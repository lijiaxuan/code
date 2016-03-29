/*
* @Author: obsidian
* @Date:   2016-03-01 10:22:51
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-13 09:21:34
*/

#include "menu_pid.h"
#include "../lcd_global.h"
#include "module/pid/pid_global.h"


void menu_get_pid_exec(void *parg);
void menu_get_pid_input(void *parg);
void menu_get_pid_back(void *parg);

void menu_set_pid_exec(void *parg);
void menu_set_pid_input(void *parg);
void menu_set_pid_back(void *parg);
void menu_pid_init(void *parg);
struct Item pid_i[] = 
{
    {"pid0-9",   1,  6,  FILE,   0,  0,  0,  0,  menu_nop,  menu_set_pid_exec,   menu_set_pid_input,  menu_set_pid_back},
    {"pid11-25", 2, 7,  FILE,   0,  0,  0,  0,  menu_nop,  menu_set_pid_exec,   menu_set_pid_input,  menu_set_pid_back},
};

struct Node pid_n = 
{
    sizeof(pid_i) / sizeof(pid_i[0]),
    sizeof(pid_i) / sizeof(pid_i[0]) / PAGE_SIZE,
    0,  //page_index
    2,  //node_id
    pid_i,
    {0, 0},
    &root_n,
    {0},
    menu_pid_init,
};

void menu_pid_init(void *parg)
{
    unsigned char i = 0;
    //for(i = 0;i < pid_n.itemnum; i ++)
    //{
        pid_i[0].p_float = &pid.pid_list[0].kp;
        pid_i[1].p_float = &pid.pid_list[10].kp;
    //} 
//     pid_n.save = pid.save;
}

void menu_get_pid_exec(void *parg)
{

}
void menu_get_pid_input(void *parg)
{

}
void menu_get_pid_back(void *parg)
{

}
void menu_set_pid_exec(void *parg)
{
    lcd.input();
}
void menu_set_pid_input(void *parg)
{

}
void menu_set_pid_back(void *parg)
{

}
