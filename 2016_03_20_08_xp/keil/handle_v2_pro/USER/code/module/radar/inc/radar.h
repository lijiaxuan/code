/* 
* @Author: obsidian
* @Date:   2016-01-16 14:40:26
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-24 09:35:41
*/
#ifndef RADAR_H
#include "stm32f10x_usart.h"
void radar_init(float * px,float *py,float* pradian);
void send_qt(void);
void update(void);
void send_bm(void);
void send_gs(void);
#define RADAR_H
#endif
