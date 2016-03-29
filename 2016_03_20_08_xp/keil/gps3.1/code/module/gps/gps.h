#ifndef GPS_H
#define GPS_H

#include "PointRoute.h"
#include "encoder.h"
#include "led.h"
#include "gyro.h"

#define CARLENGTH 256//第三版  从后轮中心到车前轮中心的距离

typedef struct 
{   
	float			            distance1;	  	//码盘1前进距离
	float			        		distance2;		  //码盘2前进距离
	float				        	distancestep1; 	//码盘1本次前进距离
	float				        	distancestep2;  //码盘2本次前进距离
	struct path_point       position;	  	  //当前坐标	
	float  	        			angle;        	//当前角度 		
	float				        	radian;			    //当前弧度
	float				        		speed;			    //当前速度
	float				        		acceleration;	  //当前加速度
}Gps_State;

extern 				 Gps_State Gps_List[2];
extern  struct path_point  position_forward;
				void GPS_Init(struct path_point position,double radian);
				void GPS_Update(void);
void TIM1_INIT(void);
#endif
