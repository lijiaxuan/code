
#include "engineConfig.h"

void brake(int Point_NowNum_b,float speed)
{
  double stop_engine_angle;
	if(	Point_NowNum_b>2632&&Point_NowNum_b<3133)//第一阶段较小限速
	 {
		 if(speed>1)
		 {
			 stop_engine_angle=stop_engine_angle-0.3;
		 }
		 if(speed<0.9)
		 {
			 stop_engine_angle=stop_engine_angle+1.2;
		 }		 
	 }
	 
	 if(	Point_NowNum_b>3133&&Point_NowNum_b<3340)//第二阶段较大限速
	 {
		 if(speed>0.6)
		 {
			 stop_engine_angle=stop_engine_angle-0.3;
		 }
		 if(speed<0.4)
		 {
			 stop_engine_angle=stop_engine_angle+0.7;
		 }		 
	 }
	 if(Point_NowNum_b>=3340)//最后一段直接以较大角度刹车
	 {
		 stop_engine_angle=-85;
	 }
	 
	 if(stop_engine_angle<-85)
		 stop_engine_angle=-85;
	 if(stop_engine_angle>-30)
		 stop_engine_angle=-30;
	 SET_ANGLE(stop_engine_angle,2);
}
