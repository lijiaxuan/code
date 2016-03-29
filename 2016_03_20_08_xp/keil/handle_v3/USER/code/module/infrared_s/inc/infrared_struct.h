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


#define SA1  ((GPIOA->IDR &(1 << 15)) >> 15)
#define SA2  ((GPIOC->IDR &(1 << 10)) >> 10)
#define SA3  ((GPIOC->IDR &(1 << 11)) >> 11)
#define SA4  ((GPIOC->IDR &(1 << 12)) >> 12)


#endif