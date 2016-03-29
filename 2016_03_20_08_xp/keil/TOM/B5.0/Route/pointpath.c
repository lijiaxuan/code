
/* Includes ------------------------------------------------------------------*/
#include "all.h"
#include "math.h" 

extern struct RouteInf RouteList[24];

//每一段路径的起点为点阵中的  下标为Route[Route_Num-1] 第Route[Route_Num-1]+1 个数据
//第 1 段路径为 第1个点   ——>  第159个点 
//第 2 段路径为 第160个点 ——>  第259个点

int	Route_Num=1;

int Route[50]={0, 162, 258, 402, 498, 639, 735, 879, 975, 1147, 1242, 1377, 1524, 1631, 1713, 1795, 1981, 2076, 2434, 2636, 2826, 3027, 3158, 3276, 3757,
//				ST	 c1   sl   c2   h1   c3   s2   c4   h2   c5     s3    c6   h3    c7     c8    r     c9    d1   dc1    d2   dc2    d3   dc3    d4
     3757, 3919, 4015, 4159, 4255, 4396, 4492, 4636, 4732, 4904, 4999, 5134, 5281, 5388, 5470, 5552, 5738, 5833, 6191, 6393, 6583, 6784, 6915, 7033, 7514,  };
//		ST	  c1     sl   c2    h1     c3   s2    c4    h2    c5     s3    c6   h3    c7     c8    r     c9    d1   dc1    d2   dc2    d3   dc3    d4	


    
int Point_NowNum=0;
int Point_StartNum=0;
int Point_EndNum=3347-5;
    
int Point_NowNum_Back=0;
int Point_NowNum_Center=0;    
  
    
//角度误差与距离误差   
float error_angle=0;
float error_dis=0;				
float fusion=0;
float angel=0;
    
float error_dis_back=0;	
float error_dis_center=0;	    

	/*
 * 函数名: SetPointPath
 * 描  述: 初始化跑点阵路径
 * 输  入: -sta_num 路径开始的点
 *		   -end_num 路径结束的点
 * 输  出: 无
 * 调  用: 外部调用
 */
void SetPointPath(int sta_num,int end_num)
{
	Point_StartNum = sta_num;
	Point_EndNum = end_num;
	Point_NowNum = sta_num;
	PID_Clear();
	//RouteFinish=1;//1表示路径未结束，0表示路径结束
	//TIM2->CNT = 0;
}

