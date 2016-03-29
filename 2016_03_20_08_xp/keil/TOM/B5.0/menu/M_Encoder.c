/* Includes ------------------------------------------------------------------*/
#include "all.h"

/*
 * 函数名: set_Encoder
 * 描  述: 码盘显示与修改
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */

void Encoder_Set(void)
{
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======Encoder======");
		LCD_SetXY(0,1);
		LCD_WriteString("Distance1:");
		LCD_WriteDouble(Encoders[0].Distance,3);
		LCD_SetXY(0,2);
		LCD_WriteString("Convert1:");
		LCD_WriteDouble(Encoders[0].Convert,3);
		LCD_SetXY(0,3);
		LCD_WriteString("Distance2:");
		LCD_WriteDouble(Encoders[1].Distance,3);
		LCD_SetXY(0,4);
		LCD_WriteString("Convert2:");
		LCD_WriteDouble(Encoders[1].Convert,3);
		LCD_SetXY(0,5);
		LCD_WriteString("1.Set_D1   2.Set_C1");
		LCD_SetXY(0,6);
		LCD_WriteString("3.Set_D2   4.Set_C2");		
        
		refresh();
        
        if(KeyReady==1)		
         {
             KeyReady=0;
            switch(KeyValue)
                {
                    case key1:
                        Input_DoubleValue(&Encoders[0].Distance,"D1");
                        break;
                    case 2:
                        Input_DoubleValue(&Encoders[0].Convert,"C1");
                        break;
                    case key3:
                        Input_DoubleValue(&Encoders[1].Distance,"D2");
                        break;
                    case key4:
                        Input_DoubleValue(&Encoders[1].Convert,"C2");
                        break;
                    case keyback:
                        return;
                }
            }
		delay(LCD_DELAY);
		}
		
	}

    /*
 * 函数名: sys_Encoder
 * 描  述: 码盘相关功能
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */
void sys_Encoder()
{	
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======Encoder=======");
		LCD_SetXY(0,1);
		LCD_WriteString("Dis1:");
		LCD_WriteDouble(Encoders[0].Distance,3);
		LCD_SetXY(0,2);
		LCD_WriteString("Dis2:");
		LCD_WriteDouble(Encoders[1].Distance,3);
		LCD_SetXY(0,3);
		LCD_WriteString("1.Init_Con 2.View");
		LCD_SetXY(0,4);
		LCD_WriteString("3.Set ");
		LCD_SetXY(0,5);
		LCD_WriteString("X:");
 		LCD_WriteDouble(Gps_List[0].position.x,6);
		LCD_WriteInt(Gps_List[0].position.x);//写整数部分
		LCD_SetXY(0,6);
		LCD_WriteString("Y:");
 		LCD_WriteDouble(Gps_List[0].position.y,6);
		LCD_WriteInt(Gps_List[0].position.y);//写整数部分
		refresh();
		if(KeyReady==1)
		{
            KeyReady=0;
			switch(KeyValue)
			{
				case key1:
					Encoder_Init_Con();
					break;
				case key2:
					Encoder_View();
					break;
				case key3:
					Encoder_Set();
					break;
				case keyback:
					return;
			}
		}
		delay(LCD_DELAY);
	}
}

void Encoder_Init_Con(void)
{
	LCD_Clear();
	LCD_SetXY(0,0);
	LCD_WriteString("Find a refrence line and press OK to mark the start position");
	
	while(KeyReady != 1)
    {
        refresh();
        delay(LCD_DELAY);
    }
    KeyReady=0;
    
    Encoder_Clear(0);
	Encoder_Clear(1);
    
	
	while(KeyReady != 1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("Push Line 3m");
		LCD_SetXY(0,1);
		LCD_WriteString("Press OK to end");
		LCD_SetXY(0,2);
		LCD_WriteDouble(Encoders[0].Distance,10);
		LCD_SetXY(0,3);
		LCD_WriteDouble(Encoders[1].Distance,10);
        refresh();
		delay(LCD_DELAY);
	}
    KeyReady=0;
	
    E_Init_Con();
    
	LCD_Clear();
	LCD_SetXY(0,0);
	LCD_WriteString("C11:");
	LCD_WriteDouble(Encoders[0].Convert,10);
	LCD_SetXY(0,1);
	LCD_WriteString("C12:");
	LCD_WriteDouble(Encoders[1].Convert,10);

	LCD_SetXY(0,6);
	LCD_WriteString("Press any key to continue...");
	
	while(KeyReady != 1)
    {
        refresh();
        delay(LCD_DELAY);
    }
    KeyReady=0;
}

/*
 * 函数名: Encoder_View
 * 描  述: 查看码盘的信息
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */
void Encoder_View(void)
{
	
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("convert1:");
		LCD_WriteDouble(Encoders[0].Convert,6);
		LCD_SetXY(0,1);
		LCD_WriteString("convert2:");
		LCD_WriteDouble(Encoders[1].Convert,6);

		
        while(KeyReady != 1)
        {
            refresh();
            delay(LCD_DELAY);
        }
        KeyReady=0;
        
        switch(KeyValue)
        {
            case keyback:
            return;
        }
		delay(LCD_DELAY);
	}
}

