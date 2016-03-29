#include "M_Encoder.h"

/*
 * 函数名: set_Encoder
 * 描  述: 码盘显示与修改
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */

void set_Encoder(void)
{
	while(1)
	{
    refresh();	
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======Encoder======");
		LCD_SetXY(0,1);
		LCD_WriteString("Distance1:");
		LCD_WriteDouble(Encoders[0].distance,3);
		LCD_SetXY(0,2);
		LCD_WriteString("Convert1:");
		LCD_WriteDouble(Encoders_par[0].convert,3);
		LCD_SetXY(0,3);
		LCD_WriteString("Distance2:");
		LCD_WriteDouble(Encoders[1].distance,3);
		LCD_SetXY(0,4);
		LCD_WriteString("Convert2:");
		LCD_WriteDouble(Encoders_par[1].convert,3);
		LCD_SetXY(0,5);
		LCD_WriteString("1.Set_D1   2.Set_C1");
		LCD_SetXY(0,6);
		LCD_WriteString("3.Set_D2   4.Set_C2");		
		
    if(KeyReady==1)		
				 {
					 KeyReady=0;
					switch(KeyValue)
						{
							case key1:
								Input_FloatValue(&Encoders[0].distance,"D1");
								break;
							case 2:
								Input_FloatValue(&Encoders_par[0].convert,"C1");
								break;
							case key3:
								Input_FloatValue(&Encoders[1].distance,"D2");
								break;
							case key4:
								Input_FloatValue(&Encoders_par[1].convert,"C2");
								break;
							case keyback:
								return;
						}
					}
		delay(20);
		}
		
	}

