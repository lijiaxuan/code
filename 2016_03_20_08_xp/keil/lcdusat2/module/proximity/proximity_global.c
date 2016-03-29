/* 
* @Author: obsidian
* @Date:   2016-02-23 14:28:18
* @Last Modified by:   obsidian
* @Last Modified time: 2016-02-29 21:46:27
*/
#include "proximity_global.h"
#include "proximity.h"
struct Proximity proximity = 
{
    0,                          //flag  unsigned char
    1.0,                        //step  float
    proximity_init,             //fun
};
