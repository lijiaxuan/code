/* Includes ------------------------------------------------------------------*/
#include "all.h"
/*
 * 函数名: set_PID
 * 描  述: PID显示与修改
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */

void set_My_PID(void)
{
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======MY_PID======");
		LCD_SetXY(0,1);
		LCD_WriteString("K0:");
		LCD_WriteDouble(k0,1);
		LCD_SetXY(0,2);
		LCD_WriteString("K1:");
		LCD_WriteDouble(k1,1);
		LCD_SetXY(0,3);
		LCD_WriteString("K2:");
		LCD_WriteDouble(k2,1);
		
		LCD_SetXY(0,4);
		LCD_WriteString("D:");
		LCD_WriteDouble(pid_adapt.d,1);
		
		LCD_SetXY(0,5);
		LCD_WriteString("CON:");
		LCD_WriteDouble(con_p,1);
		
		LCD_SetXY(0,6);
		LCD_WriteString("1.S_K0 2.S_K1 3.S_K2");
		LCD_SetXY(0,7);
		LCD_WriteString("4.Set_D    5.set_C ");	
		
        refresh();	
        if(KeyReady==1)		
         {
             KeyReady=0;
             switch(KeyValue)
              {
                case key1:
                    Input_FloatValue(&k0,"k0");
                    break;
                case 2:
                    Input_FloatValue(&k1,"k1");
                    break;
                case key3:
                    Input_FloatValue(&k2,"k2");
                    break;
                case key4:
                    Input_FloatValue(&pid_adapt.d,"d");
                    break;
                case key5:
                    Input_FloatValue(&con_p,"con");
                    break;
                case keyback:
                    return;
             }
         }
		delay(LCD_DELAY);
	}
}

void set_BIG_PID(void)
{
	float mid_p,mid_i,mid_d;
	int u;
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======BIG_PID======");
		LCD_SetXY(0,1);
		LCD_WriteString("P:");
		LCD_WriteDouble(Pid_List[1].p,3);
		LCD_SetXY(0,2);
		LCD_WriteString("I:");
		LCD_WriteDouble(Pid_List[1].i,3);
		LCD_SetXY(0,3);
		LCD_WriteString("D:");
		LCD_WriteDouble(Pid_List[1].d,3);
		LCD_SetXY(0,4);
		LCD_WriteString("1.Set_P   2.Set_I");
		LCD_SetXY(0,5);
		LCD_WriteString("3.Set_D");	
        refresh();	
        
        if(KeyReady==1)		
         {
             KeyReady=0;
            switch(KeyValue)
            {
                
                case key1:
                    Input_FloatValue(&mid_p,"p");
                  for(u=1;u<10;u=u+2)
                    {
                       Pid_List[u].p=mid_p;
                    }
                        u=0;
                    break;
                case 2:
                    Input_FloatValue(&mid_i,"i");
                  for(u=1;u<10;u=u+2)
                    {
                       Pid_List[u].i=mid_i;
                    }
                        u=0;
                    break;
                case key3:
                    Input_FloatValue(&mid_d,"d");
                  for(u=1;u<10;u=u+2)
                    {
                       Pid_List[u].d=mid_d;
                    }
                        u=0;
                    break;
                case keyback:
                    return;
            }
          }
		delay(LCD_DELAY);		
	}
}

	void set_LINE_PID(void)
{
	float mid_p,mid_i,mid_d;
	int u;
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======LINE_PID======");
		LCD_SetXY(0,1);
		LCD_WriteString("P:");
		LCD_WriteDouble(Pid_List[0].p,3);
		LCD_SetXY(0,2);
		LCD_WriteString("I:");
		LCD_WriteDouble(Pid_List[0].i,3);
		LCD_SetXY(0,3);
		LCD_WriteString("D:");
		LCD_WriteDouble(Pid_List[0].d,3);
		LCD_SetXY(0,4);
		LCD_WriteString("1.Set_P   2.Set_I");
		LCD_SetXY(0,5);
		LCD_WriteString("3.Set_D");	

        refresh();	
        
    if(KeyReady==1)		
     {
         KeyReady=0;
        switch(KeyValue)
            {
                
                case key1:
                    Input_FloatValue(&mid_p,"p");
                  for(u=0;u<13;u=u+2)
                    {
                       Pid_List[u].p=mid_p;
                    }
                    for(u=15;u<24;u=u+2)
                        {
                         Pid_List[u].p=mid_p;
                        }
                        u=0;
                    break;
                case 2:
                    Input_FloatValue(&mid_i,"i");
                  for(u=0;u<13;u=u+2)
                    {
                       Pid_List[u].i=mid_i;
                    }
                    for(u=15;u<24;u=u+2)
                    {
                     Pid_List[u].i=mid_i;
                    }
                    u=0;
                    break;
                case key3:
                    Input_FloatValue(&mid_d,"d");
                    for(u=0;u<13;u=u+2)
                    {
                       Pid_List[u].d=mid_d;
                    }
                    for(u=15;u<24;u=u+2)
                        {
                         Pid_List[u].d=mid_d;
                        }									
                        u=0;
                    break;
                case keyback:
                    return;
            }
        }
		delay(LCD_DELAY);		
	}
}

