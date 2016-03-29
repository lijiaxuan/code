/* 
* @Author: obsidian
* @Date:   2015-12-13 22:08:31
* @Last Modified by:   obsidian
* @Last Modified time: 2016-02-28 14:29:17
*/
#include "../inc/camera_global.h"
#include "driver/can/inc/canljx.h"
struct Camera camera = 
{
    Can1_Init,
    0
};