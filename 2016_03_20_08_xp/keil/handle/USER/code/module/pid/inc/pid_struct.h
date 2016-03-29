/* 
* @Author: obsidian
* @Date:   2015-12-16 10:18:32
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-18 17:35:15
*/
#ifndef PID_STRUCT_H

#define PID_NUM     24

#define ANGLE 		0
#define DISTANCE 	1


struct Pid_list
{
	float		kp;
	float		ki;
	float		kd;
};

struct Pid
{
	struct Pid_list pid_list[PID_NUM];
	float			mend[PID_NUM];
	float			error[PID_NUM][2];
	unsigned char	route_id;
	float			(*get_pid)(float error);
	float			(*get_double_loop_pid)(float distance_error,float angle_error);
};

#define PID_STRUCT_H
#endif