int PidNum[]={11,13,14,16,18,20,22,23};

void SetPid(int pid_num)
{
    while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======    PID======");
        LCD_WriteIntXY(PidNum[pid_num],7,0);     
		LCD_SetXY(0,1);
		LCD_WriteString("P:");
		LCD_WriteDouble(Pid_List[PidNum[pid_num]].p,3);
		LCD_SetXY(0,2);
		LCD_WriteString("I:");
		LCD_WriteDouble(Pid_List[PidNum[pid_num]].i,3);
		LCD_SetXY(0,3);
		LCD_WriteString("D:");
		LCD_WriteDouble(Pid_List[PidNum[pid_num]].d,3);
		LCD_SetXY(0,4);
		LCD_WriteString("1.Set_P   2.Set_I");
		LCD_SetXY(0,5);
		LCD_WriteString("3.Set_D");	
        
        refresh();	
        
        if(KeyReady==1)		
         {
            KeyReady=0;
            switch(KeyValue)
            {
                
                case key1:
                    Input_FloatValue(&Pid_List[PidNum[pid_num]].p,"p");
                    break;
                case 2:
                    Input_FloatValue(&Pid_List[PidNum[pid_num]].i,"i");
                    break;
                case key3:
                    Input_FloatValue(&Pid_List[PidNum[pid_num]].d,"d");
                    break;
                case keyback:
                    return;
            }
         }
		delay(LCD_DELAY);		
	}
}

void ChosePid(void)
{
    while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("2.Choose PID NUM");
		
        LCD_SetXY(0,1);
		LCD_WriteString("1. ");
        LCD_WriteIntXY(PidNum[0],3,1);
        
        LCD_SetXY(10,1);
		LCD_WriteString("2. ");
        LCD_WriteIntXY(PidNum[1],13,1);
        
        LCD_SetXY(0,2);
		LCD_WriteString("3. ");
        LCD_WriteIntXY(PidNum[2],3,2);
        
        LCD_SetXY(10,2);
		LCD_WriteString("4. ");
        LCD_WriteIntXY(PidNum[3],13,2);
        
        LCD_SetXY(0,3);
		LCD_WriteString("5. ");
        LCD_WriteIntXY(PidNum[4],3,3);
        
        LCD_SetXY(10,3);
		LCD_WriteString("6. ");
        LCD_WriteIntXY(PidNum[5],13,3);
        
        LCD_SetXY(0,4);
		LCD_WriteString("7. ");
        LCD_WriteIntXY(PidNum[6],3,4);     
		
        LCD_SetXY(10,4);
		LCD_WriteString("8. ");
        LCD_WriteIntXY(PidNum[7],13,4);  
        
        refresh();
		
        if(KeyReady==1)
		{
			KeyReady=0;
            switch(KeyValue)
            {
                
            case key1:
                SetPid(0);
                break;
            case key2:
                SetPid(1);
                break;
            case key3:
                SetPid(2);
                break;
            
            case key4:
                SetPid(3);
                break;
            case key5:
                SetPid(4);
                break;
            case key6:
                SetPid(5);
                break;
            
            case key7:
                SetPid(6);
                break;
            
            case key8:
                SetPid(7);
                break;
            
            case keyback:
                return;
            }

		}
        delay(LCD_DELAY);
	}
}


