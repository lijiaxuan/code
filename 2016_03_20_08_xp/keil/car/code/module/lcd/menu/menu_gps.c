/*
* @Author: obsidian
* @Date:   2016-03-01 10:18:42
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-12 10:53:52
*/

#include "menu_gps.h"
#include "../lcd_global.h"
#include "module/GPS/GPS_global.h"
#include "control/GPSpath/GPSpath_global.h"

static void menu_set_gyro_convert1_exec(void *parg);
static void menu_set_gyro_convert1_input(void *parg);
static void menu_set_gyro_convert1_back(void *parg);

static void menu_set_gyro_convert2_exec(void *parg);
static void menu_set_gyro_convert2_input(void *parg);
static void menu_set_gyro_convert2_back(void *parg);

static void menu_set_encoderl_eff_exec(void *parg);
static void menu_set_encoderl_eff_input(void *parg);
static void menu_set_encoderl_eff_back(void *parg);

static void menu_set_encoderr_eff_exec(void *parg);
static void menu_set_encoderr_eff_input(void *parg);
static void menu_set_encoderr_eff_back(void *parg);

static void menu_set_x_exec(void *parg);
static void menu_set_x_input(void *parg);
static void menu_set_x_back(void *parg);

static void menu_set_y_exec(void *parg);
static void menu_set_y_input(void *parg);
static void menu_set_y_back(void *parg);

static void menu_set_angle_exec(void *parg);
static void menu_set_angle_input(void *parg);
static void menu_set_angle_back(void *parg);

static void menu_set_direction_exec(void *parg);
static void menu_set_direction_input(void *parg);
static void menu_set_direction_back(void *parg);

void  menu_gps_init(void *parg);
struct Item gps_i[] = 
{
    {"conv1",       1,  5,  FILE, 0,    0,    0,            &gyro.gyro_convert1, menu_nop,  menu_set_gyro_convert1_exec,   menu_set_gyro_convert1_input,  menu_set_gyro_convert1_back},
    {"conv2",       2,  5,  FILE, 0,    0,    0,            &gyro.gyro_convert2,  menu_nop,  menu_set_gyro_convert2_exec,   menu_set_gyro_convert2_input,  menu_set_gyro_convert2_back},
    {"effl",        3,  4,  FILE, 0,    0,    0,            &encoder_property[0].convert,  menu_nop,  menu_set_encoderl_eff_exec,   menu_set_encoderl_eff_input,  menu_set_encoderl_eff_back},
    {"effr",        4,  4,  FILE, 0,    0,    0,            &encoder_property[1].convert,  menu_nop,  menu_set_encoderr_eff_exec,   menu_set_encoderr_eff_input,  menu_set_encoderr_eff_back},
    {"start_x",     5,  7,  FILE, 0,    0,    0, &gps.start_point.x,   menu_nop,  menu_set_x_exec,   menu_set_x_input,  menu_set_x_back},
    {"start_y",     6,  7,  FILE, 0,    0,    0,  &gps.start_point.y,      menu_nop,  menu_set_y_exec,   menu_set_y_input,  menu_set_y_back},
    {"start_ang",   7,  9,  FILE, 0,    0,    0,            &gps.start_angle,  menu_nop,  menu_set_angle_exec,   menu_set_angle_input,  menu_set_angle_back},
    {"start_index", 8,  11,FILE, 0,    0,    0,            &gps.start_angle,  menu_nop,  menu_set_angle_exec,   menu_set_angle_input,  menu_set_angle_back},
    {"direc",       9,  5,  FILE, 1,    0,    (short*)&gps.direction,  0,      menu_nop,  menu_set_direction_exec,   menu_set_direction_input,  menu_set_direction_back},
    {"status",       10,  6,  FILE, 1,    0,    (short*)&gps.status,  0,      menu_nop,  menu_set_direction_exec,   menu_set_direction_input,  menu_set_direction_back},

};
struct Node gps_n = 
{
    sizeof(gps_i) / sizeof(gps_i[0]),
    sizeof(gps_i) / sizeof(gps_i[0]) / PAGE_SIZE,
    0,  //page_index
    1,  //node_id
    gps_i,
    {0, 0},
    &root_n,
    {0},
    menu_gps_init,
};
void  menu_gps_init(void *parg)
{
    gps_n.save = gps.save;
}

void menu_set_gyro_convert1_exec(void *parg)
{
    lcd.input();
}
void menu_set_gyro_convert1_input(void *parg)
{}
void menu_set_gyro_convert1_back(void *parg)
{}
void menu_set_gyro_convert2_exec(void *parg)
{
    lcd.input();
}
void menu_set_gyro_convert2_input(void *parg)
{}
void menu_set_gyro_convert2_back(void *parg)
{}
void menu_set_encoderl_eff_exec(void *parg)
{
    lcd.input();
}
void menu_set_encoderl_eff_input(void *parg)
{}
void menu_set_encoderl_eff_back(void *parg)
{}
void menu_set_encoderr_eff_exec(void *parg)
{
    lcd.input();
}
void menu_set_encoderr_eff_input(void *parg)
{}
void menu_set_encoderr_eff_back(void *parg)
{}
void menu_set_x_exec(void *parg)
{
    lcd.input();
}
void menu_set_x_input(void *parg)
{}
void menu_set_x_back(void *parg)
{}
void menu_set_y_exec(void *parg)
{
    lcd.input();
}
void menu_set_y_input(void *parg)
{}
void menu_set_y_back(void *parg)
{}
void menu_set_angle_exec(void *parg)
{
    lcd.input();
}
void menu_set_angle_input(void *parg)
{}
void menu_set_angle_back(void *parg)
{}
void menu_set_direction_exec(void *parg)
{
    lcd.input();
}
void menu_set_direction_input(void *parg)
{}
void menu_set_direction_back(void *parg)
{}
