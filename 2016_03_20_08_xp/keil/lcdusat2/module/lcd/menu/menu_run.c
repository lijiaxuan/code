#include "menu_run.h"
#include "../lcd_global.h"
#include "../lcd.h"
#include "module/GPS/GPS_global.h"
#include "module/camera/camera_global.h"
#include "control/GPSpath/GPSpath_global.h"

struct Item run_i[] = 
{
    //ctx               id  flag   isint color int float pos
    {"frnt_x",       1,    6,   FILE,   0,   0,  0,        &gps.front_point.x,    menu_nop,   menu_nop,   menu_nop,   menu_nop,},
    {"frnt_y",       2,    6,   FILE,   0,   0,  0,        &gps.front_point.y,    menu_nop,   menu_nop,   menu_nop,   menu_nop,},
    {"rear_x",       3,    6,   FILE,   0,   0,  0,        &gps.rear_point.x,    menu_nop,   menu_nop,   menu_nop,   menu_nop,},
    {"rear_y",       4,    6,   FILE,   0,   0,  0,        &gps.rear_point.y,    menu_nop,   menu_nop,   menu_nop,   menu_nop,},
    {"angle",        5,    5,   FILE,   0,   0,  0,        &gps.angle,           menu_nop,   menu_nop,   menu_nop,   menu_nop,},
    {"radian",       6,    6,   FILE,   0,   0,  0,        &gps.radian,          menu_nop,   menu_nop,   menu_nop,   menu_nop,},
    {"index",        7,    5,   FILE,   1,   0,  (unsigned short *)&gpspath.current_index,    0,    menu_nop,   menu_nop,   menu_nop,   menu_nop,},
    {"came_off",     8,    8,   FILE,   1,   0,  (unsigned short *)&camera.offset,    0,   menu_nop,   menu_nop,   menu_nop,   menu_nop,},
    {"ang_err",      9,    7,   FILE,   0,   0,  0,        &gpspath.distance_error,           menu_nop,   menu_nop,   menu_nop,   menu_nop,},
    {"dis_err",      9,    7,   FILE,   0,   0,  0,        &gpspath.angle_error,          menu_nop,   menu_nop,   menu_nop,   menu_nop,},
    {"speed",       10,    6,   FILE,   0,   0,  0,        &gps.speed,          menu_nop,   menu_nop,   menu_nop,   menu_nop,},
    {"encoder1",     11,    7,   FILE,   0,   0,  0,        &encoder[0].distance,          menu_nop,   menu_nop,   menu_nop,   menu_nop,},
    {"encoder2",     12,    7,   FILE,   0,   0,  0,        &encoder[1].distance,          menu_nop,   menu_nop,   menu_nop,   menu_nop,},

};

struct Node run_n = 
{
    sizeof(run_i) / sizeof(run_i[0]),
    (unsigned short)sizeof(run_i) / sizeof(run_i[0])/PAGE_SIZE,
    0,      //page_index
    5,      //node_id
    run_i,
    {0, 0},
    &root_n,
    {0},
    menu_nop,
};