void Go_Path(u8 RouteNum)	//	5ms控制一次
{
	int count_scope=5;				//搜索范围
	int count_delta=0;
	int i;
	int  pro_count=0;
    static double k=4.5;		//小车角度偏50度 位置偏20MM 可认为下一个控制周期时误差为0
//    static double k=3.1415;		
	
	//搜索下一个目标点
	for(i=0;i<count_scope;i++)
	{
		if((Point_NowNum+i) >= Point_EndNum-2) 
		{
			Route_Num+=1;
//			if(Route_Num>=24) ;
			Pid_List[Route_Num-1].mend=Pid_List[Route_Num-2].mend;
			Pid_List[Route_Num-1].ep1=Pid_List[Route_Num-2].ep1;
			Pid_List[Route_Num-1].ep2=Pid_List[Route_Num-2].ep2;
			SetPointPath(Route[Route_Num-1]+1,Route[Route_Num]);
			break;
			
		}
		if(GetLength( PointRoute[Point_NowNum+i+1].position,position_forward)
			<= GetLength( PointRoute[Point_NowNum+i].position,position_forward ))		
			count_delta = i+1;
	}

	Point_NowNum += count_delta;
    
    for(i=0;i<4;i+=2)
	{
		if(GetLength( PointRoute[Point_NowNum_Back+i+1].position,Gps_List[0].position)
			<= GetLength( PointRoute[Point_NowNum_Back+i].position,Gps_List[0].position))		
			count_delta = i+1;
	}
	Point_NowNum_Back += count_delta;
    
    for(i=0;i<4;i+=2)
	{
		if(GetLength( PointRoute[Point_NowNum_Center+i+1].position,position_center)
			<= GetLength( PointRoute[Point_NowNum_Center+i].position,position_center))		
			count_delta = i+1;
	}
	Point_NowNum_Center += count_delta;
	
	if(Point_NowNum>Route[StartNum-1]+Start_Offset+5 && Point_NowNum<Route[StartNum-1]+Start_Offset+15)
		LCD_WriteIntXY(real_time,0,7);
	else if(Point_NowNum>3000 && Point_NowNum<3015)
		LCD_WriteIntXY(real_time,7,7);
	
// 	if(Gps_List[0].speed<0.3)	
// 		pro_count=5;
// 	else if(Gps_List[0].speed<0.5)
//     {
// 		pro_count=10;
//         k=4.5;
//     }
// 	else if(Gps_List[0].speed<1)
//     {
// 		pro_count=25;
//         k=4;
//     }
// 	else if(Gps_List[0].speed<1.5)
//     {
// 		pro_count=30;
//         k=3.5;
//     }
// 	else if(Gps_List[0].speed<2)
//     {
// 		pro_count=45;
//         k=3;
//     }
//     else 
//         pro_count=50;

// 	if(Gps_List[0].speed<0.3)	
// 		pro_count=5;
// 	else if(Gps_List[0].speed<0.7)
// 		pro_count=10;
// 	else if(Gps_List[0].speed<1.0)
// 		pro_count=15;
// 	else if(Gps_List[0].speed<1.5)
// 		pro_count=20;
// 	else if(Gps_List[0].speed<2)
// 		pro_count=30;
//     else if(Gps_List[0].speed<2.5)
//         pro_count=35;
//     else
//         pro_count=40;
	pro_count=0;
	//角度误差和距离误差
	error_angle = (PointRoute[Point_NowNum+pro_count].selfdir-Gps_List[0].radian)*180/PI;		//单位为角度
	
	if((PI/2-PointRoute[Point_NowNum].selfdir) == PI/2)
		error_dis=PointRoute[Point_NowNum].position.x-position_forward.x;
	else
    {
		error_dis=GetDis_P2L_PK(position_forward,PointRoute[Point_NowNum].position,tan(PI/2-PointRoute[Point_NowNum].selfdir));
		if(PI/2-PointRoute[Point_NowNum].selfdir<=PI/2)
		{
			error_dis=-error_dis;
		}
    }        //单位为mm  

    if((PI/2-PointRoute[Point_NowNum_Back].selfdir) == PI/2)
		error_dis_back=Gps_List[0].position.x-PointRoute[Point_NowNum_Back].position.x;
	else
    {
		error_dis_back =GetDis_P2L_PK(Gps_List[0].position,PointRoute[Point_NowNum_Back].position,tan(PI/2-PointRoute[Point_NowNum_Back].selfdir));
		if(PI/2-PointRoute[Point_NowNum_Back].selfdir<=PI/2)
		{
			error_dis_back =-error_dis_back ;
		}
    }
        
//     if((PI/2-PointRoute[Point_NowNum_Center].selfdir) == PI/2)
//     {
// 		error_dis_center=position_center.x-PointRoute[Point_NowNum_Center].position.x;
//     }
// 	else
//     {
// 		error_dis_center =GetDis_P2L_PK(position_center,PointRoute[Point_NowNum_Center].position,tan(PI/2-PointRoute[Point_NowNum_Center].selfdir));
// 		if(PI/2-PointRoute[Point_NowNum_Center].selfdir<=PI/2)
// 		{
// 			error_dis_center =-error_dis_center ;
// 		}
//     }
        
    if(error_angle>180)
		error_angle-=360;
	if(error_angle<-180)
		error_angle+=360;
    //重点在于error distance怎么求
//    error_dis=error_dis_center;
//     if( F_Abs(error_dis_back) > F_Abs(error_dis) )
//     {
//         error_dis=error_dis_back;
//     }
    if(( (Route_Num>=18 && Route_Num <= 24) || (Route_Num >=(18+25)&& Route_Num <= (24+25) ) ))
    {
        if (camerError != 888)
        {
            error_dis=(camerError-155)*300/80/4;
        }
        else
        {
//             if(Route_Num==19||Route_Num==19+25){error_dis=(320-155)*300/80/4;}
//             else if(Route_Num==21||Route_Num==21+25){error_dis=(0-155)*300/80/4;}
//             else if(Route_Num==23||Route_Num==23+25){error_dis=(320-155)*300/80/4;}
            ;
        }
    }
	//PID
    
//     if(RouteList[Route_Num-1].type==1)
//     {
//         k=4.5;
//     }
//     else
//     {
//         k=1.0;
//     }
    fusion=error_dis+k*error_angle;//综合位置和角度误差信息
    
    if(( (Route_Num ==24) || ( Route_Num == (24+25) ) )&& (camerError != 888))
    {
        fusion=error_dis;//综合位置和角度误差信息
    }
    
// 	angel=Pid_Cal(fusion,Pid_List[RouteNum-1]);
//    angel=Pid_Cal(fusion,RouteNum-1);
    angel=LocPIDCalc(fusion,RouteNum-1);
	if(angel > 50)           angel=50;
	else if(angel < -50)    angel=-50;
		 
	//舵机输出控制
	SET_ANGLE(angel,1);    
//	error_angle=F_Abs(error_angle);
//	dis_control(error_dis,error_angle);
	
}

int angle_2;
struct PID pid_adapt={0.5,0.00,0.05};
float k0=100;
float k1=2.5;
float k2=1.5;
float con_p=1000;

void dis_control(float err_dis,float err_angle)
{

	pid_adapt.p=(k0+error_angle*k1+error_angle*error_angle*k2)/con_p;
// 	angle_2=Pid_Cal(error_dis,pid_adapt);
	//40
		if(angle_2 > 35)
		 angle_2=35;
		else if(angle_2 < -35)
			angle_2=-35;
	
	SET_ANGLE(angle_2,1);
}


/**********************************END OF FILE*********************************/

