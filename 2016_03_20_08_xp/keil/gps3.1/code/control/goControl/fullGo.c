#include "fullGo.h"

int	Route_Num=0;
int Point_NowNum=0;
int Point_StartNum=0;
int Point_EndNum=0;

void SetPath_begin_and_end(int sta_num,int end_num)
{
	Point_StartNum = sta_num;
	Point_EndNum   = end_num;
	Point_NowNum   = sta_num;
}

void full_go_right(void)//右边跑道
{
	char back_1=0;
	Point_NowNum=0;
	Route_Num=0;//count from 0
	
  struct path_point gps_zero={0,599.4};//启动的地点 小车的后轮的坐标
 
	GPS_Init(gps_zero,0);

	//初始化路径信息    
	SetPath_begin_and_end(Route[0],Route[1]);
	LCD_Clear_Forward(7);

	while(1)
	{
		go_pointPath_right();//执行一次所需时间0.00121187	1.2ms
		
		brake(Point_NowNum,Gps_List[0].speed);
		
		LCD_WriteStringXY("ROUTE_NUM",0,0);
		LCD_WriteIntXY(Route_Num,10,0);
				
		LCD_WriteStringXY("For_X:",0,1);
		LCD_WriteDoubleXY(position_forward.x/1000,2,2,1);//m
		
		LCD_WriteStringXY("For_Y:",7,1);
		LCD_WriteDoubleXY(position_forward.y/1000,2,9,1);//m
		
		LCD_WriteStringXY("A:",14,1);
		LCD_WriteDoubleXY(Gps_List[0].angle,1,16,2);
				
		LCD_WriteStringXY("engine_A:",0,3);
		LCD_WriteDoubleXY(engine_angle1,1,10,3);
		
		LCD_WriteStringXY("d:",0,4);
		LCD_WriteDoubleXY(error_dis,1,2,4);
		LCD_WriteStringXY("a:",7,4);
		LCD_WriteDoubleXY(error_angle,1,9,4);
	
		LCD_WriteStringXY("NowNum:",0,5);
		
		LCD_WriteIntXY(Point_NowNum,8,5);
		
// 		LCD_WriteStringXY("T:",0,6);
// 		LCD_WriteIntXY(real_time/1000,2,6);
		
		LCD_WriteStringXY("V:",9,6);
		LCD_WriteDoubleXY(Gps_List[0].speed,1,11,6);
		
		
// 		LCD_WriteIntXY(real_time,0,7);
				 
// 		#if GYRO
// 		if(Gyro_Error_Flag)
// 		{
// 				UART1_DMA_EN();
// 				Gyro_Error_Flag = 0;
// 		}	
// 		#endif
		  	
		if (KeyReady==1 && KeyValue==5 && back_1 != 1)
		{
			KeyReady=0;
			back_1=1;	
		}
		if (KeyReady==1 && KeyValue==5 && back_1==1)
		{
			KeyReady=0;
			back_1=0;
			SET_ANGLE(0,1);
			SET_ANGLE(0,2);
			return;				
		}
		refresh_slow();
		delay(3);
	}
}



