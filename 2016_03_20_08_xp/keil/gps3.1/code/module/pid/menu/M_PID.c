#ifndef M_PID_C
#define M_PID_C

#include "lcd.h"
#include "pid.h"
/*
 * 函数名: set_PID
 * 描  述: PID显示与修改
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */

// void set_My_PID(void)
// {
// 	while(1)
// 	{
//     refresh();	
// 		LCD_Clear();
// 		LCD_SetXY(0,0);
// 		LCD_WriteString("======MY_PID======");
// 		LCD_SetXY(0,1);
// 		LCD_WriteString("K0:");
// 		LCD_WriteDouble(k0,1);
// 		LCD_SetXY(0,2);
// 		LCD_WriteString("K1:");
// 		LCD_WriteDouble(k1,1);
// 		LCD_SetXY(0,3);
// 		LCD_WriteString("K2:");
// 		LCD_WriteDouble(k2,1);
// 		
// 		LCD_SetXY(0,4);
// 		LCD_WriteString("D:");
// 		LCD_WriteDouble(pid_adapt.d,1);
// 		
// 		LCD_SetXY(0,5);
// 		LCD_WriteString("CON:");
// 		LCD_WriteDouble(con_p,1);
// 		
// 		LCD_SetXY(0,6);
// 		LCD_WriteString("1.S_K0 2.S_K1 3.S_K2");
// 		LCD_SetXY(0,7);
// 		LCD_WriteString("4.Set_D    5.set_C ");	
// 		
//     if(KeyReady==1)		
// 				 {
// 					 KeyReady=0;
// 					switch(KeyValue)
// 						{
// 							case key1:
// 								Input_FloatValue(&k0,"k0");
// 								break;
// 							case 2:
// 								Input_FloatValue(&k1,"k1");
// 								break;
// 							case key3:
// 								Input_FloatValue(&k2,"k2");
// 								break;
// 							case key4:
// 								Input_FloatValue(&pid_adapt.d,"d");
// 								break;
// 							case key5:
// 								Input_FloatValue(&con_p,"con");
// 								break;
// 							case keyback:
// 								return;
// 						}
// 					}
// 		delay(5);
// 		}
// 		
// 	}
	
	void set_R9_PID(void)
{
	while(1)
	{
    refresh();	
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======R9_PID======");
		LCD_SetXY(0,1);
		LCD_WriteString("P:");
		LCD_WriteDouble(Pid_List[9].p,3);
		LCD_SetXY(0,2);
		LCD_WriteString("I:");
		LCD_WriteDouble(Pid_List[9].i,3);
		LCD_SetXY(0,3);
		LCD_WriteString("D:");
		LCD_WriteDouble(Pid_List[9].d,3);
		LCD_SetXY(0,4);
		LCD_WriteString("1.Set_P   2.Set_I");
		LCD_SetXY(0,5);
		LCD_WriteString("3.Set_D");	
		
    if(KeyReady==1)		
				 {
					 KeyReady=0;
					switch(KeyValue)
						{
							case key1:
								Input_FloatValue(&Pid_List[9].p,"p");
								break;
							case 2:
								Input_FloatValue(&Pid_List[9].i,"i");
								break;
							case key3:
								Input_FloatValue(&Pid_List[9].d,"d");
								break;
							case keyback:
								return;
						}
					}
		delay(20);
		}
		
	}

	void set_R11_PID(void)
{
	while(1)
	{
    refresh();	
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======R11_PID======");
		LCD_SetXY(0,1);
		LCD_WriteString("P:");
		LCD_WriteDouble(Pid_List[11].p,3);
		LCD_SetXY(0,2);
		LCD_WriteString("I:");
		LCD_WriteDouble(Pid_List[11].i,3);
		LCD_SetXY(0,3);
		LCD_WriteString("D:");
		LCD_WriteDouble(Pid_List[11].d,3);
		LCD_SetXY(0,4);
		LCD_WriteString("1.Set_P   2.Set_I");
		LCD_SetXY(0,5);
		LCD_WriteString("3.Set_D");	
		
    if(KeyReady==1)		
				 {
					 KeyReady=0;
					switch(KeyValue)
						{
							
							case key1:
								Input_FloatValue(&Pid_List[11].p,"p");
								break;
							case 2:
								Input_FloatValue(&Pid_List[11].i,"i");
								break;
							case key3:
								Input_FloatValue(&Pid_List[11].d,"d");
								break;
							case keyback:
								return;
						}
					}
		delay(20);
		}
		
	}

	
	void set_R13_PID(void)
{
	while(1)
	{
    refresh();	
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======R13_PID======");
		LCD_SetXY(0,1);
		LCD_WriteString("P:");
		LCD_WriteDouble(Pid_List[13].p,3);
		LCD_SetXY(0,2);
		LCD_WriteString("I:");
		LCD_WriteDouble(Pid_List[13].i,3);
		LCD_SetXY(0,3);
		LCD_WriteString("D:");
		LCD_WriteDouble(Pid_List[13].d,3);
		LCD_SetXY(0,4);
		LCD_WriteString("1.Set_P   2.Set_I");
		LCD_SetXY(0,5);
		LCD_WriteString("3.Set_D");	
		
    if(KeyReady==1)		
				 {
					 KeyReady=0;
					switch(KeyValue)
						{
							
							case key1:
								Input_FloatValue(&Pid_List[13].p,"p");
								break;
							case 2:
								Input_FloatValue(&Pid_List[13].i,"i");
								break;
							case key3:
								Input_FloatValue(&Pid_List[13].d,"d");
								break;
							case keyback:
								return;
						}
					}
					
		delay(20);
		}

	}

	void set_R14_PID(void)
{
	while(1)
	{
    refresh();	
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======R14_PID======");
		LCD_SetXY(0,1);
		LCD_WriteString("P:");
		LCD_WriteDouble(Pid_List[14].p,3);
		LCD_SetXY(0,2);
		LCD_WriteString("I:");
		LCD_WriteDouble(Pid_List[14].i,3);
		LCD_SetXY(0,3);
		LCD_WriteString("D:");
		LCD_WriteDouble(Pid_List[14].d,3);
		LCD_SetXY(0,4);
		LCD_WriteString("1.Set_P   2.Set_I");
		LCD_SetXY(0,5);
		LCD_WriteString("3.Set_D");	

    if(KeyReady==1)		
				 {
					 KeyReady=0;
					switch(KeyValue)
						{
							
							case key1:
								Input_FloatValue(&Pid_List[14].p,"p");
								break;
							case 2:
								Input_FloatValue(&Pid_List[14].i,"i");
								break;
							case key3:
								Input_FloatValue(&Pid_List[14].d,"d");
								break;
							case keyback:
								return;
						}
					}
		delay(20);		
	}
		
}

	void set_R16_PID(void)
{
	while(1)
	{
    refresh();	
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======R16_PID======");
		LCD_SetXY(0,1);
		LCD_WriteString("P:");
		LCD_WriteDouble(Pid_List[16].p,3);
		LCD_SetXY(0,2);
		LCD_WriteString("I:");
		LCD_WriteDouble(Pid_List[16].i,3);
		LCD_SetXY(0,3);
		LCD_WriteString("D:");
		LCD_WriteDouble(Pid_List[16].d,3);
		LCD_SetXY(0,4);
		LCD_WriteString("1.Set_P   2.Set_I");
		LCD_SetXY(0,5);
		LCD_WriteString("3.Set_D");	

    if(KeyReady==1)		
				 {
					 KeyReady=0;
					switch(KeyValue)
						{
							
							case key1:
								Input_FloatValue(&Pid_List[16].p,"p");
								break;
							case 2:
								Input_FloatValue(&Pid_List[16].i,"i");
								break;
							case key3:
								Input_FloatValue(&Pid_List[16].d,"d");
								break;
							case keyback:
								return;
						}
					}
		delay(20);		
	}
		
}

	void set_R18_PID(void)
{
	while(1)
	{
    refresh();	
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======R18_PID======");
		LCD_SetXY(0,1);
		LCD_WriteString("P:");
		LCD_WriteDouble(Pid_List[18].p,3);
		LCD_SetXY(0,2);
		LCD_WriteString("I:");
		LCD_WriteDouble(Pid_List[18].i,3);
		LCD_SetXY(0,3);
		LCD_WriteString("D:");
		LCD_WriteDouble(Pid_List[18].d,3);
		LCD_SetXY(0,4);
		LCD_WriteString("1.Set_P   2.Set_I");
		LCD_SetXY(0,5);
		LCD_WriteString("3.Set_D");	

    if(KeyReady==1)		
				 {
					 KeyReady=0;
					switch(KeyValue)
						{
							
							case key1:
								Input_FloatValue(&Pid_List[18].p,"p");
								break;
							case 2:
								Input_FloatValue(&Pid_List[18].i,"i");
								break;
							case key3:
								Input_FloatValue(&Pid_List[18].d,"d");
								break;
							case keyback:
								return;
						}
					}
		delay(20);		
	}
		
}

	void set_R20_PID(void)
{
	while(1)
	{
    refresh();	
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======R20_PID======");
		LCD_SetXY(0,1);
		LCD_WriteString("P:");
		LCD_WriteDouble(Pid_List[20].p,3);
		LCD_SetXY(0,2);
		LCD_WriteString("I:");
		LCD_WriteDouble(Pid_List[20].i,3);
		LCD_SetXY(0,3);
		LCD_WriteString("D:");
		LCD_WriteDouble(Pid_List[20].d,3);
		LCD_SetXY(0,4);
		LCD_WriteString("1.Set_P   2.Set_I");
		LCD_SetXY(0,5);
		LCD_WriteString("3.Set_D");	

    if(KeyReady==1)		
				 {
					 KeyReady=0;
					switch(KeyValue)
						{
							
							case key1:
								Input_FloatValue(&Pid_List[20].p,"p");
								break;
							case 2:
								Input_FloatValue(&Pid_List[20].i,"i");
								break;
							case key3:
								Input_FloatValue(&Pid_List[20].d,"d");
								break;
							case keyback:
								return;
						}
					}
		delay(20);		
	}
		
}

	void set_R22_PID(void)
{
	while(1)
	{
    refresh();	
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("======R22_PID======");
		LCD_SetXY(0,1);
		LCD_WriteString("P:");
		LCD_WriteDouble(Pid_List[22].p,3);
		LCD_SetXY(0,2);
		LCD_WriteString("I:");
		LCD_WriteDouble(Pid_List[22].i,3);
		LCD_SetXY(0,3);
		LCD_WriteString("D:");
		LCD_WriteDouble(Pid_List[22].d,3);
		LCD_SetXY(0,4);
		LCD_WriteString("1.Set_P   2.Set_I");
		LCD_SetXY(0,5);
		LCD_WriteString("3.Set_D");	

    if(KeyReady==1)		
				 {
					 KeyReady=0;
					switch(KeyValue)
						{
							
							case key1:
								Input_FloatValue(&Pid_List[22].p,"p");
								break;
							case 2:
								Input_FloatValue(&Pid_List[22].i,"i");
								break;
							case key3:
								Input_FloatValue(&Pid_List[22].d,"d");
								break;
							case keyback:
								return;
						}
					}
		delay(20);		
	}
		
}


	void set_BIG_PID(void)
{
	float mid_p,mid_i,mid_d;
	int u;
	while(1)
	{
		refresh();	
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

    if(KeyReady==1)		
				 {
					 KeyReady=0;
					switch(KeyValue)
						{
							
							case key1:
								Input_FloatValue(&mid_p,"p");
							  for(u=1;u<=7;u=u+2)
							    {
								   Pid_List[u].p=mid_p;
							    }
									u=0;
								break;
							case 2:
								Input_FloatValue(&mid_i,"i");
							  for(u=1;u<=7;u=u+2)
							    {
								   Pid_List[u].i=mid_i;
							    }
									u=0;
								break;
							case key3:
								Input_FloatValue(&mid_d,"d");
							  for(u=1;u<=7;u=u+2)
							    {
								   Pid_List[u].d=mid_d;
							    }
									u=0;
								break;
							case keyback:
								return;
						}
					}
		delay(20);		
	}
}

