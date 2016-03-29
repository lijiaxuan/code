#include "all.h"

int Stop_Point=0;
int stopcount=0;//刹车第二情况判断标志

float stop_engine_angle=37;
float stop_engine=80;

int stopCount=0;
int stopStatus=0;

void stop(void)
{
    if(stopStatus)
    {
        SET_ANGLE(38,2);  
        return;
    }
    else if((Point_NowNum>Route[20-1]+1 &&Point_NowNum<Route[21-1] )|| (Point_NowNum>Route[20+25-1]+1 &&Point_NowNum<Route[21+25-1] ))//??2??
         {
             if(stopCount < 1)
             {
                 brakesec=3;
                 TIM_Cmd(TIM6,ENABLE);
                 stopCount=1;
                 stopStatus=1;
                 return;
             }
             
             if(Gps_List[0].speed>0.4)
             {
                 stop_engine_angle=stop_engine_angle+2.5;
             }
             else if(Gps_List[0].speed<0.35)
             {
                 stop_engine_angle=stop_engine_angle-1.2;
             }	
         }
        else if( (Point_NowNum>Route[23-1]+1 &&Point_NowNum<Route[24-1]+70) || (Point_NowNum>Route[23+25-1]+1 &&Point_NowNum<Route[24+25-1]+70) )//山道3直线
         {
             if(stopCount < 2)
             {
                 brakesec=2;
                 TIM_Cmd(TIM6,ENABLE);
                 stopCount=2;
                 stopStatus=1;
                 return;
             }

             if(Gps_List[0].speed>0.3)
             {
                 stop_engine_angle=stop_engine_angle+1.5;
             }
             if(Gps_List[0].speed<0.2)
             {
                 stop_engine_angle=stop_engine_angle-0.5;
             }
         }
 
//         else if(Point_NowNum>=Route[24-1]+3&& Point_NowNum<=Route[24-1]+70)//最后一段
//         {
//             if(Gps_List[0].speed>0.09)
//             {
//                 stop_engine_angle=stop_engine_angle+0.3;
//             }
//             if(Gps_List[0].speed<0.02)
//             {
//                 stop_engine_angle=stop_engine_angle-0.3;
//             }        
//	  }
         else if( (Point_NowNum>=Route[24-1]+100  &&Point_NowNum<=Route[25-1]) || (Point_NowNum>=Route[24+25-1]+100  &&Point_NowNum<=Route[25+25-1] ))//最后一段
         {
                 stop_engine_angle=38;
               
        }
         else
         {
             stop_engine_angle=0;
         }
         
         
         if(stop_engine_angle>38)
             stop_engine_angle=38;
         else if(stop_engine_angle<0)
             stop_engine_angle=0;
         SET_ANGLE(stop_engine_angle,2); 
         
}

void brake_timer_cfg(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 , ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 1440 - 1 ; 
	TIM_TimeBaseStructure.TIM_Period = 50000; // 5000 - 5ms
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM6,TIM_IT_Update);
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	
	//TIM_Cmd(TIM6,ENABLE);

}

static char count = 0;
int brakesec=0;

void TIM6_IRQHandler(void)
{
	TIM_ClearFlag(TIM6,TIM_IT_Update);
	count ++;
	if(count <= brakesec)
	{
		return;
	}
	SET_ANGLE(36,2);  
	stopStatus = 0;
	TIM_Cmd(TIM6,DISABLE);
}
