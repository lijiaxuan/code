#ifndef PID_H
#define PID_H

struct PID
{
	float p;
	float i;
	float d;
	float mend;
	float ep1;
	float ep2;
};
extern struct PID Pid_List[];

void PID_Clear(void);

float Pid_Cal(double Error,struct PID *pid);

#endif
