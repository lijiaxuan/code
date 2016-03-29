#ifndef GPS_C
#define GPS_C

#include "gps.h"

#ifndef PI
#define PI 3.1415926
#endif

 Gps_State Gps_List[2]={{0,0,0,0,{0,0},0,0,0,0},
												{0,0,0,0,{0,0},0,0,0,0}};
 struct path_point  position_forward={0,0};

void GPS_Update(void)
{
 	float radian=0;
  float delta;
	float x_delta;
	float y_delta;
	
	Gps_List[1]=Gps_List[0];
	
	Gps_List[0].distance1 = Encoders[0].distance;
	Gps_List[0].distance2 = Encoders[1].distance;
	
 	Gps_List[0].distancestep1 = Gps_List[0].distance1 - Gps_List[1].distance1;
 	Gps_List[0].distancestep2 = Gps_List[0].distance2 - Gps_List[1].distance2; 
	Gps_List[0].speed=(Gps_List[0].distancestep2+Gps_List[0].distancestep1)/2/1;	//	m/s
	Gps_List[0].acceleration=(Gps_List[0].speed-Gps_List[1].speed)/1*1000;	// m/s^2

//		陀螺仪加码盘
	{
		Gps_List[0].angle=Gyro_Angle_Total;//角度范围为-180~180
		Gps_List[0].radian=Gyro_Angle_Total*PI/180;

		//取上一时刻与现在时刻的角平分线做为该时间内车身的角度
		radian=(Gps_List[0].radian+Gps_List[1].radian)/2;

        //判断是为了解决在PI分界线处的突变问题
		if(Gps_List[0].radian*Gps_List[1].radian<0)
			if(fabs(Gps_List[0].radian)+fabs(Gps_List[1].radian)>PI)
				radian=PI;

		delta=(Gps_List[0].distancestep1+Gps_List[0].distancestep2)/2;
		x_delta=delta*cos(PI/2-radian);
		y_delta=delta*sin(PI/2-radian);
			
		Gps_List[0].position.x+=x_delta;
		Gps_List[0].position.y+=y_delta;
		
	}
	position_forward.x=Gps_List[0].position.x+CARLENGTH*cos(PI/2-Gps_List[0].radian);
	position_forward.y=Gps_List[0].position.y+CARLENGTH*sin(PI/2-Gps_List[0].radian);
}


void GPS_Init(struct path_point position,double radian)
{
	int i;

	Encoder_Clear(0);
	Encoder_Clear(1);
	Gyro_Clear();

	for(i=0;i<2;i++)
	{
		Gps_List[i].distance1=0.0;
		Gps_List[i].distance2=0.0;
		Gps_List[i].distancestep1=0.0;
		Gps_List[i].distancestep2=0.0;
		Gps_List[i].position=position;
		Gps_List[i].angle=radian/PI*180.0;
		Gps_List[i].radian=radian;
		Gps_List[i].speed=0.0;
		Gps_List[i].acceleration=0.0;
	}
}



#endif
