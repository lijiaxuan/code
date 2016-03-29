/* Includes ------------------------------------------------------------------*/
#include "all.h"


void Gyro_Init(void)
{	
	LCD_Clear();
	LCD_SetXY(0,0);
	LCD_WriteString("Find a refrence border and press OK to mark the start angle");
	
    while(KeyReady != 1)
    {
        refresh();
        delay(LCD_DELAY);
    }
    KeyReady=0;
    
    Gyro_Clear();
	
	LCD_Clear();
	LCD_SetXY(0,0);
	LCD_WriteString("Press any key to start turning +10 cicles.");
    
	while(KeyReady != 1)
    {
        refresh();
        delay(LCD_DELAY);
    }
    KeyReady=0;
    

	while(KeyReady!= 1)
	{		
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("turning...");
		LCD_SetXY(0,1);
		LCD_WriteString("Press any key to stop");
        
        LCD_SetXY(0,2);
        LCD_WriteDouble(Gyro_Angle_Total,3);
        
        refresh();
		delay(LCD_DELAY);
	}
    KeyReady=0;
	
    Gyro_Init_P();
    Gyro_Clear();
    
    
	LCD_Clear();
	LCD_SetXY(0,0);
	LCD_WriteString("Press any key to start turning -10 cicles.");
    
	while(KeyReady != 1)
        {
            refresh();
            delay(LCD_DELAY);
        }
    KeyReady=0;
	
	while(KeyReady != 1)
	{		
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("turning...");
		LCD_SetXY(0,1);
		LCD_WriteString("Press any key to stop");
        
        LCD_SetXY(0,2);
        LCD_WriteDouble(Gyro_Angle_Total,3);
        
		refresh();
		delay(LCD_DELAY);
	}
    KeyReady=0;
	
    Gyro_Init_N();
	
	delay(LCD_DELAY);
	
	LCD_Clear();
	LCD_SetXY(0,0);
	LCD_WriteString("Con1:");
	LCD_WriteDouble(Gyro_Convert1,10);
	LCD_SetXY(0,1);
	LCD_WriteString("Con2:");
	LCD_WriteDouble(Gyro_Convert2,10);
	LCD_SetXY(0,2);
	LCD_WriteString("Press any key to continue...");
    
	while(KeyReady != 1)
    {
        refresh();
        delay(LCD_DELAY);
    }
    KeyReady=0;
}


/*
 * 函数名: set_Gyro
 * 描  述: Gyro显示与修改
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */

void Gyro_Set(void)
{
	while(1)
	{
    	
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
		LCD_WriteString("Convert:");
		LCD_WriteDouble(Gyro_Convert2,3);
		LCD_SetXY(0,4);
		LCD_WriteString("1.Set_A   2.Set_C_1  3.Set_C_2");
		
        refresh();
        
        if(KeyReady==1)		
         {
             KeyReady=0;
            switch(KeyValue)
            {
                case key1:
                    Input_DoubleValue(&Gyro_Angle_Total,"A");
                    break;
                case key2:
                    Input_DoubleValue(&Gyro_Convert1,"C_1");
                    break;
                case key3:
                    Input_DoubleValue(&Gyro_Convert2,"C_2");
                    break;
                case keyback:
                    return;
            }
        }
		delay(LCD_DELAY);
    }
}

void sys_Gyro(void)
{
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("========Gyro========");
		LCD_SetXY(0,1);
		LCD_WriteString("angle:");
		LCD_WriteDouble(Gyro_Angle_Total,6);
		LCD_SetXY(0,2);
		LCD_WriteString("Con1:");
		LCD_WriteDouble(Gyro_Convert1,6);
		LCD_SetXY(0,3);
		LCD_WriteString("Con2:");
		LCD_WriteDouble(Gyro_Convert2,6);
		LCD_SetXY(0,4);
        LCD_WriteString("Float_Num:");
        LCD_WriteInt(Gyro_Float);
        
        LCD_SetXY(0,5);
        LCD_WriteString("value:");
        LCD_WriteInt(Gyro_Now.value);
        
        LCD_SetXY(0,6);
		LCD_WriteString("1.Init 2.Set");
        LCD_SetXY(0,7);
		LCD_WriteString("3.Float 4.Clear");

		refresh();
        
		if(KeyReady==1)
		{
            KeyReady=0;
			switch(KeyValue)
			{
				case key1:
					Gyro_Init();
					break;
				case key2:
					Gyro_Set();
					break;
				case key3:
					Gyro_CheckFloat();
					break;
                case key4:
                    Gyro_Clear();
                    break;
				case keyback:
					return;
			}
		}
		delay(LCD_DELAY);
	}
}
