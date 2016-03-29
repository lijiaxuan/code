/* 
* @Author: obsidian
* @Date:   2015-12-13 22:04:14
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-18 14:00:17
*/
#ifndef ENCODER_GLOBAL_H
#include "GPS_struct.h"
extern struct GPS			gps;
extern struct Encoder_property encoder_property[ENCODER_NUM];
extern struct Gyro          gyro;
extern struct Encoder_data  encoder[ENCODER_NUM];
#define ENCODER_GLOBAL_H
#endif
