/* 
* @Author: obsidian
* @Date:   2016-02-23 14:28:55
* @Last Modified by:   obsidian
* @Last Modified time: 2016-02-23 14:55:43
*/
#ifndef INFRARED_STRUCT_H
#define INFRARED_STRUCT_H

struct Infrared
{
    signed char flag;
    float       step;
    void       (*infrared_init)();
};


#endif