/* Includes ------------------------------------------------------------------*/
#include "gyro.h"
#include "lcd.h"

/*
 * 函数名: set_Gyro
 * 描  述: Gyro显示与修改
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */

void set_Gyro(void)
{
	while(1)
	{
    refresh();	
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======Gyro======");
		LCD_SetXY(0,1);
		LCD_WriteString("Angle:");
		LCD_WriteDouble(Gyro_Angle_Total,3);
		LCD_SetXY(0,2);
		LCD_WriteString("Convert:");
		LCD_WriteDouble(Gyro_Convert1,3);
		LCD_SetXY(0,3);
		LCD_WriteString("1.Set_A   2.Set_C1");
		LCD_SetXY(0,4);
		LCD_WriteString("3.Set_C1  4.CheckFloat");
		
    if(KeyReady==1)		
				 {
					 KeyReady=0;
					switch(KeyValue)
						{
							case key1:
								Input_DoubleValue(&Gyro_Angle_Total,"A");
								break;
							case key2:
								Input_DoubleValue(&Gyro_Convert1,"C");
							case key3:
								Input_DoubleValue(&Gyro_Convert2,"C");
								break;
							case key4:
								Gyro_CheckFloat();
								break;
							case keyback:
								return;
						}
					}
		delay(20);
		}
		
	}
