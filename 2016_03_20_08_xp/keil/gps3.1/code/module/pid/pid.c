#ifndef PID_C
#define PID_C

#include "pid.h"

#define Normal_pid	{0.3,0.0,0.00},
#define BIG_PID		{0.175,0.00,0.07},

struct PID Pid_List[]=
{
	Normal_pid
	Normal_pid
	Normal_pid
	Normal_pid
	Normal_pid
	Normal_pid
	Normal_pid
	Normal_pid
	Normal_pid
	Normal_pid
	Normal_pid
	Normal_pid//11th(from 0)
	Normal_pid
	Normal_pid
	Normal_pid
	Normal_pid
	Normal_pid
	Normal_pid
	Normal_pid//18 the first yellow big turn
	Normal_pid
	Normal_pid//20 the second yellow big turn
	Normal_pid
	Normal_pid
	Normal_pid
};


/*
 * 函数名: PID_Clear
 * 描  述: 将PID存储的误差值清除
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */
void PID_Clear(void)
{
	int i;
	
	for(i=0;i<23;i++)
	{
		Pid_List[i].mend=0;
		Pid_List[i].ep1=0;
		Pid_List[i].ep2=0;
	}
}

/*
 * 函数名: PID
 * 描  述: 位置式PID调整，利用了增量式的递推
 * 输  入: -Error 误差量
 *		   -pid pid参数
 * 输  出: 反馈结果
 * 调  用: 外部调用
 */
float Pid_Cal(double Error,struct PID *pid)
{
	double A=0;
	double B=0;
	double C=0;
	double delta=0;
	
	A =  (pid->p+pid->i+pid->d);
	B = -(pid->p+2*pid->d);
	C = pid->d;
	
	delta = A*Error + B*pid->ep1 + C*pid->ep2;
	pid->mend += delta;
	
	
	pid->ep2 = pid->ep1;
	pid->ep1 = Error;
	
	return pid->mend;
}


#endif 

/**********************************END OF FILE*********************************/
