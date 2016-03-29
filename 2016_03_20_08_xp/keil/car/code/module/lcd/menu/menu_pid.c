/*
* @Author: obsidian
* @Date:   2016-03-01 10:22:51
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-17 14:34:55
*/

#include "menu_pid.h"
#include "../lcd_global.h"
#include "module/pid/pid_global.h"

void menu_set_pid0_10_exec(void *parg);
void menu_set_pid0_10_input(void *parg);
void menu_set_pid11_13_exec(void *parg);
void menu_set_pid11_13_input(void *parg);

void menu_set_pid14_17_exec(void *parg);
void menu_set_pid14_17_input(void *parg);
void menu_set_pid18_25_exec(void *parg);
void menu_set_pid18_25_input(void *parg);

void menu_pid_init(void *parg);
struct Item pid_i[] = 
{
    {"pid0-10",  1,  6,  FILE,   0,  0,  0,  0,  menu_nop,  menu_set_pid0_10_exec,   menu_set_pid0_10_input,    menu_nop},
    {"pid11-13", 2,  8,  FILE,   0,  0,  0,  0,  menu_nop,  menu_set_pid11_13_exec,   menu_set_pid11_13_input,  menu_nop},
    {"pid13-17", 3,  8,  FILE,   0,  0,  0,  0,  menu_nop,  menu_set_pid14_17_exec,   menu_set_pid14_17_input,     menu_nop},
    {"pid18-25", 4,  8,  FILE,   0,  0,  0,  0,  menu_nop,  menu_set_pid18_25_exec,   menu_set_pid18_25_input,  menu_nop},
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
//     unsigned char i = 0;
    //for(i = 0;i < pid_n.itemnum; i ++)
    //{
        pid_i[0].p_float = &pid.pid_list[0].kp;
        pid_i[1].p_float = &pid.pid_list[11].kp;
        pid_i[2].p_float = &pid.pid_list[14].kp;
        pid_i[3].p_float = &pid.pid_list[18].kp;
    //} 
     pid_n.save = pid.save;
}

void menu_set_pid0_10_exec(void *parg)
{
    lcd.input();
}
void menu_set_pid0_10_input(void *parg)
{
    unsigned char i = 0;
    for(i = 1;i <= 10;i ++)
    {
        pid.pid_list[i] = pid.pid_list[0];
    }
}

void menu_set_pid11_13_exec(void *parg)
{
    lcd.input();
}
void menu_set_pid11_13_input(void *parg)
{
    unsigned char i = 0;
    for(i = 12;i <= 13;i ++)
    {
        pid.pid_list[i] = pid.pid_list[11];
    }
}



void menu_set_pid14_17_exec(void *parg)
{
    lcd.input();
}
void menu_set_pid14_17_input(void *parg)
{
    unsigned char i = 0;
    for(i = 15;i <= 17;i ++)
    {
        pid.pid_list[i] = pid.pid_list[14];
    }
}
void menu_set_pid18_25_exec(void *parg)
{
    lcd.input();
}
void menu_set_pid18_25_input(void *parg)
{
    unsigned char i = 0;
    for(i = 19;i < 25;i ++)
    {
        pid.pid_list[i] = pid.pid_list[18];
    }
}
