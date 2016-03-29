/* Includes ------------------------------------------------------------------*/
#include "all.h"

/*
 * 函数名: set_Engine
 * 描  述: 舵机显示与修改
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */
 
//第一版
#ifdef FIRST
 int mid_value_pwm1=1445;
 int mid_value_pwm2=1020;    //30度时正常过赛道
                            //36开始接触地面
                            //37度开始刹车
 #define ANGLE_CONVERT_pwm1   -3.6
#define pwm1_value_scope     171
#define ANGLE_CONVERT_pwm2   -3.6
#define  pwm2_value_scope    171

#endif
#ifdef SECOND
  //第二版
  int mid_value_pwm1=1500;
  int mid_value_pwm2=1300;
  #define ANGLE_CONVERT_pwm1   3.6
#define pwm1_value_scope     171
#define ANGLE_CONVERT_pwm2   3.6
#define  pwm2_value_scope    171
  
 
 #endif
 
double steer_angle=0;

void set_Engine_pwm1(void)
{
	while(1)
	{
		
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("========ENGINE========");
		LCD_SetXY(0,1);
		LCD_WriteString("angle:");
		LCD_WriteDouble(steer_angle,1);
		LCD_SetXY(0,2);		
		LCD_WriteString("MID_VALUE_1:");
		LCD_WriteInt(mid_value_pwm1);
		LCD_SetXY(0,3);
		LCD_WriteString("1.Set_A   2.Set_M");
		refresh();	
		if(KeyReady==1)
		  {
			KeyReady=0;
			switch(KeyValue)
			{
				
				case key1:
					Input_DoubleValue(&steer_angle,"angle");
					SET_ANGLE(steer_angle,1);
					break;
				case key2:
					Input_IntValue(&mid_value_pwm1,"MID_VALUE");
					break;
				case keyback:
					return;
			}
		  }
		delay(LCD_DELAY);
	}
}



void set_Engine_pwm2(void)//刹车 
{
	while(1)
	{
		
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("========ENGINE========");
		LCD_SetXY(0,1);
		LCD_WriteString("angle:");
		LCD_WriteDouble(steer_angle,2);
		LCD_SetXY(0,2);		
		LCD_WriteString("Stop_Point:");
		LCD_WriteInt(Stop_Point);
		LCD_SetXY(0,3);
		LCD_WriteString("1.Set_A   2.Set_M");
		refresh();	
        if(KeyReady==1)
		  {
			KeyReady=0;
			switch(KeyValue)
			{
				
				case key1:
					Input_DoubleValue(&steer_angle,"angle");
					SET_ANGLE(steer_angle,2);
					break;
				case key2:
					Input_IntValue(&mid_value_pwm2,"MID_VALUE");
					break;
				case keyback:
					
					return;
			}
		  }
		delay(LCD_DELAY);
	}
}



void SET_ANGLE(double angle,int pwm_port)//控制舵机偏转角度, 右偏为正
{
	u16 pwm_count;
	if(pwm_port==1)
	{
		pwm_count=mid_value_pwm1+(int)(angle*ANGLE_CONVERT_pwm1);	//
		if(pwm_count>mid_value_pwm1+171)
			pwm_count=mid_value_pwm1+171;
		else if(pwm_count<mid_value_pwm1-171)
			pwm_count=mid_value_pwm1-171;
		TIM4 ->CCR4=pwm_count;
    }
	
	if(pwm_port==2)
	{
		pwm_count=mid_value_pwm2+(int)(angle*ANGLE_CONVERT_pwm2);	//
// 		if(pwm_count>mid_value_pwm2+271)
// 			pwm_count=mid_value_pwm2+271;
// 		else if(pwm_count<mid_value_pwm2-271)
// 			pwm_count=mid_value_pwm2-271;
		TIM4 ->CCR3=pwm_count;
  }
 
}


/********************************    END OF FILE    ***************************/
