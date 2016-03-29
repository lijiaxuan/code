/* Includes ------------------------------------------------------------------*/
#include "all.h"
#include<math.h>


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

/*
 * 函数名: sys_GPS
 * 描  述: GPS显示与修改
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */

double StartAngle;
extern int show;
void sys_GPS()
{
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("========GPS========");
		LCD_SetXY(0,1);
		LCD_WriteString("Angle:");
		LCD_WriteDouble(Gps_List[0].angle,1);
        LCD_WriteString("  Speed:");
		LCD_WriteDouble(Gps_List[0].speed,1);
		LCD_SetXY(0,2);
		LCD_WriteString("X:");
		LCD_WriteDouble(Gps_List[0].position.x,2);
		LCD_WriteString("   Y:");
		LCD_WriteDouble(Gps_List[0].position.y,2);
		LCD_SetXY(0,3);
		LCD_WriteString("1.Set_A   2.Set_S");
		LCD_SetXY(0,4);
		LCD_WriteString("3.Set_X   4.Set_Y");
		LCD_SetXY(0,5);
		LCD_WriteString("5.Clear");
		LCD_SetXY(0,6);
		LCD_WriteString("StartA: ");
		StartAngle = Gps_List[0].radian*180/PI;
		LCD_WriteDouble(StartAngle,1);
        
        LCD_SetXY(0,7);
		LCD_WriteString("Length");
        LCD_WriteDouble(hypot(Gps_List[0].position.x,Gps_List[0].position.y),1);
		refresh();	
                
        if(KeyReady==1)
        {
            KeyReady=0;
            switch(KeyValue)
            {
                
                case key1:

                    Input_DoubleValue(&Gps_List[0].angle,"angle");
                    Gps_List[0].angle=Gps_List[0].angle*PI/180;
                    break;
                case key2:

                    Input_FloatValue(&Gps_List[0].speed,"speed");
                    break;
                case key3:

                    Input_DoubleValue(&Gps_List[0].position.x,"X");
                    break;
                case key4:

                    Input_DoubleValue(&Gps_List[0].position.y,"Y");
                    break;
                case key5:

                    GPS_Clear();
                    break;
                case keyback:
                    return;
            }
         }
        delay(LCD_DELAY);
	}
}

void Camer(void)
{
    while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("========CAMER========");
		LCD_SetXY(0,1);
		LCD_WriteString("Dis:");
		LCD_WriteDouble(camerError,1);
//         LCD_WriteString("  Speed:");
// 		LCD_WriteDouble(Gps_List[0].speed,1);
// 		LCD_SetXY(0,2);
// 		LCD_WriteString("X:");
// 		LCD_WriteDouble(Gps_List[0].position.x,2);
// 		LCD_WriteString("   Y:");
// 		LCD_WriteDouble(Gps_List[0].position.y,2);
// 		LCD_SetXY(0,3);
// 		LCD_WriteString("1.Set_A   2.Set_S");
// 		LCD_SetXY(0,4);
// 		LCD_WriteString("3.Set_X   4.Set_Y");
// 		LCD_SetXY(0,5);
// 		LCD_WriteString("5.Clear");
// 		LCD_SetXY(0,6);
// 		LCD_WriteString("StartA: ");
// 		StartAngle = Gps_List[0].radian*180/PI;
// 		LCD_WriteDouble(StartAngle,1);
//         
//         LCD_SetXY(0,7);
// 		LCD_WriteString("Length");
//         LCD_WriteDouble(hypot(Gps_List[0].position.x,Gps_List[0].position.y),1);
		refresh();	
                
        if(KeyReady==1)
        {
            KeyReady=0;
            switch(KeyValue)
            {
                
//                 case key1:

//                     Input_DoubleValue(&Gps_List[0].angle,"angle");
//                     Gps_List[0].angle=Gps_List[0].angle*PI/180;
//                     break;
//                 case key2:

//                     Input_FloatValue(&Gps_List[0].speed,"speed");
//                     break;
//                 case key3:

//                     Input_DoubleValue(&Gps_List[0].position.x,"X");
//                     break;
//                 case key4:

//                     Input_DoubleValue(&Gps_List[0].position.y,"Y");
//                     break;
//                 case key5:

//                     GPS_Clear();
//                     break;
                case keyback:
                    return;
            }
         }
        delay(LCD_DELAY);
	}
}
/********************************    END OF FILE    ***************************/
