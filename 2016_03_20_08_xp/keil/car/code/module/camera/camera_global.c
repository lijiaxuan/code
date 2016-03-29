/* 
* @Author: obsidian
* @Date:   2015-12-13 22:08:31
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-17 09:15:11
*/
#include "camera_global.h"
#include "driver/can/can.h"


struct Camera camera = 
{
    Can1_Init,
    0
};
