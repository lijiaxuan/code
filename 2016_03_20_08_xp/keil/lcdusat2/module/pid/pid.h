/* 
* @Author: obsidian
* @Date:   2015-12-16 10:17:47
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-28 21:48:15
*/
#ifndef PID_H
#include "pid_struct.h"

float get_pid(float error);
float get_double_loop_pid(float distance_error,float angle_error);
float get_angle_pid(float error);
void  pid_clear();
void pid_init();
void pid_save();
#define PID_H
#endif