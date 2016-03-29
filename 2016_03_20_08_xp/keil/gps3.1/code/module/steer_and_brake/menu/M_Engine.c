#include "M_Engine.h"

double steer_angle1=0;
void set_Engine_pwm1()
{
	steer_angle1=0;
	while(1)
	{
		refresh();	
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("========ENGINE1========");
		LCD_SetXY(0,1);
		LCD_WriteString("angle:");
		LCD_WriteDouble(steer_angle1,1);
		LCD_SetXY(0,2);		
		LCD_WriteString("MID_VALUE_1:");
		LCD_WriteInt(mid_value_pwm1);
		LCD_SetXY(0,3);
		LCD_WriteString("1.Set_A   2.Set_M");
		
		if(KeyReady==1)
		  {
			KeyReady=0;
			switch(KeyValue)
			{
				
				case key1:
					Input_DoubleValue(&steer_angle1,"angle");
					SET_ANGLE(steer_angle1,1);
					break;
				case key2:
					Input_IntValue(&mid_value_pwm1,"mid_value_pwm1");
					break;
				case keyback:
					return;
			}
		  }
		delay(20);
	}
}


 double steer_angle2=0;
void set_Engine_pwm2()
{
	steer_angle2=0;
	while(1)
	{
		refresh();	
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("========ENGINE2========");
		LCD_SetXY(0,1);
		LCD_WriteString("angle:");
		LCD_WriteDouble(steer_angle2,2);
		LCD_SetXY(0,2);		
		LCD_WriteString("mid_value_pwm2:");
		LCD_WriteInt(mid_value_pwm2);
		LCD_SetXY(0,3);
		LCD_WriteString("1.Set_A   2.Set_M");
		if(KeyReady==1)
		  {
			KeyReady=0;
			switch(KeyValue)
			{
				
				case key1:
					Input_DoubleValue(&steer_angle2,"angle");
					SET_ANGLE(steer_angle2,2);
					break;
				case key2:
					Input_IntValue(&mid_value_pwm2,"mid_value_pwm2");
					break;
				case keyback:
					
					return;
			}
		  }
		delay(20);
	}
}
