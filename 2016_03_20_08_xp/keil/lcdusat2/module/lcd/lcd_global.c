/*
* @Author: obsidian
* @Date:   2016-03-01 10:32:33
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-12 20:32:25
*/
#include "lcd_global.h"
#include "lcd.h"
#include "menu/menu_pid.h"
#include "menu/menu_brake.h"
#include "menu/menu_steer.h"
#include "menu/menu_gps.h"
#include "menu/menu_camera.h"
#include "menu/menu_run.h"
#include "menu/menu_gpspath.h"
#include "menu/menu_select.h"


struct Item root_i[] = 
{
    {"gps",     1,  3,  FOLDER,     0,      0,      0,      0,      menu_gps_enter, menu_nop,   menu_nop,   menu_nop},
    {"pid",     2,  3,    FOLDER,     0,      0,      0,      0,      menu_pid_enter, menu_nop,   menu_nop,   menu_nop},
    {"steer",   3,  5,  FOLDER,     0,      0,      0,      0,      menu_steer_enter, menu_nop,   menu_nop,   menu_nop},
    {"brake",   4,  5,  FOLDER,     0,      0,      0,      0,      menu_brake_enter, menu_nop,   menu_nop,   menu_nop},
    {"run",     5,  3,  FOLDER,     0,      0,      0,      0,      menu_run_enter, menu_nop,   menu_nop,   menu_run_back},
    {"gpspath", 6,  7,  FOLDER,     0,      0,      0,      0,      menu_nop, menu_nop,   menu_nop,   menu_nop},
    {"select",  7,  7,  FOLDER,     0,      0,      0,      0,      menu_nop, menu_nop,   menu_nop,   menu_nop},
    // {"",  5,  FOLDER,    0,  {0, 0}, menu_gps_enter, menu_nop,   menu_nop,   menu_nop},
    // {"gps",  6,  FOLDER,    0,  {0, 0}, menu_gps_enter, menu_nop,   menu_nop,   menu_nop},
    // {"gps",  7,  FOLDER,    0,  {0, 0}, menu_gps_enter, menu_nop,   menu_nop,   menu_nop},

};

struct Node root_n = 
{
    sizeof(root_i)/sizeof(root_i[0]),
    sizeof(root_i)/sizeof(root_i[0]) / PAGE_SIZE,
    0,
    0,
    root_i,
    {0, 0},
    &root_n,
    {
        0,
        &gps_n,
        &pid_n,
        &steer_n,
        &brake_n,
        &run_n,
		&gpspath_n,
        &select_n,
    },
    root_init
};

struct Status status =
{
    &root_n,
    root_i,//root_n.p_item,
    0,
};
struct Lcd lcd = 
{
    &root_n,
    &status,
    0xff,       //key
    0,          //update_flag
    lcd_init,
    key_process,
    input,
};
