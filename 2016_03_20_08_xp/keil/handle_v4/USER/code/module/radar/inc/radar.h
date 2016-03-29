/* 
* @Author: obsidian
* @Date:   2016-01-16 14:40:26
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-30 16:50:58
*/
#ifndef RADAR_H
#include "stm32f10x_usart.h"
void radar_init(void);
void update(void);
void scan(void);
#define RADAR_H
#endif
