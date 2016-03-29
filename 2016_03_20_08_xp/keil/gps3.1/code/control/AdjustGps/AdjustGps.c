#include "gps.h"
#include "AdjustGps.h"
#include "math.h"
#include "can.h"
#include "goPointPath.h"
void AdjustGps(float adjust_error)
{
float a=0,a_rad=0;
float i=0;
//float d1=150;
//float x1=584.1561;
//float y1=7270.5664;
//float theta1=44.9249/180*Pi;
float x1;
float y1;
float x2;
float y2;
float theta1;
float theta0=atan(y1/x1);
float theta2=theta1;
float t=sqrt(1+1/(tan(theta2)*tan(theta2)));
float k=sqrt(x1*x1+y1*y1);
float p=-1/tan(theta2);
float y=0;
	for (i=0;a<45;i++)
	{
	a=a+0.1;
	a_rad=a/180*3.1415926;
	y=adjust_error/t*(tan(theta1+a_rad)-p)+k*sin(theta0-a_rad)-y1+(x1-k*cos(theta0-a_rad))*tan(theta1+a_rad);
	if(y<0)
		break;
  }
	y2=k*sin(theta0-a);
  x2=k*cos(theta0-a);
	Gps_List[0].position.x=x2-adj_start.position.x+Gps_List[0].position.x;
	Gps_List[0].position.y=y2-adj_start.position.y+Gps_List[0].position.y;
	Gps_List[0].radian=Gps_List[0].radian+a_rad;
	Gps_List[0].angle=Gps_List[0].angle+a;
}
void CAN1_RX1_IRQHandler(void)
{
	uint8_t i;
	CanRxMsg   RxMessage;
	float delta_a=0;
	
	CAN_Receive(CAN1, CAN_FIFO1, &RxMessage);
	
	if(RxMessage.StdId==0x2)
	{
		delta_a=*((float*)RxMessage.Data);
	  AdjustGps(delta_a);
	}
}