void set_LINE_PID(void)
{
	float mid_p,mid_i,mid_d;
	int u;
	while(1)
	{
    refresh();	
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

		
		
    if(KeyReady==1)		
				 {
					 KeyReady=0;
					switch(KeyValue)
						{
							
							case key1:
								Input_FloatValue(&mid_p,"p");
							  for(u=0;u<=12;u=u+2)
							    {
								   Pid_List[u].p=mid_p;
							    }
								for(u=15;u<=23;u=u+2)
									{
									 Pid_List[u].p=mid_p;
									}
									u=0;
								break;
							case 2:
								Input_FloatValue(&mid_i,"i");
							  for(u=0;u<=12;u=u+2)
							    {
								   Pid_List[u].i=mid_i;
							    }
								for(u=15;u<=23;u=u+2)
									{
									 Pid_List[u].i=mid_i;
									}
									u=0;
								break;
							case key3:
								Input_FloatValue(&mid_d,"d");
							  for(u=0;u<=12;u=u+2)
							    {
								   Pid_List[u].d=mid_d;
							    }
								for(u=15;u<=23;u=u+2)
									{
									 Pid_List[u].d=mid_d;
									}									
									u=0;
								break;
							case keyback:
								return;
						}
					}
		delay(20);		
	}
}
#endif 
