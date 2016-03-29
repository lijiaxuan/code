/* 
* @Author: obsidian
* @Date:   2016-01-16 14:40:26
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-28 19:25:30
*/
#ifndef RADAR_H
#include "stm32f10x_usart.h"
void radar_init();
void send_qt(void);
void update(void);
void send_bm(void);
void send_gs(void);
#define RADAR_H
#endif
