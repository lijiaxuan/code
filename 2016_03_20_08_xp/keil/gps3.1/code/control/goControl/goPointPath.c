#include "goPointPath.h"
#include "fullGo.h"
#include "can.h"
#include "gps.h"
#ifndef PI
#define PI 3.1415926
#endif
uint8_t adj_gpsflag=0;
Gps_State adj_start;
// float get_point2line_length(struct path_point p_point,struct path_point p_line_point,float angle)
// {
// 	float k = tan((90 - angle) * PI / 180);
// 	float a = p_point.y - k*p_point.x + k * p_line_point.x - p_line_point.y;
// 	return a/(sqrt(1 + k*k));
// }

 float get_point2line_length(struct path_point p_point,struct path_point p_line_point,float angle)
 {
	 float a,b,c;
	a=angle;
	b=-1;
	c=-angle*p_line_point.x+p_line_point.y;
	 return (a*p_point.x+b*p_point.y+c)/(sqrt(a*a+b*b));
 }
	 


float get_point2point_length(struct path_point p_line_point1,struct path_point p_line_point2)
{
	return sqrt((p_line_point1.x - p_line_point2.x)*(p_line_point1.x - p_line_point2.x) + ((p_line_point1.y - p_line_point2.y)*(p_line_point1.y - p_line_point2.y)));
}

float GetLength(struct path_point  a,struct path_point  b)
{
	float length;
	float dx,dy;
	dx = ((float)a.x-(float)b.x)*((float)a.x-(float)b.x);
	dy = ((float)a.y-(float)b.y)*((float)a.y-(float)b.y);
	length = sqrt(dx+dy);
	return length;
}

void STEER_CONTROL(float pos_error_temp, float ang_error_temp,struct PID *pid);

float error_angle=0;
float error_dis=0;	

//void go_pointPath_right(int PointNowNum,unsigned char  RouteNum)	//	5ms控制一次
void go_pointPath_right()
{
	int count_scope=5;				//搜索范围
	int count_delta=0; 				//步进点的个数
	int i;
	int  pro_count=0;            	//提前点个数
	
	//搜索下一个目标点
	for(i=0;i<count_scope;i++)
	{
		if((Point_NowNum+i) >= Point_EndNum-2) 
		{
			Route_Num+=1;
			Pid_List[Route_Num].mend=Pid_List[Route_Num-1].mend;
			Pid_List[Route_Num].ep1 =Pid_List[Route_Num-1].ep1;
			Pid_List[Route_Num].ep2 =Pid_List[Route_Num-1].ep2;
			SetPath_begin_and_end(Route[Route_Num],Route[Route_Num+1]);
			break;			
		}
		if(GetLength( PointRoute[Point_NowNum+i+1].position,position_forward)
			<= GetLength( PointRoute[Point_NowNum+count_delta].position,position_forward ))		
			count_delta = i+1;
	}
	Point_NowNum += count_delta;		 
	
	
	//提前点需要根据舵机响应时间进行计算  队里不同舵机响应时间差别较大
	if(Gps_List[0].speed<0.3)	
		pro_count=5;
	else if(Gps_List[0].speed<0.5)
		pro_count=10;
	else if(Gps_List[0].speed<1)
		pro_count=15;
	else if(Gps_List[0].speed<1.5)
		pro_count=20;
	else if(Gps_List[0].speed<2)
		pro_count=25;
	
	//角度误差和距离误差
	error_angle = (PointRoute[Point_NowNum].selfdir-Gps_List[0].radian)*180/PI;
	
	if((PI/2-PointRoute[Point_NowNum].selfdir) == PI/2)
		error_dis=position_forward.x-PointRoute[Point_NowNum].position.x;
	else
		error_dis=get_point2line_length(position_forward,PointRoute[Point_NowNum].position,tan(PI/2-PointRoute[Point_NowNum].selfdir));
	
	if(PI/2-PointRoute[Point_NowNum].selfdir<=PI/2)
	{
		error_dis=-error_dis;  
	}
			
	if(error_angle>2*PI)
		error_angle-=2*PI;
	if(error_angle<-2*PI)
		error_angle+=2*PI;
	
	STEER_CONTROL(error_dis,error_angle,&Pid_List[Route_Num]);
	if(Route_Num==17)
	{
		if(adj_gpsflag==0)
		{		
	   Can_RXMessage(0x01,1);
//	   adj_gpsflag=1;
			delay(20);
	   adj_start=Gps_List[0];
		}
	}
}

void STEER_CONTROL(float pos_error_temp, float ang_error_temp,struct PID *pid)
{
	float fusion;
	float angel;

	static double k=4.5;	
	fusion=pos_error_temp+k*ang_error_temp;//综合位置和角度误差信息
	
	angel=Pid_Cal(fusion,pid);
	SET_ANGLE(angel,1);//舵机输出控制
}

