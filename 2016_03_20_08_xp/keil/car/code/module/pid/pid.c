/* 
* @Author: obsidian
* @Date:   2015-12-16 10:18:52
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-12 20:43:49
*/
#include "pid.h"
#include "pid_global.h"
#include "driver/sd/file.h"
#include "control/car.h"

void pid_init()
{
	unsigned int k = 0;
	car.strjoint("pid");
    f_open(&fil,car.path,FA_READ);

    if(fil.fs == 0){return;}
    f_read(&fil, (char *)pid.pid_list, sizeof(pid.pid_list), &k);
    file_close();
}
void pid_save()
{
	  unsigned int k = 0;
	car.strjoint("pid");
    f_open(&fil,car.path,FA_WRITE | FA_CREATE_ALWAYS);
    if(fil.fs == 0){return;}
    f_write(&fil, (char *)pid.pid_list, sizeof(pid.pid_list), &k);
    file_close();
}



void pid_clear()
{
	unsigned char i = 0;
	for(i = 0;i < 24; i ++)
	{
		pid.mend[i] = 0;
		pid.error[i][0] = 0;
		pid.error[i][1] = 0;
	}
}
float get_pid(float error)
{
	float A=0;
	float B=0;
	float C=0;
	float delta=0;

	
	A = pid.pid_list[pid.route_id].kd + pid.pid_list[pid.route_id].ki + pid.pid_list[pid.route_id].kp;
	B = - (pid.pid_list[pid.route_id].kp + 2 * pid.pid_list[pid.route_id].kd);
	C = pid.pid_list[pid.route_id].kd;
	
	delta = A*error + B*pid.error[pid.route_id][0] + C*pid.error[pid.route_id][1];
	pid.mend[pid.route_id] += delta;
	
	pid.error[pid.route_id][1] = pid.error[pid.route_id][0];
	pid.error[pid.route_id][0] = error;
	
	return pid.mend[pid.route_id];
}

float get_double_loop_pid(float distance_error,float angle_error)
{
	float A = 0;
	float B = 0;
	float C = 0;
	float angle_delta = 0;
	float distance_delta = 0;
	
	//distance_error = distance_error/pid.pid_list[DISTANCE].coff;
	//distance_error = distance_error * distance_error * distance_error / 10000;
	A = (pid.pid_list[ANGLE].kp*(1+1/pid.pid_list[ANGLE].ki+pid.pid_list[ANGLE].kd));
	B = -pid.pid_list[ANGLE].kp*(1+2*pid.pid_list[ANGLE].kd);
	C = pid.pid_list[ANGLE].kp*pid.pid_list[ANGLE].kd;
	
	angle_delta = A*angle_error + B*pid.error[ANGLE][0] + C*pid.error[ANGLE][1];
	pid.mend[ANGLE] += angle_delta;
	
	pid.error[ANGLE][1] = pid.error[ANGLE][0];
	pid.error[ANGLE][0] = angle_error;


	distance_error += angle_delta;
	A = (pid.pid_list[DISTANCE].kp*(1+1/pid.pid_list[DISTANCE].ki+pid.pid_list[DISTANCE].kd));
	B = -pid.pid_list[DISTANCE].kp*(1+2*pid.pid_list[DISTANCE].kd);
	C = pid.pid_list[DISTANCE].kp*pid.pid_list[DISTANCE].kd;
	
	distance_delta = A*distance_error + B*pid.error[DISTANCE][0] + C*pid.error[DISTANCE][1];
	
	pid.error[DISTANCE][1] = pid.error[DISTANCE][0];
	pid.error[DISTANCE][0] = distance_error;
	pid.mend[DISTANCE] += distance_delta;
	return pid.mend[DISTANCE];
}

float get_angle_pid(float error)
{
	float A=0;
	float B=0;
	float C=0;
	float delta=0;

	
	A = (pid.pid_list[ANGLE].kp*(1+1/pid.pid_list[ANGLE].ki+pid.pid_list[ANGLE].kd));
	B = -pid.pid_list[ANGLE].kp*(1+2*pid.pid_list[ANGLE].kd);
	C = pid.pid_list[ANGLE].kp*pid.pid_list[ANGLE].kd;
	
	delta = A*error + B*pid.error[ANGLE][0] + C*pid.error[ANGLE][1];
	pid.mend[ANGLE] += delta;
	
	pid.error[ANGLE][1] = pid.error[ANGLE][0];
	pid.error[ANGLE][0] = error;
	
	return pid.mend[ANGLE];
}
